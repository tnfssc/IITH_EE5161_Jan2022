/**@type {Array<{ time: number, velX: number, velY: number }>} */
const velocities = require("./velocities");

const positions = [
  {
    time: velocities[0].time,
    posX: 0,
    posY: 0,
  },
];

for (let i = 1; i < velocities.length - 1; i++) {
  const timetaken = velocities[i].time - positions[positions.length - 1].time;
  const deltaPosX = velocities[i].velX * timetaken;
  const deltaPosY = velocities[i].velY * timetaken;
  positions.push({
    time: velocities[i].time,
    posX: positions[positions.length - 1].posX + deltaPosX,
    posY: positions[positions.length - 1].posY + deltaPosY,
  });
}

module.exports = positions;
