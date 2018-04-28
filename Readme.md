The purpose of this project is to track location of lora equiped beer drinkers

# How this works,

You must set up a GPS enabled Lora-wan device that sends data to the things network.

Data is collected and stored on the things network, then the webpage on firebase will use the
appkey to retreive the data.

https://www.thethingsnetwork.org/docs/applications/storage/api.html



## Setting up your application in the ttn console

#  PayloadFormats tab.

To get the data collected by the things network you must enter the decoder,js in the things console.


# Integrations tab

You must also enable Data Storage in the things network console.
When you do this you get an interface for fetching data direcly from the things network.

https://esp32_heltec.data.thethingsnetwork.org/


The application on firebase uses these url to fetch the data.

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
 ```

To deploy
 ```
npm -g install firebase-tools

firebase login
firebase deploy
 ```
