#ifndef GWAY_GLOBALS_H
#define GWAY_GLOBALS_H

#include <Arduino.h>
#include <loraModem.h>

extern int debug;

extern bool sx1272;
extern uint32_t cp_nb_rx_rcv;
extern uint32_t cp_nb_rx_ok;
extern uint32_t cp_nb_rx_bad;
extern uint32_t cp_nb_rx_nocrc;
extern uint32_t cp_up_pkt_fwd;

extern uint8_t MAC_array[6];

extern sf_t sf;
extern sf_t sfi;

extern float lat;
extern float lon;
extern int   alt;
extern char platform[24];
extern char email[40];
extern char description[64];

extern IPAddress ttnServer;
extern IPAddress thingServer;

//extern WiFiUDP Udp;
extern uint32_t stattime;
extern uint32_t pulltime;
extern uint32_t lastTmst;

extern int inSPI;
extern int inSPO;
extern int mutexSPI;

extern int inIntr;

#if A_SERVER==1
	#ifdef ESP32BUILD
		extern WebServer server;
	#else
		 extern ESP8266WebServer server;
	#endif
	extern uint32_t wwwtime;
#endif

#if NTP_INTR==0
extern uint32_t ntptimer;
#endif

#if OLED==1
#include "SSD1306.h"
extern SSD1306  display;
#endif

extern wpas wpa[3];

#endif //GWAY_GLOBALS_H
