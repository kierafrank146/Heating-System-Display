import { createClient } from "https://cdn.jsdelivr.net/npm/@supabase/supabase-js/+esm";

const SUPABASE_URL = "https://temfsuafvybgaagywnxd.supabase.co";
const SUPABASE_ANON_KEY = "sb_publishable_YfRP1S2oOt3yFgX1YqIQ5Q_3U_LleC5";

const supabase = createClient(SUPABASE_URL, SUPABASE_ANON_KEY);

// DOM
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


let inputLineChart;
const inputLineCanvas = document.getElementById("inputLineChart");
const inputLineContext = inputLineCanvas.getContext("2d");



let currentRows = [];
let realtimeChannel = null;

function setStatus(msg) {
  statusEl.textContent = msg;
}

function setLastUpdated() {
  lastUpdatedEl.textContent =
    `Last updated: ${formatLocalTime(new Date())}`;
}

function formatLocalTime(ts) {
  const date = new Date(ts);

  return new Intl.DateTimeFormat(undefined, {
    year: "numeric",
    month: "short",
    day: "2-digit",
    hour: "2-digit",
    minute: "2-digit",
    second: "2-digit",
    timeZoneName: "short"
  }).format(date);
}

function renderTable() {
  const filter = sensorFilterEl.value;
  const visible = filter
    ? currentRows.filter((r) => r.sensor_type === filter)
    : currentRows;

  rowsEl.innerHTML = visible
    .map(
      (r) => `
      <tr>
        <td>${formatLocalTime(r.time)}</td>
        <td>${escapeHtml(r.sensor_type)}</td>
        <td>${Number(r.value).toFixed(3)}</td>
      </tr>
    `
    )
    .join("");
}

function escapeHtml(s) {
  return String(s)
    .replaceAll("&", "&amp;")
    .replaceAll("<", "&lt;")
    .replaceAll(">", "&gt;")
    .replaceAll('"', "&quot;")
    .replaceAll("'", "&#039;");
}

function rowKey(r) {
    return `${r.time}|${r.sensor_type}`;
}


function rebuildSensorFilterOptions() {
    const existing = sensorFilterEl.value;
    const sensors = Array.from(new Set(currentRows.map((r) => r.sensor_type))).sort();

    // Keep "All"
    sensorFilterEl.innerHTML = `<option value="">All</option>` + sensors
        .map((s) => `<option value="${escapeHtml(s)}">${escapeHtml(s)}</option>`)
        .join("");

    // Restore selection if still present
    if (sensors.includes(existing)) sensorFilterEl.value = existing;
    else sensorFilterEl.value = "";
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


async function loadInitialData() {
    try {
        setStatus("Loading...");
        const limit = Number(rowLimitEl.value);

        const { data, error } = await supabase
            .from("data")
            .select("time, sensor_type, value")
            .order("time", { ascending: false })
            .limit(limit);

        if (error) throw error;

        currentRows = data ?? [];
        rebuildSensorFilterOptions();
        renderTable();
        setLastUpdated();
        setStatus("Live (initial load complete)");
    } catch (err) {
        console.error(err);
        setStatus(`Error loading: ${err.message ?? err}`);
    }
}

async function loadChartDataToday() {
    const startOfDay = new Date();
    startOfDay.setHours(0, 0, 0, 0);

    const endOfDay = new Date(startOfDay);
    endOfDay.setHours(23, 59, 59, 999);

    // Clear existing chart data
    const pointsOfSensor = new Map(LINE_CHART_SENSORS.map(s => [s, []]));

    const PAGE_SIZE = 1000;
    let from = 0;

    while (true) {
        const to = from + PAGE_SIZE - 1;

        const { data, error } = await supabase
        .from("data")
        .select("time, sensor_type, value")
        .in("sensor_type", LINE_CHART_SENSORS)
        .gte("time", startOfDay.toISOString())
        .lte("time", endOfDay.toISOString())
        .order("time", { ascending: true })
        .range(from, to); // <-- pagination

        if (error) {
            console.log(error);
            return;
        }

        if (!data || data.length === 0) break;

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

        from += PAGE_SIZE;
    }

    inputLineChart.data.datasets.forEach(ds => {
        ds.data = pointsOfSensor.get(ds.label) ?? [];
    });

    inputLineChart.options.scales.x.min = startOfDay;
    inputLineChart.options.scales.x.max = endOfDay;

    inputLineChart.update();
}



function upsertRow(newRow) {
    const newKey = rowKey(newRow);

    const idx = currentRows.findIndex((r) => rowKey(r) === newKey);
    if (idx >= 0) currentRows[idx] = newRow;
    else currentRows.unshift(newRow);

    // Newest first
    currentRows.sort((a, b) => new Date(b.time) - new Date(a.time));

    // Trim to limit
    const limit = Number(rowLimitEl.value);
    currentRows = currentRows.slice(0, limit);
}


async function startRealtime() {
  // Clean up old channel if any
    if (realtimeChannel) {
        await supabase.removeChannel(realtimeChannel);
        realtimeChannel = null;
    }

    setStatus("Connecting realtime...");

    realtimeChannel = supabase
        .channel("public:data_changes")
        .on(
            "postgres_changes",
            { event: "*", schema: "public", table: "data" },
            (payload) => {
            // payload.eventType: INSERT/UPDATE/DELETE
            if (payload.eventType === "DELETE") {
                const oldRow = payload.old;
                if (oldRow?.time && oldRow?.sensor_type) {
                    const oldKey = rowKey(oldRow);
                    currentRows = currentRows.filter((r) => rowKey(r) !== oldKey);
                }
            } else {
                const row = payload.new;
                if (row) upsertRow(row);
            }

            rebuildSensorFilterOptions();
            renderTable();
            setLastUpdated();
            setStatus("Live");
            }
        )
        .subscribe((state) => {
        // states: SUBSCRIBED, TIMED_OUT, CLOSED, CHANNEL_ERROR
            if (state === "SUBSCRIBED") setStatus("Live (subscribed)");
            else if (state === "CHANNEL_ERROR") setStatus("Realtime error");
            else if (state === "TIMED_OUT") setStatus("Realtime timed out");
            else if (state === "CLOSED") setStatus("Realtime closed");
        });
}

async function boot() {
  createLineChart();
  await loadInitialData();
  await loadChartDataToday();  // backfill chart FIRST
  startRealtime();             // then realtime updates
}


// UI events
sensorFilterEl.addEventListener("change", () => renderTable());

rowLimitEl.addEventListener("change", async () => {
    await loadInitialData(); // reload with new limit
});

reloadBtn.addEventListener("click", async () => {
    await loadInitialData();
    await loadChartDataToday();
});

// Start
boot();

