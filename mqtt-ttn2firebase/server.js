/**
 *
 * This NodeJS application listens to MQTT messages and records them to MongoDB
 *
 * @author  Dennis de Greef <github@link0.net>
 * @license MIT
 *
 */
var mqtt     = require('mqtt');
var config   = require('./config');

const admin = require('firebase-admin');
const serviceAccount = require("./service-key.json");


//admin.initializeApp({
//    credential: admin.credential.cert(serviceAccount),
//    databaseURL: "https://beer-tracker-ecaa3.firebaseio.com"
//});


//var mqttUri  = 'mqtt://' + config.mqtt.hostname + ':' + config.mqtt.port;
var mqttUri  = 'mqtt://' + config.mqtt.user + ':' + config.mqtt.password + '@' + config.mqtt.hostname + ':' + config.mqtt.port;

var client   = mqtt.connect(mqttUri);

client.on('connect', function () {
    client.subscribe(config.mqtt.namespace);
});


client.on('message', function (topic, message) {
    var messageObject = {
        topic: topic,
        message: message.toString()
     };

     //admin.firestore()
     //.collection("positions")
     //.doc(docTitle)
     //.set(messageObject)
     //.then((res) => {
     //    console.log("Document successfully written!");
     //})
     //.catch((error) => {
     //    console.error("Error writing document: ", error);
     //});

     console.log("MQTT MESSAGE",messageObject);

});
