const fs = require("fs");
const { ChartJSNodeCanvas } = require("chartjs-node-canvas");

/**@type {Array<{ time: number, angleZ: number }>} */
const positions = require("./angles");

const data = positions.map(({ angleZ, time }) => ({ x: time, y: angleZ }));

const width = 1000; //px
const height = 1000; //px
const backgroundColour = "white"; // Uses https://www.w3schools.com/tags/canvas_fillstyle.asp
const chartJSNodeCanvas = new ChartJSNodeCanvas({ width, height, backgroundColour });

(async () => {
  const image = await chartJSNodeCanvas.renderToBuffer({
    data: { datasets: [{ data, showLine: true }] },
    type: "scatter",
    options: {
      responsive: true,
      color: "white",
      borderColor: "black",
    },
  });
  fs.writeFileSync("./chart.png", image, "base64");
})();
