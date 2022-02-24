var PORT = 8989;
var HOST = "0.0.0.0";

var dgram = require("dgram");
var fs = require("fs");
var server = dgram.createSocket("udp4");

server.on("listening", function () {
  var address = server.address();
  console.log("UDP Server listening on " + address.address + ":" + address.port);
});

server.on("message", function (message, remote) {
  console.log(remote.address + ":" + remote.port + " - " + message);
  fs.appendFileSync("data.txt", message);
});

server.bind(PORT, HOST);
