#ifndef WWWSERVER_H
#define WWWSERVER_H

#include <ESP-sc-gway.h>



#ifdef ESP32BUILD
#include "esp_wifi.h"
#include "WiFi.h"
#else
#include <ESP8266WiFi.h>
#include <DNSServer.h> 							// Local DNSserver
#endif

#ifdef __cplusplus
extern "C" {
#endif

void setupWWW();
static void renewWebPage();
void sendWebPage(const char *cmd, const char *arg);
static void wifiData();
static void systemData();
static void sensorData();
static void statisticsData();
static void interruptData();
static void configData();
static void openWebPage();
static void setVariables(const char *cmd, const char *arg);
static void stringTime(unsigned long t, String& response);
static void printHEX(char * hexa, const char sep, String& response);
static void printIP(IPAddress ipa, const char sep, String& response);

#ifdef __cplusplus
}
#endif

#endif //WWWSERVER_H
