const fs = require("fs");

const lines = fs
  .readFileSync("demoCircular.txt", "utf-8")
  .split(/\r?\n/)
  .map(line => {
    const [time, acc, gyro] = line.split("\t").map(prop => prop.split(": ")[1]);
    const [accX, accY, accZ] = acc.split(", ");
    const [gyX, gyY, gyZ] = gyro.split(", ");
    return {
      time: (parseInt(time) / 1000000).toPrecision(6),
      accX: parseFloat(accX).toFixed(3),
      accY: parseFloat(accY).toFixed(3),
      accZ: parseFloat(accZ).toFixed(3),
      gyX: parseFloat(gyX).toFixed(3),
      gyY: parseFloat(gyY).toFixed(3),
      gyZ: parseFloat(gyZ).toFixed(3),
      // gyZ: 0,
    };
  });
// .filter(line => Math.abs(line.accX) < 0.5 && Math.abs(line.accY) < 0.5 && Math.abs(line.gyZ) < 0.5);

module.exports = lines;
