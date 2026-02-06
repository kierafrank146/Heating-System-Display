# Home Heating System Dashboard

A web-based dashboard designed to remotely visualize operational data from a home heating system. The application presents recent sensor readings in a table and displays the current day’s data in a time-series line chart.

## Project Overview

This project was developed to make heating system data accessible from anywhere through a browser-based interface. Sensor readings are collected locally, stored, and then synchronized to the cloud for remote visualization.

## Data Collection

An Arduino is used to serially transmit sensor data to a local computer.

A local SQLite database stores this data on the machine’s hard drive, acting as the primary data storage device.
Each data point is stored with the following information:
- Sensor identifier
- Timestamp
- Measured value

To enable remote access, the locally collected data is uploaded to a Supabase-hosted PostgreSQL database. The Supabase schema and queries were implemented using SQL.

## Web Application

The frontend is built using HTML, CSS, and JavaScript and provides a read-only interface to visualize the data.

The UI components include:
- A table displaying up to the 1,000 most recent sensor readings
- A line chart visualizing all sensor data collected during the current day across a 24-hour period

All data visualization is handled in the browser after fetching raw data from Supabase.

## Tech Stack

- Arduino (sensor data transmission)
- SQLite (local data storage)
- Python (serial data handling, SQLite interaction, and cloud synchronization)
- Supabase (cloud database and access layer)
- HTML, CSS, JavaScript (frontend)
- Vercel (deployment)

## Security

The Supabase database is protected using Row Level Security (RLS). The website is restricted to read-only access, preventing updates or deletes from others.
