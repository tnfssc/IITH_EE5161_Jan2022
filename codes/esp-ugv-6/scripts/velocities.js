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
/**@type {Array<{ time: number, angleZ: number }>} */
const angles = require("./angles");
const rightAngle = require("./rightangle");
const velocities = [
  {
    time: data[0].time,
    velX: 0,
    velY: 0,
  },
];

for (let i = 1; i < data.length; i++) {
  const timetaken = data[i].time - velocities[velocities.length - 1].time;
  const accX =
    data[i - 1].accX * Math.cos((Math.PI / 2) * (angles[i].angleZ / rightAngle)) +
    data[i - 1].accY * Math.sin((Math.PI / 2) * (angles[i].angleZ / rightAngle));
  const accY =
    data[i - 1].accY * Math.cos((Math.PI / 2) * (angles[i].angleZ / rightAngle)) -
    data[i - 1].accX * Math.sin((Math.PI / 2) * (angles[i].angleZ / rightAngle));
  // const accX = data[i].accX;
  // const accY = data[i].accY;
  const deltaVelX = accX * timetaken;
  const deltaVelY = accY * timetaken;
  velocities.push({
    time: data[i].time,
    velX: velocities[velocities.length - 1].velX + deltaVelX,
    velY: velocities[velocities.length - 1].velY + deltaVelY,
  });
}

module.exports = velocities;
