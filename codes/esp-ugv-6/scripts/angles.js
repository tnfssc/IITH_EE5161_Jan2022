/**@type {Array<{
    time: number;
    accX: number;
    accY: number;
    accZ: number;
    gyX: number;
    gyY: number;
    gyZ: number;
}>} */
const data = require("./lines");
const angles = [
  {
    time: data[0].time,
    angleZ: 0,
  },
];

for (let i = 1; i < data.length; i++) {
  const timetaken = data[i].time - angles[angles.length - 1].time;
  const deltaAngleZ = data[i - 1].gyZ * timetaken;
  angles.push({
    time: data[i].time,
    angleZ: angles[angles.length - 1].angleZ + deltaAngleZ,
  });
}

module.exports = angles;
