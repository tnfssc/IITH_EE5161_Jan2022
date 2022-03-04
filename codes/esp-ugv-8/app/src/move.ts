export enum Direction {
  forward = "forward",
  backward = "backward",
  left = "left",
  right = "right",
}

export const move = (
  characteristic: BluetoothRemoteGATTCharacteristic,
  direction: Direction,
) => characteristic.writeValue(new TextEncoder().encode(direction));
