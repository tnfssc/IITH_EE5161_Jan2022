import { useState } from "react";

const SERVICE_UUID = "1fafc201-1fb5-459e-8fcc-c5c9c331914b";
const CHARACTERISTIC_UUID = "aeb5483e-36e1-4688-b7f5-ea07361b26a8";

export default function App() {
  const [data, setData] = useState("Hello");
  const [characteristic, setCharacteristic] =
    useState<BluetoothRemoteGATTCharacteristic>();
  const handleClick = async () => {
    if (!navigator.bluetooth) setData("Bluetooth is not supported");
    else {
      const device = await navigator.bluetooth.requestDevice({
        acceptAllDevices: true,
        optionalServices: [SERVICE_UUID],
      });
      if (!device.gatt) {
        setData("Bluetooth device does not support GATT");
        return;
      }
      const server = await device.gatt.connect();
      const service = await server.getPrimaryService(SERVICE_UUID);
      const _characteristic = await service.getCharacteristic(CHARACTERISTIC_UUID);
      setCharacteristic(_characteristic);
    }
  };
  const handleClick2 = async () => {
    if (!characteristic) return;
    await characteristic.writeValue(new TextEncoder().encode("forward"));
  };
  const handleClick3 = async () => {
    if (!characteristic) return;
    const value = await characteristic.readValue();
    setData(new TextDecoder().decode(value));
  };
  return (
    <>
      <button onClick={handleClick}>Do</button>
      <button onClick={handleClick2}>Do2</button>
      <button onClick={handleClick3}>Do3</button>

      <pre>{data}</pre>
    </>
  );
}
