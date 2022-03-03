const fs = require("fs");
const express = require("express");
const https = require("https");
const client = require("dgram").createSocket("udp4");
require("dotenv").config();

const app = express();

app.get("/", (req, res) => {
  res.redirect("https://ugv-controller.vercel.app/");
});

app.post("/forward", (req, res) => {
  client.send(Buffer.from("forward"), 0, 5, process.env.PORT, process.env.HOST, err =>
    err ? res.status(500).send(err) : res.status(200).send("ok")
  );
});

app.post("/backward", (req, res) => {
  client.send(Buffer.from("backward"), 0, 5, process.env.PORT, process.env.HOST, err =>
    err ? res.status(500).send(err) : res.status(200).send("ok")
  );
});

app.post("/right", (req, res) => {
  client.send(Buffer.from("right"), 0, 5, process.env.PORT, process.env.HOST, err =>
    err ? res.status(500).send(err) : res.status(200).send("ok")
  );
});

app.post("/left", (req, res) => {
  client.send(Buffer.from("left"), 0, 5, process.env.PORT, process.env.HOST, err =>
    err ? res.status(500).send(err) : res.status(200).send("ok")
  );
});

const httpsServer = https.createServer(
  { key: fs.readFileSync("./key.pem", "utf8"), cert: fs.readFileSync("./cert.pem", "utf8"), passphrase: "123456" },
  app
);
httpsServer.listen(8443);
