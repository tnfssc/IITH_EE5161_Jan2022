import { useState } from "react";

export default function App() {
  const [data, setData] = useState("Hello");
  const handleClick = () => {
    if (!navigator.bluetooth) setData("Bluetooth is not supported");
    else
      navigator.bluetooth
        .requestDevice({
          acceptAllDevices: true,
          optionalServices: ["battery_service"], // Required to access service later.
        })
        .then((device) => {
          setData(JSON.stringify(device, null, 2));
        })
        .catch((error) => {
          console.error(error);
        });
    // navigator.bluetooth
    //   .getDevices()
    //   .then((devices) => setData(JSON.stringify(devices, null, 2)));
  };
  return (
    <>
      <button onClick={handleClick}>Do</button>
      <pre>{data}</pre>
    </>
  );
}
