var ttn = require("ttn")

var appID = "foo";
var accessKey = "ttn-account-v2.eiPq8mEeYRL_PNBZsOpPy-O3ABJXYWulODmQGR5PZzg";

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