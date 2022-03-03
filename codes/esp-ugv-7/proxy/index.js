const fs = require("fs");
const express = require("express");
const cors = require("cors");
const https = require("https");
const client = require("dgram").createSocket("udp4"); // We create a UDP socket to send data to the ESP
require("dotenv").config();

// Express is the goto web server framework for node.js
const app = express();

// Enable cross-origin resource sharing because our web app is on a different domain
app.use(cors());

app.get("/", (req, res) => {
  // Change the deployment URL here
  res.redirect("https://ugv-controller.vercel.app/");
});

app.post("/forward", (req, res) => {
  // Send the forward command to the ESP
  client.send(Buffer.from("forward"), 0, 4, process.env.ESP_PORT, process.env.ESP_IP, err =>
    err ? res.status(500).send(err) : res.status(200).send("ok")
  );
});

app.post("/backward", (req, res) => {
  client.send(Buffer.from("backward"), 0, 4, process.env.ESP_PORT, process.env.ESP_IP, err =>
    err ? res.status(500).send(err) : res.status(200).send("ok")
  );
});

app.post("/right", (req, res) => {
  client.send(Buffer.from("right"), 0, 4, process.env.ESP_PORT, process.env.ESP_IP, err =>
    err ? res.status(500).send(err) : res.status(200).send("ok")
  );
});

app.post("/left", (req, res) => {
  client.send(Buffer.from("left"), 0, 4, process.env.ESP_PORT, process.env.ESP_IP, err =>
    err ? res.status(500).send(err) : res.status(200).send("ok")
  );
});

// Since HTTPS sites can't send requests to HTTP sites, we make a HTTPS server
const httpsServer = https.createServer(
  { key: fs.readFileSync("./key.pem", "utf8"), cert: fs.readFileSync("./cert.pem", "utf8"), passphrase: "123456" },
  app
);
httpsServer.listen(8443, "0.0.0.0", () => {
  console.log("HTTPS Server running on port 8443");
});
