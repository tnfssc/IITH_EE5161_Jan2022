const fs = require("fs");
const { ChartJSNodeCanvas } = require("chartjs-node-canvas");

/**@type {Array<{
    time: number;
    accX: number;
    accY: number;
    accZ: number;
    gyX: number;
    gyY: number;
    gyZ: number;
}>} */
const lines = require("./lines");
const dataAccX = lines.map(({ accX, accY, gyZ, time }) => ({ x: time, y: accX }));
const dataAccY = lines.map(({ accX, accY, gyZ, time }) => ({ x: time, y: accY }));
const dataGyZ = lines.map(({ accX, accY, gyZ, time }) => ({ x: time, y: gyZ }));

const width = 1000; //px
const height = 1000; //px
const backgroundColour = "white"; // Uses https://www.w3schools.com/tags/canvas_fillstyle.asp
const chartJSNodeCanvas = new ChartJSNodeCanvas({ width, height, backgroundColour });

(async () => {
  const image = await chartJSNodeCanvas.renderToBuffer({
    data: {
      datasets: [
        { data: dataAccX, label: "AccX", borderColor: "red", fill: false },
        { data: dataAccY, label: "AccY", borderColor: "blue", fill: false },
        { data: dataGyZ, label: "GyZ", borderColor: "black", fill: false, showLine: true },
      ],
    },
    type: "scatter",
    options: {
      responsive: true,
      color: "white",
      borderColor: "black",
    },
  });
  fs.writeFileSync("./chart.png", image, "base64");
})();
