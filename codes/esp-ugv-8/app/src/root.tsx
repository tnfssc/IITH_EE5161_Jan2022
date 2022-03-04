import { AppShell, Center, Header, MantineProvider } from "@mantine/core";

import App from "./App";

export default function Root() {
  return (
    <MantineProvider theme={{ colorScheme: "dark" }} withGlobalStyles withNormalizeCSS>
      <AppShell
        padding="md"
        header={
          <Header height={60} padding="xs">
            <Center style={{ height: "100%", fontWeight: "bold" }}>
              UGV Bluetooth controller
            </Center>
          </Header>
        }
        styles={{ main: { height: "calc(100vh - 60px)" } }}
      >
        <Center style={{ height: "100%" }}>
          <App />
        </Center>
      </AppShell>
    </MantineProvider>
  );
}
