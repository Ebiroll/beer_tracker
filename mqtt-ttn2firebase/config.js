var config = {};

config.debug = process.env.DEBUG || false;

config.mqtt  = {};

config.mqtt.namespace = process.env.MQTT_NAMESPACE || '+/devices/+/up';
config.mqtt.hostname  = process.env.MQTT_HOSTNAME  || 'eu.thethings.network';
config.mqtt.port      = process.env.MQTT_PORT      || 1883;
config.mqtt.user      = process.env.MQTT_USER      || '<appid>';
config.mqtt.password  = process.env.MQTT_PASSWORD  || '<accesskey>';

module.exports = config;
