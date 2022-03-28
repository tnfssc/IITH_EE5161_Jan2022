# UGV Realtime stats

## Prerequisites

- Assembled UGV
- Firefox Browser
- `python3`

## Setup

- Flash the [code](./) to your UGV.

## Usage

- After flashing the code to the ESP module, it starts acting like a WiFi access point.
- Connect to it. (SSID: `UGV`, Password: `00000000`).
- Run `python3 -m "http.server" 4000` in the [root](./) folder.
- Visit [http://192.168.4.1](http://192.168.4.1) on Firefox.
- Press `Start` button to start charting.
- Use the other buttons as they are indicated.

## How it works

The ESP's WiFi module is being used in AP mode. This allows us to directly connect to the ESP without any middle router. The connection between the UGV and the client PC is done using WebSockets. The local python server was started because of the required static files for the webpage (`chart.min.js` didn't work from the ESP).
