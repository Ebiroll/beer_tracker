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

function Decoder(bytes, port) {
    // Decode an uplink message from a buffer
    // (array) of bytes to an object of fields.
    var decoded = {};

    switch(port) {
    case 2:
	decoded.lat = ((bytes[0]<<16)>>>0) + ((bytes[1]<<8)>>>0) + bytes[2];
	decoded.lat = (decoded.lat / 16777215.0 * 180) - 90;
	decoded.lon = ((bytes[3]<<16)>>>0) + ((bytes[4]<<8)>>>0) + bytes[5];
	decoded.lon = (decoded.lon / 16777215.0 * 360) - 180;
	var altValue = ((bytes[6]<<8)>>>0) + bytes[7];
	var sign = bytes[6] & (1 << 7);
	if(sign) {
	    decoded.alt = 0xFFFF0000 | altValue;
	} else {
	    decoded.alt = altValue;
	}
	decoded.hdop = bytes[8] / 10.0;
	break;

    case 3:
	var bat = (bytes[0] | bytes[1]<<8 | (bytes[1] & 0x80 ? 0xFFFF<<16 : 0)) * 10.0;

	decoded ={
	    "battery":{
		"bat":bat
	    }
	};
	break;

    case 4:
	var aX = (bytes[0] | bytes[1]<<8 | (bytes[1] & 0x80 ? 0xFFFF<<16 : 0)) / 10.0;
	var aY = (bytes[2] | bytes[3]<<8 | (bytes[3] & 0x80 ? 0xFFFF<<16 : 0)) / 10.0;
	var aZ = (bytes[4] | bytes[5]<<8 | (bytes[5] & 0x80 ? 0xFFFF<<16 : 0)) / 10.0;

	decoded ={
	    "acceleration":{
		"aX":aX,
		"aY":aY,
		"aZ":aZ,
	    }
	};
	break;
    }
    return decoded;
}


ttn.data(appID, accessKey)
  .then(function (client) {
    client.on("uplink", function (devID, payload) {
      console.log("Received uplink from ", devID)
      console.log(payload);
      //if (payload & payload.payload_raw) {
      //  delete payload[payload_raw]; 
      //}

      var obj = {
	    data : payload.payload_fields,
	    meta : payload.metadata
      }

	
      // The things network already decoded our message
      //if (payload & payload.payload_raw) {
      //    obj= Decode(payload.payload_raw,payload.port);
      //}	


      var docTitle= "d" + i;
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


      
      var latest_dev = payload.dev_id;
      if (!latest_dev) {
	  latest_dev="NONE";
      }
      
      
      admin.firestore()
      .collection("latest_pos")
      .doc(latest_dev)
      .set(obj)
      .then((res) => {
          console.log("Latest pos ",latest_dev);
      })
      .catch((error) => {
          console.error("Error writing pos document: ", error);
      });

  })
  .catch(function (error) {
    console.error("Error", error)
      process.exit(1)
  });
});
