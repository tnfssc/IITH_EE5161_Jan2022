const fs = require("fs");
const express = require("express");
const { createProxyMiddleware } = require("http-proxy-middleware");
const https = require("https");

const app = express();

app.use("/api", createProxyMiddleware({ target: "http://www.example.org", changeOrigin: true }));

const httpsServer = https.createServer(
  { key: fs.readFileSync("./key.pem", "utf8"), cert: fs.readFileSync("./cert.pem", "utf8"), passphrase: "123456" },
  app
);
httpsServer.listen(8443);
