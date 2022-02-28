const fs = require("fs");

const lines = fs
  .readFileSync("data.txt", "utf-8")
  .split(/\r?\n/)
  .map(line => {
    const [time, acc, gyro] = line.split("\t").map(prop => prop.split(": ")[1]);
    const [accX, accY, accZ] = acc.split(", ");
    const [gyX, gyY, gyZ] = gyro.split(", ");
    return {
      time: (parseInt(time) / 1000000).toFixed(6),
      accX: parseFloat(accX),
      accY: parseFloat(accY),
      accZ: parseFloat(accZ),
      gyX: parseFloat(gyX),
      gyY: parseFloat(gyY),
      gyZ: parseFloat(gyZ),
    };
  });

module.exports = lines;
