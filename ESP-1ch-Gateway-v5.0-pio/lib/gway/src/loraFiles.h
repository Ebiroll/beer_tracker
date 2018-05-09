#ifndef LORA_FILES_H
#define LORA_FILES_H

#include <loraModem.h>

#ifdef ESP32BUILD
#include <SPIFFS.h>
#endif
#include <FS.h>

struct espGwayConfig {
	uint16_t fcnt;				// =0 as init value	XXX Could be 32 bit in size
	uint16_t boots;				// Number of restarts made by the gateway after reset
	uint16_t resets;			// Number of statistics resets
	uint16_t views;				// Number of sendWebPage() calls
	uint16_t wifis;				// Number of WiFi Setups
	uint16_t reents;			// Number of re-entrant interrupt handler calls
	uint16_t ntpErr;			// Number of UTP requests that failed
	uint16_t ntps;

	uint32_t ntpErrTime;		// Record the time of the last NTP error
	uint8_t ch;					// index to freqs array, freqs[ifreq]=868100000 default
	uint8_t sf;					// range from SF7 to SF12
	uint8_t debug;				// range 0 to 4

	bool cad;					// is CAD enabled?
	bool hop;					// Is HOP enabled (Note: SHould be disabled)
	bool isNode;				// Is gateway node enabled
	bool refresh;				// Is WWW browser refresh enabled

	String ssid;				// SSID of the last connected WiFi Network
	String pass;				// Password
};

// Definition of the configuration record that is read at startup and written
// when settings are changed.
extern espGwayConfig gwayConfig;

#ifdef __cplusplus
extern "C" {
#endif

int readConfig(const char *fn, struct espGwayConfig *c);
int writeGwayCfg(const char *fn, String ssid);
int writeConfig(const char *fn, struct espGwayConfig *c);

#ifdef __cplusplus
}
#endif


#endif /* end of include guarLORA_FILES_H */
