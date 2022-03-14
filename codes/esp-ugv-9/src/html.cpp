#include <ESPAsyncWebServer.h>

const char index_html[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1" />
    <title>UGV controller</title>
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <meta name="theme-color" content="#000000" />
    <meta name="description" content="UGV controller" />
    <meta name="author" content="tnfssc" />
  </head>

  <body>
    <div id="root"></div>
    <script src="http://localhost:8000/assets/babel.min.js" crossorigin></script>
    <script src="http://localhost:8000/assets/react.development.js" crossorigin></script>
    <script src="http://localhost:8000/assets/react-dom.development.js" crossorigin></script>
    <script src="http://localhost:8000/assets/material-ui.production.min.js" crossorigin></script>
    <style>
      .square {
        aspect-ratio: 1;
      }
    </style>
    <script type="text/babel">
      const {
        CssBaseline,
        ThemeProvider,
        Typography,
        Container,
        createTheme,
        Box,
        Grid,
        Button,
        ButtonBase,
        TextField,
      } = MaterialUI;
      const GridItem = props => (
        <Grid item xs={4}>
          {props.children ? (
            <ButtonBase
              className="square"
              onMouseDown={props.onHold}
              onMouseUp={props.onRelease}
              onMouseLeave={props.onRelease}
              onTouchStart={props.onHold}
              onTouchEnd={props.onRelease}
              style={{
                display: "flex",
                flexDirection: "column",
                justifyContent: "center",
                alignItems: "center",
                width: "100%",
                border: "solid white 1px",
                borderRadius: "20%",
              }}>
              <Typography>{props.children}</Typography>
            </ButtonBase>
          ) : (
            <React.Fragment></React.Fragment>
          )}
        </Grid>
      );
      const App = () => {
        const [holding, setHolding] = React.useState(false);
        const [address, setAddress] = React.useState(`ws://${window.location.hostname}/ws`);
        const [direction, setDirection] = React.useState("");
        const intervalRef = React.useRef(0);
        /** @type {[WebSocket]} */
        const [ws, setWs] = React.useState(new WebSocket(address));
        const data = React.useRef([]);

        const sendMovement = direction => {
          if (ws.readyState === WebSocket.OPEN) ws.send(direction);
        };

        React.useEffect(() => {
          if (holding)
            intervalRef.current = setInterval(() => {
              sendMovement(direction);
            }, 250);
          else clearInterval(intervalRef.current);
          () => clearInterval(intervalRef.current);
        }, [holding]);

        const resetState = () => {
          setHolding(false);
          setDirection("");
        };

        const go = dir => {
          setHolding(true);
          setDirection(dir);
        };

        const stop = () => {
          resetState();
        };

        const handleConnect = () => {
          if (ws) ws.close();
          const _ws = new WebSocket(address);
          _ws.onmessage = e => data.current.push(e.data);
          setWs(_ws);
        };

        const handleDownloadData = () => {
          const a = document.createElement("a");
          a.style.display = "none";
          a.href = URL.createObjectURL(new Blob(data.current));
          a.download = "data.txt";
          a.click();
        };

        return (
          <Container
            style={{
              display: "flex",
              flexDirection: "column",
              justifyContent: "center",
              alignItems: "center",
              width: "100vw",
              height: "80vh",
            }}>
            <Typography variant="h3" padding={4}>
              UGV Controller
            </Typography>
            <Box sx={{ display: "flex" }}>
              <TextField
                size="small"
                placeholder="made by Sharath"
                label="WebSocket Address"
                value={address}
                helperText="Default is the same as the URL"
                onChange={e => setAddress(e.target.value)}
                style={{ marginBottom: "1rem" }}
              />
              <Button onClick={handleConnect} variant="contained">
                Connect
              </Button>
            </Box>
            <Button onClick={handleDownloadData} sx={{ marginBottom: 8 }}>
              Download data
            </Button>
            <Box>
              <Grid container spacing={2}>
                <GridItem></GridItem>
                <GridItem onRelease={stop} onHold={() => go("forward")}>
                  Forward
                </GridItem>
                <GridItem></GridItem>

                <GridItem onRelease={stop} onHold={() => go("left")}>
                  Left
                </GridItem>
                <GridItem></GridItem>
                <GridItem onRelease={stop} onHold={() => go("right")}>
                  Right
                </GridItem>

                <GridItem></GridItem>
                <GridItem onRelease={stop} onHold={() => go("backward")}>
                  Back
                </GridItem>
                <GridItem></GridItem>
              </Grid>
            </Box>
          </Container>
        );
      };
      ReactDOM.render(
        <ThemeProvider theme={createTheme({ palette: { mode: "dark", background: { default: "#000" } } })}>
          <CssBaseline />
          <App />
        </ThemeProvider>,
        document.getElementById("root")
      );
    </script>
  </body>
</html>
)rawliteral";
