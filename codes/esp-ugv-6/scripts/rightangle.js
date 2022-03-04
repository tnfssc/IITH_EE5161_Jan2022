const fs = require("fs");

const data = fs
  .readFileSync("left1.txt", "utf-8")
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

const angles = [
  {
    time: data[0].time,
    angleZ: 0,
  },
];

for (let i = 1; i < data.length - 1; i++) {
  const timetaken = data[i].time - angles[angles.length - 1].time;
  const deltaAngleZ = data[i].gyZ * timetaken;
  angles.push({
    time: data[i].time,
    angleZ: angles[angles.length - 1].angleZ + deltaAngleZ,
  });
}

module.exports = Math.abs(angles[angles.length - 1].angleZ);
console.log(`Right angle value: ${module.exports}`);
