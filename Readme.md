The purpose of this project is to track location of lora equiped beer drinkers

# How this works,

You must set up a GPS enabled Lora-wan device that sends data to the things network.

Data is collected and stored on the things network,

The collected data will be displayed here.

https://beer-tracker-ecaa3.firebaseapp.com/#14/59.3123/18.0678


The data from ttn is moved from ttn to firebase here,
cd mqtt-ttn2firebase
npm install
You have to use your accounts on ttn & firebase to get,
    ttn-accsess-key.json
    service-key.json

    node ttn2fb.js

# Pitfalls during development

When I first saw that the things network had a Data storage Integrations possibility I thought that the
webpage displaying the map could pull the data directly from the things network a similar way that map tiles are
pulled from google.

## Pitfall #1 CORS
Unfortunately I didnt know enough about CORS, Cross-Origin Resource Sharing (CORS) Cross-Origin Resource Sharing (CORS)
This prevented me with my poor javascript skills to pull the data from ttn.

So I thought a simple proxy in firebase would do the job. 

## Pitfal #2 Firebase functions
This would have solved my problem.
https://firebase.google.com/docs/functions/
I implemented an untested solution in the functions folder. Unfortunately, as it turned out, It would only work with google infrastructure unless I upgrade my account.

## Pitfal #3 resin.io node.js
This is where I am currently at. I will try to implement a node.js script running in resin.io that pulls the data from  ttn Data Store and puts it into the firebase database that I can use.
The reason for this solution is because I already have 2 resin.io devices that are constantly running.
https://hackernoon.com/filling-cloud-firestore-with-data-3f67d26bd66e


## Pitfal #4
uncaught exception: [DEFAULT]: Firebase: No Firebase App '[DEFAULT]' has been created - call Firebase App.initializeApp() (app/no-app)
Bah!
I changed var firebase=require("firebase"); to  require("firebase");
Now it worked better...

## Pitfal #5
I used arch arm for raspberry and it turns out 
Node js 10 does not like Buffer objects.. :-
    at isInsideNodeModules (internal/util.js:360:28)
    at showFlaggedDeprecation (buffer.js:149:8)
    at new Buffer (buffer.js:174:3)
    at Array.<anonymous> (/home/olof/work/beer_tracker/mqtt-ttn2firebase/node_modules/source-map-support/source-map-support.js:149:21)
    at /home/olof/work/beer_tracker/mqtt-ttn2firebase/node_modules/source-map-support/source-map-support.js:53:24
    at mapSourcePosition (/home/olof/work/beer_tracker/mqtt-ttn2firebase/node_modules/source-map-support/source-map-support.js:171:21)
    at wrapCallSite (/home/olof/work/beer_tracker/mqtt-ttn2firebase/node_modules/source-map-support/source-map-support.js:343:20)
    at /home/olof/work/beer_tracker/mqtt-ttn2firebase/node_modules/source-map-support/source-map-support.js:378:26
    at Array.map (<anonymous>)
    at Function.prepareStackTrace (/home/olof/work/beer_tracker/mqtt-ttn2firebase/node_modules/source-map-support/source-map-support.js:377:24)


https://github.com/nodejs/node/issues/20258
Probably there is not enough time to implement this but it would be nice to have a process running that gathers 
data about the gateways that intercepted a message and move that to firebase.

## Pitfal #6
I used nodejs that came with raspbian. Should have done this.
    https://deb.nodesource.com/setup_8.x | sudo -E bash - sudo 
    apt-get install -y nodejs.


## Pitfal #7
If you make a misstake in your code and read data to quickly,
i.e. like this.
    setInterval(() => {
        if (firebaseAppDefined) {
            var db = firebase.firestore();
            db.collection("latest_pos").get().then(function(querySnapshot) {
            querySnapshot.forEach(function(doc) {
                console.log(doc.id, " => ", doc.data());
            });
            });
        }
    }, 100)
Dont read data every 100 ms
Then your quota will exhausted at once.

# Setting up your application in the ttn console

##  PayloadFormats tab.

To get the data collected by the things network you must enter the decoder,js in the things console.


## Integrations tab

You must also enable Data Storage in the things network console.
When you do this you get an interface for fetching data direcly from the things network.

https://esp32_heltec.data.thethingsnetwork.org/


The first idea was that the web-application on firebase uses these url to fetch the data.
However this did not work out because of Cross-Origin Resource Sharing problems.

Here is the api info.
https://www.thethingsnetwork.org/docs/applications/storage/api.html


# Decoder

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

# Rak file commisioning

You must change this file for the RAK811, Commisioning,h

https://os.mbed.com/teams/Semtech/code/LoRaWAN-NAMote72-Application-Demo/


# Data format
This is how the RAK data format reply from TTN storage looks like

https://esp32_heltec.data.thethingsnetwork.org/api/v2/query

[
  {
    "acceleration": null,
    "alt": 39,
    "device_id": "rak811",
    "hdop": 0,
    "lat": 59.423255289987054,
    "lon": 17.8296850818208,
    "raw": "1INYjK3LACcAAA==",
    "time": "2018-04-28T09:47:17.030977044Z"
  },
  {
    "acceleration": null,
    "alt": 34,
    "device_id": "rak811",
    "hdop": 0,
    "lat": 59.42327674766045,
    "lon": 17.8296850818208,
    "raw": "1INajK3LACIAAA==",
    "time": "2018-04-28T09:58:00.369678207Z"
  },
  {
    "acceleration": "map[aX:25.5 aY:0 aZ:25.5]",
    "alt": null,
    "device_id": "rak811",
    "hdop": null,
    "lat": null,
    "lon": null,
    "raw": "/wAAAP8A",
    "time": "2018-04-28T10:00:28.495532902Z"
  }
]

# Leaflet and ajax
https://github.com/calvinmetcalf/leaflet-ajax/tree/ajax


# Interesting links



https://os.mbed.com/teams/Semtech/code/LoRaWAN-NAMote72-Application-Demo/

https://www.thethingsnetwork.org/forum/t/rak811-tracker-board/11006/234



# Wifi localisation

https://ico.org.uk/media/1560691/wi-fi-location-analytics-guidance.pdf

https://www.thethingsnetwork.org/forum/t/big-esp32-sx127x-topic-part-2/11973/267

https://www.cbsnews.com/news/uk-bars-trash-cans-from-tracking-people-with-wi-fi/


To develop,
 ```
All files to deploy should be placed in public
browserify js/init-db.js -o js/personal.js 
firebase serve --except functions
 ```

To deploy
 ```
npm -g install firebase-tools

firebase login
firebase deploy
firebase deploy --except functions
 ```
