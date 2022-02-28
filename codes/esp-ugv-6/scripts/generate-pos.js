const fs = require("fs");
const { ChartJSNodeCanvas } = require("chartjs-node-canvas");

/**@type {Array<{ time: number, posX: number, posY: number }>} */
const positions = require("./positions");

const data = positions.map(({ time, posX, posY }) => ({ x: posX, y: posY }));

const width = 1000; //px
const height = 1000; //px
const backgroundColour = "white"; // Uses https://www.w3schools.com/tags/canvas_fillstyle.asp
const chartJSNodeCanvas = new ChartJSNodeCanvas({ width, height, backgroundColour });

const min = Math.min(...data.map(({ x, y }) => Math.min(x, y)));
const max = Math.max(...data.map(({ x, y }) => Math.max(x, y)));

(async () => {
  const image = await chartJSNodeCanvas.renderToBuffer({
    data: { datasets: [{ data }] },
    type: "scatter",
    options: {
      responsive: true,
      color: "white",
      borderColor: "black",
      scales: { x: { type: "linear", min, max }, y: { type: "linear", min, max } },
    },
  });
  fs.writeFileSync("./chart.png", image, "base64");
})();
