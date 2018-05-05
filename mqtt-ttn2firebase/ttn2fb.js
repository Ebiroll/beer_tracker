var ttn = require("ttn")

const admin = require('firebase-admin');
const serviceAccount = require("./service-key.json");

var appID = "foo";

var accessKey = require("./ttn-accsess-key.json");


ttn.data(appID, accessKey)
  .then(function (client) {
    client.on("uplink", function (devID, payload) {
      console.log("Received uplink from ", devID)
      console.log(payload)
    })
  })
  .catch(function (error) {
    console.error("Error", error)
    process.exit(1)
  })
