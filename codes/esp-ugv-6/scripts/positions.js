/**@type {Array<{ time: number, velX: number, velY: number }>} */
const velocities = require("./velocities");
/**@type {Array<{ time: number, angleZ: number }>} */
const angles = require("./angles");
const rightAngle = require("./rightangle");

const positions = [
  {
    time: velocities[0].time,
    posX: 0,
    posY: 0,
  },
];

for (let i = 1; i < velocities.length; i++) {
  const timetaken = velocities[i].time - positions[positions.length - 1].time;
  const velX =
    velocities[i - 1].velX * Math.cos((Math.PI / 2) * (angles[i].angleZ / rightAngle)) +
    velocities[i - 1].velY * Math.sin((Math.PI / 2) * (angles[i].angleZ / rightAngle));
  const velY =
    velocities[i - 1].velY * Math.cos((Math.PI / 2) * (angles[i].angleZ / rightAngle)) -
    velocities[i - 1].velX * Math.sin((Math.PI / 2) * (angles[i].angleZ / rightAngle));
  const deltaPosX = velX * timetaken;
  const deltaPosY = velY * timetaken;
  // const deltaPosX = velocities[i].velX * timetaken;
  // const deltaPosY = velocities[i].velY * timetaken;
  positions.push({
    time: velocities[i].time,
    posX: positions[positions.length - 1].posX + deltaPosX,
    posY: positions[positions.length - 1].posY + deltaPosY,
  });
}

module.exports = positions;
