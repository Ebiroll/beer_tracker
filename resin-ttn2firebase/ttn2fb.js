const admin = require('/home/olof/work/node_modules/firebase-admin');
const serviceAccount = require("./service-key.json");

const data = require("./data.json");

admin.initializeApp({
    credential: admin.credential.cert(serviceAccount),
    databaseURL: "https://beer-tracker-ecaa3.firebaseio.com"
});

// Lazy hack, 
global.XMLHttpRequest = require("xhr2");

//var jsonData = JSON.parse(JSON.stringify(data));
//console.log(jsonData);

//data && Object.keys(data).forEach(key => {
    //const nestedContent = data[key];
    const nestedContent = {
        acceleration: null,
        alt: 30,
        "device_id": "0x00012345",
        "hdop": 2,
        lat: 59.422096575623556,
        lon: 17.82944904741342,
        raw: "1ILsjK3AAB4UAA==",
       time: "2018-05-01T19:25:53.13286467Z"
    }

    function reqListener () {
        console.log(this.response);
        var data=JSON.parse(this.response);
        var arrayLength = data.length;
        for (var i = 0; i < arrayLength; i++) {
            var docTitle= "data" + i;
            admin.firestore()
                .collection("positions")
                .doc(docTitle)
                .set(data[i])
                .then((res) => {
                    console.log("Document successfully written!");
                })
                .catch((error) => {
                    console.error("Error writing document: ", error);
                });
        }
    }

    var oReq = new XMLHttpRequest();
    oReq.addEventListener("load", reqListener);
    oReq.open("GET", "https://esp32_heltec.data.thethingsnetwork.org/api/v2/query?last=5d");
    //oReq.open("GET", "data.json");
    oReq.setRequestHeader('Authorization', 'key ttn-account-v2.roDff_0RLN8RalPCctiau1q-gLAtb-QhzTy3lO4EtfY');
    oReq.send();


//data && Object.keys(data).forEach(key => {
//    const nestedContent = data[key];
//
//    if (typeof nestedContent === "object") {
//        Object.keys(nestedContent).forEach(docTitle => {
//            admin.firestore()
//                .collection(key)
//                .doc(docTitle)
//                .set(nestedContent[docTitle])
//                .then((res) => {
//                    console.log("Document successfully written!");
//                })
//                .catch((error) => {
//                    console.error("Error writing document: ", error);
//                });
//        });
//    }
//});
