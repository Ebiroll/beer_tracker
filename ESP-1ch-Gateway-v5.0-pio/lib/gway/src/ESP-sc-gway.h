#ifndef ESP_SC_GWAY_H
#define ESP_SC_GWAY_H

// For Heltec ESP32 based WIFI_LoRa_32 boards (and may-be others as well)
// REMOVE for ESP8266 builds
#define ESP32BUILD  1

#ifdef ESP32BUILD
#define VERSION "V.5.0.2.H+; 171128a nOLED, 15/10"
#else
#define VERSION "V.5.0.2.H; 171118a nOLED, 15/10"
#endif

// This value of DEBUG determines whether some parts of code get compiled.
// Also this is the initial value of debug parameter.
// The value can be changed using the admin webserver
// For operational use, set initial DEBUG vaulue 0
#define DEBUG 1

// Debug message will be put on Serial is this one is set.
// If set to 0, not USB Serial prints are done
// Set to 1 it will prin all user level messages (with correct debug set)
// If set to 2 it will also print interrupt messages (not recommended)
#define DUSB 1

// The spreading factor is the most important parameter to set for a single channel
// gateway. It specifies the speed/datarate in which the gateway and node communicate.
// As the name says, in principle the single channel gateway listens to one channel/frequency
// and to one spreading factor only.
// This parameters contains the default value of SF, the actual version can be set with
// the webserver and it will be stored in SPIFF
// NOTE: The frequency is set in the loraModem.h file and is default 868.100000 MHz.
#define _SPREADING SF7

#define TX_BUFF_SIZE  1024						// Upstream buffer to send to MQTT
#define RX_BUFF_SIZE  1024						// Downstream received from MQTT
#define STATUS_SIZE	  512						// Should(!) be enough based on the static text .. was 1024

// Definitions for the admin webserver.
// A_SERVER determines whether or not the admin webpage is included in the sketch.
// Normally, leave it in!
#define A_SERVER 1        // Define local WebServer only if this define is set
#define A_REFRESH 1				// Will the webserver refresh or not?
#define A_SERVERPORT 80			// local webserver port
#define A_MAXBUFSIZE 192		// Must be larger than 128, but small enough to work

#if A_SERVER==1
	#ifdef ESP32BUILD
		#include <WiFiClient.h>
		#include <WebServer.h>
		#include <ESPmDNS.h>
	#else
		#include <Streaming.h>          				// http://arduiniana.org/libraries/streaming/
	#endif
#endif


// Definitions for over the air updates. At the moment we support OTA with IDE
// Make sure that tou have installed Python version 2.7 and have Bonjour in your network.
// Bonjour is included in iTunes (which is free) and OTA is recommended to install
// the firmware on your router witout having to be really close to the gateway and
// connect with USB.
//
#ifdef ESP32BUILD
// Not available (yet) for ESP32
#define A_OTA 0
#else
#define A_OTA 0
#endif

// Single channel gateways if they behave strict should only use one frequency
// channel and one spreading factor. However, the TTN backend replies on RX2
// timeslot for spreading factors SF9-SF12.
// Also, the server will respond with SF12 in the RX2 timeslot.
// If the 1ch gateway is working in and for nodes that ONLY transmit and receive on the set
// and agreed frequency and spreading factor. make sure to set STRICT to 1.
// In this case, the frequency and spreading factor for downlink messages is adapted by this
// gateway
// NOTE: If your node has only one frequency enabled and one SF, you must set this to 1
//		in order to receive downlink messages
// NOTE: In all other cases, value 0 works for most gateways with CAD enabled
#define _STRICT_1CH	0

// Allows configuration through WifiManager AP setup. Must be 0 or 1
#ifdef ESP32BUILD
// Not available (yet) for ESP32
#define WIFIMANAGER 0
#else
#define WIFIMANAGER 0
#endif

// Define the name of the accesspoint if the gateway is in accesspoint mode (is
// getting WiFi SSID and password using WiFiManager)
#ifdef ESP32BUILD
#define AP_NAME "ESP32-Gway-Things4U"
#else
#define AP_NAME "ESP8266-Gway-Things4U"
#endif
#define AP_PASSWD "MyPw01!"


// Defines whether the gateway will also report sensor/status value on MQTT
// after all, a gateway can be a node to the system as well
// Set its LoRa address and key below in this file
// See spec. para 4.3.2
#define GATEWAYNODE 0
#define _CHECK_MIC 0

// This section defines whether we use the gateway as a repeater
// For his, we use another output channle as the channel (default==0) we are
// receiving the messages on.
#define REPEATER 0

// Will we use Mutex or not?
// +SPI is input for SPI, SPO is output for SPI
#define MUTEX 1
#define MUTEX_SPI 0
#define MUTEX_SPO 0
// Protect the interrupt module
#define MUTEX_INT 1

// Define whether we want to manage the gateway over UDP (next to management
// thru webinterface).
// This will allow us to send messages over the UDP connection to manage the gateway
// and its parameters. Sometimes the gateway is not accesible from remote,
// in this case we would allow it to use the SERVER UDP connection to receive
// messages as well.
// NOTE: Be aware that these messages are NOT LoRa and NOT LoRa Gateway spec compliant.
//	However that should not interfere with regular gateway operation but instead offer
//	functions to set/reset certain parameters from remote.
#define GATEWAYMGT 0

// Name of he configfile in SPIFFs	filesystem
// In this file we store the configuration and other relevant info that should
// survive a reboot of the gateway
#define CONFIGFILE "/gwayConfig.txt"

// Set the Server Settings (IMPORTANT)
#define _LOCUDPPORT 1700					// UDP port of gateway! Often 1700 or 1701 is used for upstream comms

// Timing
#define _MSG_INTERVAL 15
#define _PULL_INTERVAL 55					// PULL_DATA messages to server to get downstream in milliseconds
#define _STAT_INTERVAL 120					// Send a 'stat' message to server
#define _NTP_INTERVAL 3600					// How often do we want time NTP synchronization
#define _WWW_INTERVAL	60					// Number of seconds before we refresh the WWW page

// MQTT definitions, these settings should be standard for TTN
// and need not changing
#define _TTNPORT 1700						// Standard port for TTN
#define _TTNSERVER "router.eu.thethings.network"

// If you have a second back-end server defined such as Semtech or loriot.io
// your can define _THINGPORT and _THINGSERVER with your own value.
// If not, make sure that you do not defined these, which will save CPU time
// Port is UDP port in this program
//
// Default for testing: Switch off
//#define _THINGPORT 1700					// dash.westenberg.org:8057
//#define _THINGSERVER "yourServer.com"		// Server URL of the LoRa-udp.js handler

// Gateway Ident definitions
#define _DESCRIPTION "ESP32 LoRaWaN Gateway by Pele"
#define _EMAIL "pele@balorda.com"
#define _PLATFORM "ESP32"
#define _LAT 43.86477759
#define _LON 18.40934990
#define _ALT 525

// ntp
#define NTP_TIMESERVER "pool.ntp.org"	// Country and region specific
#define NTP_TIMEZONES	1					// How far is our Timezone from UTC (excl daylight saving/summer time)
//#define SECS_PER_HOUR	3600
#define NTP_INTR 0							// Do NTP processing with interrupts or in loop();

#if GATEWAYNODE==1
#define _DEVADDR { 0x26, 0x00, 0x00 0x00 }
#define _APPSKEY { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
#define _NWKSKEY { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
#define _SENSOR_INTERVAL 300
#endif

// Define the correct radio type that you are using
#define CFG_sx1276_radio
//#define CFG_sx1272_radio

// Serial Port speed
#define _BAUDRATE 115200					// Works for debug messages to serial momitor

// if OLED Display is connected to i2c
#define OLED 1								// Make define 1 on line if you have an OLED display connected
#define OLED_SCL 22							// GPIO5 / D1
#define OLED_SDA 21							// GPIO4 / D2
#define OLED_ADDR 0x3C						// Default 0x3C for 0.9", for 1.3" it is 0x78

// Wifi definitions
// WPA is an array with SSID and password records. Set WPA size to number of entries in array
// When using the WiFiManager, we will overwrite the first entry with the
// accesspoint we last connected to with WifiManager
// NOTE: Structure needs at least one (empty) entry.
//		So WPASIZE must be >= 1
struct wpas {
	char login[32];							// Maximum Buffer Size (and allocated memory)
	char passw[64];
};

// For asserting and testing the following defines are used.
//
#if !defined(CFG_noassert)
#define ASSERT(cond) if(!(cond)) gway_failed(__FILE__, __LINE__)
#else
#define ASSERT(cond) /**/
#endif



#ifdef __cplusplus
extern "C" {
#endif

#include <gwayGlobals.h>

void die(const char *s);
void gway_failed(const char *file, uint16_t line);
int sendUdp(IPAddress server, int port, uint8_t *msg, int length);
void ftoa(float f, char *val, int p);

#ifdef __cplusplus
}
#endif

#endif //ESP_SC_GWAY_H
