/************************************************************
 * LMIC LoRaWAN configuration
 * 
 * Read the values from TTN console (or whatever applies)
 * 
 ************************************************************/

#include <Arduino.h>

/* 

// Set your DEVEUI here, if you have one. You can leave this untouched, 
// then the DEVEUI will be generated during runtime from device's MAC adress
// Note: Use same format as in TTN console (cut & paste, for your convenience)
// *** Take care : If Using a board with Microchip 24AA02E64 Uinique ID for deveui, **
// *** this DEVEUI will be overwriten by the one contained in the Microchip module ***
static const u1_t DEVEUI[8]={ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

// Note: Use msb format for APPEUI as in TTN console (cut & paste, for your convenience)
// For TTN, APPEUI always starts with 0x70, 0xB3, 0xD5
static const u1_t APPEUI[8]={ 0x70, 0xB3, 0xD5, 0x00, 0x00, 0x00, 0x00, 0x00 };

// Note: Use msb format for APPEUI as in TTN console (cut & paste, for your convenience)
static const u1_t APPKEY[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

*/
static const u1_t DEVEUI[8]={ 0x06, 0x00, 0xC5, 0x0A, 0x80, 0xFF, 0x0E, 0xE0 };

// Note: Use msb format for APPEUI as in TTN console (cut & paste, for your convenience)
// For TTN, APPEUI always starts with 0x70, 0xB3, 0xD5
static const u1_t APPEUI[8]= { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x00, 0x85, 0xBE };

// Note: Use msb format for APPEUI as in TTN console (cut & paste, for your convenience)
static const u1_t APPKEY[16] = { 0x76, 0x79, 0x4A, 0x82, 0x0A, 0xFF, 0xB9, 0x16, 0xCB, 0xB3, 0xD4, 0xB2, 0x6A, 0x51, 0xAF, 0x46 };
