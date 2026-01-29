import { createClient } from "https://cdn.jsdelivr.net/npm/@supabase/supabase-js/+esm";

const SUPABASE_URL = "https://temfsuafvybgaagywnxd.supabase.co";
const SUPABASE_ANON_KEY = "sb_publishable_YfRP1S2oOt3yFgX1YqIQ5Q_3U_LleC5";

const supabase = createClient(SUPABASE_URL, SUPABASE_ANON_KEY);

// DOM (Document Object Model): JS references to HTML elements
const statusEl = document.getElementById("status");
const lastUpdatedEl = document.getElementById("lastUpdated");
const rowsEl = document.getElementById("rows");
const sensorFilterEl = document.getElementById("sensorFilter");
const rowLimitEl = document.getElementById("rowLimit");
const reloadBtn = document.getElementById("reload");

const LINE_CHART_SENSORS = [
    "INPUT_HPi", 
    "INPUT_HPo", 
    "INPUT_TANKi", 
    "INPUT_TANKo", 
    "INPUT_GNDi", 
    "INPUT_GNDo", 
    "INPUT_AIRo"
];

const HIDDEN_BY_DEFAULT = new Set([
  "INPUT_HPi",
  "INPUT_TANKi",
  "INPUT_GNDi"
]);

const BAR_CHART_SENSORS = [
    'MODE',
    'MODE_heat_floor',
    'MODE_heat_forced',
    'MODE_cool_forced',
    'MODE_proactive',
    'STATE_HP_O',
    'STATE_HP_Y',
    'STATE_PUMP_HP',
    'STATE_PUMP_BMNT',
    'STATE_PUMP_1ST2ND',
    'STATE_AH_W',
    'STATE_AH_O',
    'STATE_AH_Y',
    'STATE_AH_G',
    'CriticalFlag',
    'PowerAlarmLevelFlag',
    'TempAlarmLevelFlag',
    'RelayAlarmLevelFlag'
]

let inputLineChart;
const inputLineCanvas = document.getElementById("inputLineChart");
const inputLineContext = inputLineCanvas.getContext("2d");

let stateTimelineChart;
const stateTimelineCanvas = document.getElementById("stateTimelineChart");
const stateTimelineContext = stateTimelineCanvas.getContext("2d");

const STATES = [0, 1, 2];

const STATE_COLORS = {
    0: "#1f4ed8", // blue
    1: "#22c55e", // green
    2: "#ef4444" // red
};

let currentRows = []; //holds the current dataset from SUPABASE for the table
let realtimeChannel = null; //live connection used to receive updates from SUPABASE

function setStatus(msg) { //updates text shown in status area of the page
  statusEl.textContent = msg;
}

function setLastUpdated() { //states when data was last updated
    lastUpdatedEl.textContent =
        `Last updated: ${formatLocalTime(new Date())}`; //the time will be when it last checked for data
}

function formatLocalTime(ts) { //takes ISO string from SUPABASE as a parameter
    const date = new Date(ts); //formats in JS type

    return new Intl.DateTimeFormat(undefined, { //format dates according to the user's own language, region, and timezone
        year: "numeric", //full year
        month: "short", //short month name (eg. Jan)
        day: "2-digit", 
        hour: "2-digit",
        minute: "2-digit",
        second: "2-digit",
        timeZoneName: "short" //short timezone name (eg. EST)
    }).format(date); //formats into human readable string
}

function renderTable() { //rebuilds HTML table rows based on current data and the selected sensor filter
    const filter = sensorFilterEl.value; //sensor type the user wants to see
    const visible = filter ? currentRows.filter((r) => r.sensor_type === filter) : currentRows; //show only the filtered sensor types, or all of them if no sensor type is selected

    //table is fully re-rendered each time this function runs
    rowsEl.innerHTML = visible.map((row) => ` 
        <tr>
            <td>${formatLocalTime(row.time)}</td>
            <td>${escapeHtml(row.sensor_type)}</td>
            <td>${Number(row.value).toFixed(3)}</td>
        </tr>
        `
        ).join("");
}

function escapeHtml(string) { //converts special characters into safe text equivalents to not be interpreted as HTML or Javascript
  return String(string)
    .replaceAll("&", "&amp;")
    .replaceAll("<", "&lt;")
    .replaceAll(">", "&gt;")
    .replaceAll('"', "&quot;")
    .replaceAll("'", "&#039;");
}

function rowKey(row) { //creates a unique string identifier for a row
    return `${row.time}|${row.sensor_type}`;
}


function rebuildSensorFilterOptions() { //rebuilds sensor dropdown filter for all present sensor types
    const existing = sensorFilterEl.value; //saves currently selected sensor in the dropdown
    const sensors = Array.from(new Set(currentRows.map((row) => row.sensor_type))).sort(); //created an alphabetically sorted array of current sensor names with no duplicates

    sensorFilterEl.innerHTML = `<option value="">All</option>` + sensors.map((string) => `<option value="${escapeHtml(string)}">${escapeHtml(string)}</option>`).join(""); //creates one option per sensor, plus keeps the "ALL" option

    if (sensors.includes(existing)) sensorFilterEl.value = existing; //keeps existing user selection if it still exists
    else sensorFilterEl.value = ""; //falls back to "All" if previous option no longer exists
}

function createLineChart() {
    inputLineChart = new Chart(inputLineContext, {  //inputLineContext is the space that the chart is drawn on
        type: "line", //line chart
        data: {  //holds all data that will be plotted
            datasets: LINE_CHART_SENSORS.map((name) => ({
                label: name,
                data: [], //where the {x,y} points will go
                hidden: HIDDEN_BY_DEFAULT.has(name) 
            }))
        },
        options: { //controls how the chart behaves and looks
            scales: { //axes behaviour
                x: {
                    type: "time",
                    time: {
                        unit: "hour"
                    }
                },
                y: {
                    beginAtZero: false, //do not autoscale at zero, let it autoscale based on the data
                    title: {
                        display: true,
                        text: "Temperature (°C)"
                    }
                }
            }
        }
    });
}

function createStateTimelineChart() {
  stateTimelineChart = new Chart(stateTimelineContext, {
    type: "line", //line chart
        data: {  //holds all data that will be plotted
            datasets: BAR_CHART_SENSORS.map((name) => ({
                label: name,
                data: [], //where the {x,y} points will go
                stepped: true,
                pointRadius: 0,
                tension: 0,
                segment: {
                    borderColor: (ctx) => {
                        const raw = ctx.p0?.raw;
                        if (!raw) return "#9ca3af";

                        const OFFSET = 10;
                        const state = Math.round(raw.y % OFFSET);

                        return STATE_COLORS[state] ?? "#9ca3af";
                    }
                }
            }))
        },
        options: {
            plugins: {
                legend: { display: false },
                afterDatasetsDraw: (chart) => {
                    const { ctx, scales: { x } } = chart;
                    ctx.save();
                    ctx.font = "12px sans-serif";
                    ctx.fillStyle = "#374151";
                    ctx.textBaseline = "middle";

                    chart.data.datasets.forEach((dataset, i) => {
                        if (dataset.hidden) return;
                        const meta = chart.getDatasetMeta(i);
                        if (!meta.data.length) return;

                        const p = meta.data[meta.data.length - 1];
                        ctx.fillText(dataset.label, x.right + 6, p.y);
                    });

                    ctx.restore();
                }
            },
            scales: {
                x: { type: "time", time: { unit: "hour" } },
                y: {
                    ticks: {
                        autoSkip: false,
                        stepSize: 10, // must match your OFFSET
                        callback: (value) => {
                            const OFFSET = 10;
                            const index = Math.round(value / OFFSET);
                            return BAR_CHART_SENSORS[index] ?? "";
                        }
                    }
                }


            }
        }

    });
}



async function loadInitialData() { //loads the first batch of data from SUPABASE when the page starts, populates the table and filters, and updates the UI state
    try {
        setStatus("Loading..."); //informs user the data is being fetched
        const limit = Number(rowLimitEl.value); //reads the max number of rows teh user wants to display in the table

        const {data, error} = await supabase //calls SUPABASE asynchronously
            .from("data")
            .select("time, sensor_type, value")
            .order("time", { ascending: false }) //sorts by newest records first in the table
            .limit(limit); //limits results to match the UI row limit

        if (error) throw error; //goes to catch if SUPABASE reports an error

        currentRows = data ?? []; //stores fetched data locally
        rebuildSensorFilterOptions();
        renderTable();
        setLastUpdated();
        setStatus("Live (initial load complete)");
    } catch (err) {
        console.error(err); //logs the error
        setStatus(`Error loading: ${err.message ?? err}`); //sends readable error message to user
    }
}

async function loadChartDataToday() { //loads all chart points for today, grouped by sensor, in the line chart
    //computes "today" time window
    const startOfDay = new Date();
    startOfDay.setHours(0, 0, 0, 0);
    const endOfDay = new Date(startOfDay);
    endOfDay.setHours(23, 59, 59, 999);

    //creates an array/bucket for each sensor's data
    const pointsOfSensor = new Map(LINE_CHART_SENSORS.map(s => [s, []]));

    //requests small batches until all rows of the day are received
    //pagination must be done otherwise only a limited subset of rows can be returned and not all data for the data can be displayed
    const PAGE_SIZE = 1000; //takes batches of 1000
    let from = 0; //starts from 0

    while (true) {
        const to = from + PAGE_SIZE - 1; //first loop: 0-999, second loop: 1000-1999, etc.

        const {data, error} = await supabase //SUPABASE query
        .from("data")
        .select("time, sensor_type, value")
        .in("sensor_type", LINE_CHART_SENSORS)
        .gte("time", startOfDay.toISOString())
        .lte("time", endOfDay.toISOString())
        .order("time", { ascending: true })
        .range(from, to); // <-- pagination

        if (error) { //if query is unsuccessful, log an error
            console.log(error);
            return;
        }

        if (!data || data.length === 0) break; //if query returns no rows, data is done being loaded

        //each sensor is identified and ends up with an array of {x, y} points
        for (const entry of data) {
            const sensor = String(entry.sensor_type).trim();
            if (!pointsOfSensor.has(sensor)) continue;

            pointsOfSensor.get(sensor).push({
                x: new Date(entry.time),
                y: Number(entry.value)
            });
        }

        // last page
        if (data.length < PAGE_SIZE) break;

        from += PAGE_SIZE; //go to next page or batch of data
    }

    //put points into the chart dataset
    inputLineChart.data.datasets.forEach(dataset => {
        dataset.data = pointsOfSensor.get(dataset.label) ?? [];
    });

    //ensures the chart data is limited to today
    inputLineChart.options.scales.x.min = startOfDay;
    inputLineChart.options.scales.x.max = endOfDay;

    //chart.js re-renders the data
    inputLineChart.update();
}

async function loadStateTimelineToday() { //loads all chart points for today, grouped by sensor, in the line chart
    //computes "today" time window
    const startOfDay = new Date();
    startOfDay.setHours(0, 0, 0, 0);
    const endOfDay = new Date(startOfDay);
    endOfDay.setHours(23, 59, 59, 999);

    const OFFSET = 10; // vertical spacing between sensors
    const sensorIndex = new Map(BAR_CHART_SENSORS.map((s, i) => [s, i]));

    //creates an array/bucket for each sensor's data
    const pointsOfSensor = new Map(BAR_CHART_SENSORS.map(s => [s, []]));

    //requests small batches until all rows of the day are received
    const PAGE_SIZE = 1000; //takes batches of 1000
    let from = 0; //starts from 0

    while (true) {
        const to = from + PAGE_SIZE - 1;

        const {data, error} = await supabase
        .from("data")
        .select("time, sensor_type, value")
        .in("sensor_type", BAR_CHART_SENSORS)
        .gte("time", startOfDay.toISOString())
        .lte("time", endOfDay.toISOString())
        .order("time", { ascending: true })
        .range(from, to);

        if (error) {
            console.log(error);
            return;
        }

        if (!data || data.length === 0) break;

        for (const entry of data) {
            const sensor = String(entry.sensor_type).trim();
            if (!pointsOfSensor.has(sensor)) continue;

            const idx = sensorIndex.get(sensor) ?? 0;
            const v = Number(entry.value);

            pointsOfSensor.get(sensor).push({
                x: new Date(entry.time),
                y: v + idx * OFFSET
            });
        }

        if (data.length < PAGE_SIZE) break;
        from += PAGE_SIZE;
    }

    //put points into the chart dataset
    stateTimelineChart.data.datasets.forEach(dataset => {
        dataset.data = pointsOfSensor.get(dataset.label) ?? [];
    });

    stateTimelineChart.options.scales.x.min = startOfDay;
    stateTimelineChart.options.scales.x.max = endOfDay;

    stateTimelineChart.update();
}



async function boot() {
    createLineChart();
    createStateTimelineChart();
    await loadInitialData();
    await loadChartDataToday();  // backfill chart FIRST
    await loadStateTimelineToday();

    /*
    setInterval(async () => {
        await loadInitialData();
        await loadChartDataToday();
        await loadStateTimelineToday();
    }, 10000);  
    */
}

// UI events
sensorFilterEl.addEventListener("change", () => renderTable());

rowLimitEl.addEventListener("change", async () => {
    await loadInitialData(); // reload with new limit
});

reloadBtn.addEventListener("click", async () => {
    await loadInitialData();
    await loadChartDataToday();
    await loadStateTimelineToday();
});


// Start
boot();

