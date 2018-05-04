mqtt-ttn2firebase
===========

This NodeJS application listens to MQTT messages and records them to a firebase firestore database





Check this link
https://www.thethingsnetwork.org/labs/story/save-your-data-using-nodejs-mqtt-mongodb



Example
=======

Clone the repository
```bash
$ git clone https://github.com/Ebiroll/beer_tracker
$ cd beer_tracker/mqtt-ttn2firebase
$ npm install
```

Start up the server by editing the config.js first to suit your needs
```bash
$ $EDITOR config.js
$ node server.js
```

Or by using environment variables
```bash
$ MQTT_HOSTNAME="192.168.0.1" node server.js
```

Publish some MQTT messages to try it out (I use mosquitto server for this, but whatever MQTT server should work)
```bash
$ mosquitto_pub -m "bar" -t "foo"
```



