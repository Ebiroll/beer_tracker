var ttn = require("ttn")

const admin = require('firebase-admin');
const serviceAccount = require("./service-key.json");

admin.initializeApp({
  credential: admin.credential.cert(serviceAccount),
  databaseURL: "https://beer-tracker-ecaa3.firebaseio.com"
});


var appID = "esp32_heltec";

var accessKey = require("./ttn-accsess-key.json");
// Put key in file like this
//"ttn-account-v2.XXXXXXXXXXXXXXXXXXXXXXXXXXX"


console.log(accessKey);


var i=0;

ttn.data(appID, accessKey)
  .then(function (client) {
    client.on("uplink", function (devID, payload) {
      console.log("Received uplink from ", devID)
      console.log(payload);
      if (payload & payload.payload_raw) {
        delete payload[payload_raw]; 
      }

      var obj = JSON.parse(payload);
      var docTitle= "data" + i;
      i++;

     admin.firestore()
     .collection("ttn")
     .doc(docTitle)
     .set(obj)
     .then((res) => {
         console.log("Document successfully written!");
     })
     .catch((error) => {
         console.error("Error writing document: ", error);
     });

    })
  })
  .catch(function (error) {
    console.error("Error", error)
    process.exit(1)
  })
