import { Anchor, Button, Center, SimpleGrid } from "@mantine/core";
import { useEffect, useRef, useState } from "react";

import { CHARACTERISTIC_UUID, SERVICE_UUID } from "./constants";
import { Direction, move } from "./move";

export default function App() {
  const [characteristic, setCharacteristic] =
    useState<BluetoothRemoteGATTCharacteristic>();
  const [device, setDevice] = useState<BluetoothDevice>();

  const isGattBeingUsed = useRef(false);
  const intervalRef = useRef<number>(0);

  const startMoving = (direction: Direction) => () => {
    if (intervalRef.current) return;
    intervalRef.current = setInterval(() => {
      sendMovement(direction)();
    }, 1);
  };
  const stopMoving = () => {
    if (intervalRef.current) {
      clearInterval(intervalRef.current);
      intervalRef.current = 0;
    }
  };

  useEffect(() => {
    return () => stopMoving();
  }, []);

  const sendMovement = (direction: Direction) => async () => {
    if (!characteristic) return alert("ERROR! No characteristic");
    if (isGattBeingUsed.current) return;
    isGattBeingUsed.current = true;
    await move(characteristic, direction);
    isGattBeingUsed.current = false;
  };

  const handleConnect = async () => {
    if (!navigator.bluetooth) alert("ERROR! No bluetooth");
    else {
      try {
        const _device = await navigator.bluetooth.requestDevice({
          acceptAllDevices: true,
          optionalServices: [SERVICE_UUID],
        });
        setDevice(_device);
        if (!_device.gatt) return alert("ERROR! No GATT");
        const server = await _device.gatt.connect();
        const service = await server.getPrimaryService(SERVICE_UUID);
        const _characteristic = await service.getCharacteristic(CHARACTERISTIC_UUID);
        setCharacteristic(_characteristic);
      } catch (error) {
        alert("ERROR! " + error);
        console.error(error);
      }
    }
  };
  return (
    <div>
      {!navigator.bluetooth && (
        <div>
          <Center color="red">
            Bluetooth is not enabled. Enable it using the following flag
          </Center>
          <Center color="red" mb="xl">
            <Anchor
              href="chrome://flags/#enable-experimental-web-platform-features"
              target="_blank"
              rel="noreferrer noopener"
            >
              chrome://flags/#enable-experimental-web-platform-features
            </Anchor>
          </Center>
        </div>
      )}
      <Center mb="xl">
        <Button onClick={handleConnect} disabled={!!device}>
          {device ? `Connected to ${device.name}` : "Connect Bluetooth"}
        </Button>
      </Center>
      <SimpleGrid cols={3}>
        <div></div>
        <Button
          onMouseDown={startMoving(Direction.forward)}
          onMouseUp={stopMoving}
          onMouseLeave={stopMoving}
          onTouchStart={startMoving(Direction.forward)}
          onTouchEnd={stopMoving}
          onTouchCancel={stopMoving}
          disabled={!characteristic}
          color="gray"
          size="xl"
        >
          Forward
        </Button>
        <div></div>
        <Button
          onMouseDown={startMoving(Direction.left)}
          onMouseUp={stopMoving}
          onMouseLeave={stopMoving}
          onTouchStart={startMoving(Direction.left)}
          onTouchEnd={stopMoving}
          onTouchCancel={stopMoving}
          disabled={!characteristic}
          color="gray"
          size="xl"
        >
          Left
        </Button>
        <Button
          onMouseDown={startMoving(Direction.backward)}
          onMouseUp={stopMoving}
          onMouseLeave={stopMoving}
          onTouchStart={startMoving(Direction.backward)}
          onTouchEnd={stopMoving}
          onTouchCancel={stopMoving}
          disabled={!characteristic}
          color="gray"
          size="xl"
        >
          Backward
        </Button>
        <Button
          onMouseDown={startMoving(Direction.right)}
          onMouseUp={stopMoving}
          onMouseLeave={stopMoving}
          onTouchStart={startMoving(Direction.right)}
          onTouchEnd={stopMoving}
          onTouchCancel={stopMoving}
          disabled={!characteristic}
          color="gray"
          size="xl"
        >
          Right
        </Button>
      </SimpleGrid>
    </div>
  );
}
