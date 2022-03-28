#include <ESPAsyncWebServer.h>

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>UGV Controller</title>
  </head>
  <body>
    <script>
      let chart = null;
      let raw_data = [];
      let collection_on = false;
      const ws_address = `ws://${window.location.hostname}/ws`;
      const ws = new WebSocket(ws_address);
      ws.onopen = () => console.log("Connected to websocket");
      ws.onmessage = e => collection_on && raw_data.push(e.data);
      const send = msg => ws.readyState === WebSocket.OPEN && ws.send(msg);
      const download_data = () => {
        const a = document.createElement("a");
        a.href = URL.createObjectURL(new Blob(raw_data));
        a.download = "data.json";
        a.click();
      };
      let data_processor = (chart, data) => {
        data.slice(chart.data.datasets[0].data.length).forEach(line => {
          const [_time, _acc, _gyro] = line.split("\t");
          const time = _time.split(": ")[1];
          const [acc_x, acc_y, acc_z] = _acc.split(": ")[1].split(", ");
          const [gyro_x, gyro_y, gyro_z] = _gyro.split(": ")[1].split(", ");
          chart.data.datasets[0].data.push({ y: acc_x, x: time });
          chart.data.datasets[1].data.push({ y: acc_y, x: time });
          chart.data.datasets[2].data.push({ y: gyro_z, x: time });
        });
      };
      const update_chart = () =>
        requestAnimationFrame(() => {
          if (!chart || !data_processor) return requestAnimationFrame(update_chart);
          data_processor(chart, raw_data);
          chart.update();
          requestAnimationFrame(update_chart);
        });
      requestAnimationFrame(update_chart);
    </script>
    <style>
      body {
        background-color: #fff;
        color: #000;
        font-family: "Roboto", sans-serif;
        font-size: 1.5rem;
        margin: 0;
      }

      #root {
        width: 100vw;
        height: 100vh;
        display: flex;
        flex-direction: column;
        justify-content: center;
        align-items: center;
      }

      .grid {
        display: grid;
        max-width: 400px;
        grid-template-columns: 1fr 1fr 1fr;
        grid-template-rows: 1fr 1fr 1fr;
        grid-gap: 20px;
      }

      .grid > div {
        width: 100%;
        height: 100%;
        background: white;
        padding: 0.5em;
        display: flex;
        justify-content: center;
        align-items: center;
      }

      .grid > div > button {
        width: calc(100% - 0.5em);
        height: calc(100% - 0.5em);
        background: white;
        aspect-ratio: 1;
        border: solid black 1px;
        cursor: pointer;
      }
    </style>
    <div id="root">
      <div class="grid">
        <div></div>
        <div>
          <button onclick="send('forward')">Forward</button>
        </div>
        <div></div>
        <div>
          <button onclick="send('left')">Left</button>
        </div>
        <div>
          <button onclick="send('backward')">Backward</button>
        </div>
        <div>
          <button onclick="send('right')">Right</button>
        </div>
        <div>
          <button onclick="collection_on=true">Start</button>
        </div>
        <div>
          <button onclick="download_data()">Download</button>
        </div>
        <div>
          <button onclick="collection_on=false">Stop</button>
        </div>
      </div>
      <div style="position: relative; height:40vh; width:80vw; margin-top: 48px">
        <canvas id="myChart"></canvas>
      </div>
    </div>

    <script src="http://localhost:4000/chart.min.js"></script>
    <script defer>
      const ctx = document.getElementById("myChart").getContext("2d");
      chart = new Chart(ctx, {
        type: "scatter",
        data: {
          datasets: [
            {
              label: "AccX",
              data: [],
              backgroundColor: "red",
            },
            {
              label: "AccY",
              data: [],
              backgroundColor: "blue",
            },
            {
              label: "GyroZ",
              data: [],
              backgroundColor: "green",
            },
          ],
        },
        options: {
          animation: false,
        },
      });
    </script>
  </body>
</html>

)rawliteral";
