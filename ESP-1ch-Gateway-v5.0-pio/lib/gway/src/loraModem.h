#ifndef LORAMODEM_H
#define LORAMODEM_H

// 1-channel LoRa Gateway for ESP8266
// Copyright (c) 2016, 2017 Maarten Westenberg version for ESP8266
// Version 5.0.1
// Date: 2017-11-14
//
// 	based on work done by Thomas Telkamp for Raspberry PI 1ch gateway
//	and many other contributors.
//
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the MIT License
// which accompanies this distribution, and is available at
// https://opensource.org/licenses/mit-license.php
//
// Author: Maarten Westenberg (mw12554@hotmail.com)
//
// This file contains a number of compile-time settings and declarations that are
// specific to the LoRa rfm95, sx1276, sx1272 radio of the gateway.
//
//
// ------------------------------------------------------------------------------------

#include <Arduino.h>
#include <SPI.h>

extern long txDelay;

#define SPISPEED 8000000						// was 50000/50KHz < 10MHz

extern int freqs[10];
extern uint32_t  freq;
extern uint8_t	 ifreq;

// Set the structure for spreading factor
enum sf_t { SF6=6, SF7, SF8, SF9, SF10, SF11, SF12 };

// The state of the receiver. See Semtech Datasheet (rev 4, March 2015) page 43
// The _state is of the enum type (and should be cast when used as a number)
enum state_t { S_INIT=0, S_SCAN, S_CAD, S_RX, S_TX, S_TXDONE};

extern volatile state_t _state;
extern volatile uint8_t _event;

extern uint8_t _rssi;

#include <ESP-sc-gway.h>

// In order to make the CAD behaviour dynamic we set a variable
// when the CAD functions are defined. Value of 3 is minimum frequencies a
// gateway should support to be fully LoRa compliant.
#define NUM_HOPS 3

// Channel Activity Detection
// This function will scan for valid LoRa headers and determine the Spreading
// factor accordingly. If set to 1 we will use this function which means the
// 1-channel gateway will become even more versatile. If set to 0 we will use the
// continuous listen mode.
// Using this function means that we HAVE to use more dio pins on the RFM95/sx1276
// device and also connect enable dio1 to detect this state.
#define _CAD 1

extern bool _cad;
extern bool _hop;
extern bool inHop;
extern unsigned long nowTime;
extern unsigned long hopTime;
extern unsigned long msgTime;

// We support two pin-out configurations out-of-the-box: HALLARD and COMPRESULT.
// If you use one of these two, just set the parameter to the right value.
// If your pin definitions are different, update the loraModem.h file to reflect these settings.
//	1: HALLARD
//	2: COMRESULT pin out
//	3: For Heltec ESP32 based WIFI_LoRa_32 board
//  4: Other, define your own in loraModem.h
#define _PIN_OUT 3

typedef struct {
	uint8_t dio0;
	uint8_t dio1;
	uint8_t dio2;
	uint8_t ss;
	uint8_t rst;
} pins_t;

extern pins_t pins;

#if _PIN_OUT==1
	#define DI00		15	// GPIO15 / D8. For the Hallard board shared between DIO0/DIO1/DIO2
	#define DI01		15	// GPIO15 / D8. Used for CAD, may or not be shared with DIO0
	#define DI02		15	// GPIO15 / D8. Used for frequency hopping, don't care
	#define SS			16	// GPIO16 / D0. Select pin connected to GPIO16 / D0
	#define RST			0		// GPIO0 / D3. Reset pin not used
											// MISO 12 / D6
											// MOSI 13 / D7
											// CLK  14 / D5
#elif _PIN_OUT==2
	#define DI00		5		// GPIO5 / D1. Dio0 used for one frequency and one SF
	#define DI01		4		// GPIO4 / D2. Used for CAD, may or not be shared with DIO0
	#define DI02		0		// GPIO0 / D3. Used for frequency hopping, don't care
	#define SS			15	// GPIO15 / D8. Select pin connected to GPIO15
	#define RST			0		// GPIO0 / D3. Reset pin not used
#elif _PIN_OUT==3
	// For ComResult gateway PCB use the following settings
	#define SCK     5    // GPIO5  -- SX127x's SCK
	#define MISO    19   // GPIO19 -- SX127x's MISO
	#define MOSI    27   // GPIO27 -- SX127x's MOSI
	#define SS      18   // GPIO18 -- SX127x's CS
	#define RST     14   // GPIO14 -- SX127x's RESET
	#define DI00    26   // GPIO26 -- SX127x's IRQ(Interrupt Request)
	#define DI01		33   // GPIO4 / D2. Used for CAD, may or not be shared with DIO0
	#define DI02		32   // GPIO0 / D3. Used for frequency hopping, don't care
#else
		// Use your own pin definitions, and uncomment #error line below
		// MISO 12 / D6
		// MOSI 13 / D7
		// CLK  14 / D5
		// SS   16 / D0
	#error "Pin Definitions _PIN_OUT must be 1(HALLARD) or 2 (COMRESULT)"
#endif

// STATR contains the statictis that are kept by message.
// Ech time a message is received or sent the statistics are updated.
// In case STATISTICS==1 we define the last MAX_STAT messages as statistics
struct stat_t {
	unsigned long tmst;						// Time since 1970 in milli seconds
	unsigned long node;						// 4-byte DEVaddr (the only one known to gateway)
	uint8_t ch;								// Channel index to freqs array
	uint8_t sf;
#if RSSI==1
	int8_t rssi;							// XXX Can be < -128
#endif
	int8_t prssi;							// XXX Can be < -128
};

extern stat_t statt;

// Gather statistics on sensor and Wifi status
// 0= No statistics
// 1= Keep track of messages statistics, number determined by MAX_STAT
// 2= See 1 + Keep track of messages received PER each SF
#define STATISTICS 2

// Maximum number of statistics records gathered. 20 is a good maximum (memory intensive)
#define MAX_STAT 20

#if STATISTICS >= 1
// History of received uplink messages from nodes
extern stat_t statr[MAX_STAT];

// STATC contains the statistic that are gateway related and not per
// message. Example: Number of messages received on SF7 or number of (re) boots
// So where statr contains the statistics gathered per packet the statc
// contains general statics of the node
#if STATISTICS >= 2							// Only if we explicitely set it higher
struct stat_c {
	unsigned long sf7;						// Spreading factor 7
	unsigned long sf8;						// Spreading factor 8
	unsigned long sf9;						// Spreading factor 9
	unsigned long sf10;						// Spreading factor 10
	unsigned long sf11;						// Spreading factor 11
	unsigned long sf12;						// Spreading factor 12

	uint16_t boots;							// Number of boots
	uint16_t resets;
};
extern stat_c statc;
#endif
#else // STATISTICS==0
extern stat_t statr[1];						// Always have at least one element to store in
#endif

// Define the payload structure used to separate interrupt ans SPI
// processing from the loop() part
extern uint8_t payLoad[128];			// Payload i
struct LoraBuffer {
	uint8_t * payLoad;
	uint8_t payLength;
	uint32_t tmst;
	uint8_t sfTx;
	uint8_t powe;
	uint32_t fff;
	uint8_t crc;
	uint8_t iiq;
};

extern LoraBuffer LoraDown;

// Up buffer (from Lora to UDP)
//

struct Loraup {
	uint8_t payLoad[128];
	uint8_t payLength;
	int prssi;
	long snr;
	int rssicorr;
	uint8_t sf;
};

extern Loraup LoraUp;

extern SPISettings readSettings;
extern SPISettings writeSettings;

// ----------------------------------------
// Used by REG_PAYLOAD_LENGTH to set receive payload length
#define PAYLOAD_LENGTH              0x40	// 64 bytes
#define MAX_PAYLOAD_LENGTH          0x80	// 128 bytes

// Do not change these setting for RSSI detection. They are used for CAD
// Given the correction factor of 157, we can get to -120dB with this rating
//
#define RSSI_LIMIT	37							// was 39
#define RSSI_LIMIT_DOWN 34						// Was 34

// How long to wait in LoRa mode before using the RSSI value.
// This period should be as short as possible, yet sufficient
//
#define RSSI_WAIT	15							// was 100 works, 50 works, 40 works, 25 works
#define RSSI_WAIT_DOWN 10						// was 75 works, 35 works, 30 works, 20 works

// ============================================================================
// Set all definitions for Gateway
// ============================================================================
// Register definitions. These are the addresses of the TFM95, SX1276 that we
// need to set in the program.

#define REG_FIFO                    0x00
#define REG_OPMODE                  0x01
// Register 2 to 5 are unused for LoRa
#define REG_FRF_MSB					0x06
#define REG_FRF_MID					0x07
#define REG_FRF_LSB					0x08
#define REG_PAC                     0x09
#define REG_PARAMP                  0x0A
#define REG_LNA                     0x0C
#define REG_FIFO_ADDR_PTR           0x0D
#define REG_FIFO_TX_BASE_AD         0x0E
#define REG_FIFO_RX_BASE_AD         0x0F

#define REG_FIFO_RX_CURRENT_ADDR    0x10
#define REG_IRQ_FLAGS_MASK          0x11
#define REG_IRQ_FLAGS               0x12
#define REG_RX_NB_BYTES             0x13
#define REG_PKT_SNR_VALUE			0x19
#define REG_PKT_RSSI				0x1A	// latest package
#define REG_RSSI					0x1B	// Current RSSI, section 6.4, or  5.5.5
#define REG_HOP_CHANNEL				0x1C
#define REG_MODEM_CONFIG1           0x1D
#define REG_MODEM_CONFIG2           0x1E
#define REG_SYMB_TIMEOUT_LSB  		0x1F

#define REG_PAYLOAD_LENGTH          0x22
#define REG_MAX_PAYLOAD_LENGTH 		0x23
#define REG_HOP_PERIOD              0x24
#define REG_MODEM_CONFIG3           0x26
#define REG_RSSI_WIDEBAND			0x2C

#define REG_INVERTIQ				0x33
#define REG_DET_TRESH				0x37	// SF6
#define REG_SYNC_WORD				0x39
#define REG_TEMP					0x3C

#define REG_DIO_MAPPING_1           0x40
#define REG_DIO_MAPPING_2           0x41
#define REG_VERSION	  				0x42

#define REG_PADAC					0x5A
#define REG_PADAC_SX1272			0x5A
#define REG_PADAC_SX1276			0x4D


// ----------------------------------------
// opModes
#define SX72_MODE_SLEEP             0x80
#define SX72_MODE_STANDBY           0x81
#define SX72_MODE_FSTX              0x82
#define SX72_MODE_TX                0x83	// 0x80 | 0x03
#define SX72_MODE_RX_CONTINUOS      0x85

// ----------------------------------------
// LMIC Constants for radio registers
#define OPMODE_LORA      			0x80
#define OPMODE_MASK      			0x07
#define OPMODE_SLEEP     			0x00
#define OPMODE_STANDBY   			0x01
#define OPMODE_FSTX      			0x02
#define OPMODE_TX        			0x03
#define OPMODE_FSRX      			0x04
#define OPMODE_RX        			0x05
#define OPMODE_RX_SINGLE 			0x06
#define OPMODE_CAD       			0x07



// ----------------------------------------
// LOW NOISE AMPLIFIER

#define LNA_MAX_GAIN                0x23
#define LNA_OFF_GAIN                0x00
#define LNA_LOW_GAIN		    	0x20

// CONF REG
#define REG1                        0x0A
#define REG2                        0x84

// ----------------------------------------
// MC1 sx1276 RegModemConfig1
#define SX1276_MC1_BW_125           0x70
#define SX1276_MC1_BW_250           0x80
#define SX1276_MC1_BW_500           0x90
#define SX1276_MC1_CR_4_5           0x02
#define SX1276_MC1_CR_4_6           0x04
#define SX1276_MC1_CR_4_7           0x06
#define SX1276_MC1_CR_4_8           0x08
#define SX1276_MC1_IMPLICIT_HEADER_MODE_ON  0x01

#define SX72_MC1_LOW_DATA_RATE_OPTIMIZE     0x01 	// mandated for SF11 and SF12

// ----------------------------------------
// MC2 definitions
#define SX72_MC2_FSK                0x00
#define SX72_MC2_SF7                0x70		// SF7 == 0x07, so (SF7<<4) == SX7_MC2_SF7
#define SX72_MC2_SF8                0x80
#define SX72_MC2_SF9                0x90
#define SX72_MC2_SF10               0xA0
#define SX72_MC2_SF11               0xB0
#define SX72_MC2_SF12               0xC0

// ----------------------------------------
// MC3
#define SX1276_MC3_LOW_DATA_RATE_OPTIMIZE  0x08
#define SX1276_MC3_AGCAUTO                 0x04

// ----------------------------------------
// FRF
#define FRF_MSB						0xD9		// 868.1 Mhz
#define FRF_MID						0x06
#define FRF_LSB						0x66

// ----------------------------------------
// DIO function mappings           		     D0D1D2D3
#define MAP_DIO0_LORA_RXDONE   		0x00  // 00------ bit 7 and 6
#define MAP_DIO0_LORA_TXDONE   		0x40  // 01------
#define MAP_DIO0_LORA_CADDONE  		0x80  // 10------
#define MAP_DIO0_LORA_NOP   		0xC0  // 11------

#define MAP_DIO1_LORA_RXTOUT   		0x00  // --00---- bit 5 and 4
#define MAP_DIO1_LORA_FCC			0x10  // --01----
#define MAP_DIO1_LORA_CADDETECT		0x20  // --10----
#define MAP_DIO1_LORA_NOP      		0x30  // --11----

#define MAP_DIO2_LORA_FCC0      	0x00  // ----00-- bit 3 and 2
#define MAP_DIO2_LORA_FCC1      	0x04  // ----01-- bit 3 and 2
#define MAP_DIO2_LORA_FCC2      	0x08  // ----10-- bit 3 and 2
#define MAP_DIO2_LORA_NOP      		0x0C  // ----11-- bit 3 and 2

#define MAP_DIO3_LORA_CADDONE  		0x00  // ------00 bit 1 and 0
#define MAP_DIO3_LORA_NOP      		0x03  // ------11

#define MAP_DIO0_FSK_READY     		0x00  // 00------ (packet sent / payload ready)
#define MAP_DIO1_FSK_NOP       		0x30  // --11----
#define MAP_DIO2_FSK_TXNOP     		0x04  // ----01--
#define MAP_DIO2_FSK_TIMEOUT   		0x08  // ----10--

// ----------------------------------------
// Bits masking the corresponding IRQs from the radio
#define IRQ_LORA_RXTOUT_MASK 		0x80
#define IRQ_LORA_RXDONE_MASK 		0x40
#define IRQ_LORA_CRCERR_MASK 		0x20
#define IRQ_LORA_HEADER_MASK 		0x10
#define IRQ_LORA_TXDONE_MASK 		0x08
#define IRQ_LORA_CDDONE_MASK 		0x04
#define IRQ_LORA_FHSSCH_MASK 		0x02
#define IRQ_LORA_CDDETD_MASK 		0x01


// ----------------------------------------
// Definitions for UDP message arriving from server
#define PROTOCOL_VERSION			0x01
#define PKT_PUSH_DATA				0x00
#define PKT_PUSH_ACK				0x01
#define PKT_PULL_DATA				0x02
#define PKT_PULL_RESP				0x03
#define PKT_PULL_ACK				0x04
#define PKT_TX_ACK                  0x05

#define MGT_RESET					0x15	// Not a LoRa Gateway Spec message
#define MGT_SET_SF					0x16
#define MGT_SET_FREQ				0x17

#ifdef __cplusplus
extern "C" {
#endif

uint8_t readRegister(uint8_t addr);
void writeRegister(uint8_t addr, uint8_t value);
void writeBuffer(uint8_t addr, uint8_t *buf, uint8_t len);
void setRate(uint8_t sf, uint8_t crc);
void  setFreq(uint32_t freq);
void setPow(uint8_t powe);
void  opmode(uint8_t mode);
void hop();
uint8_t receivePkt(uint8_t *payload);
bool sendPkt(uint8_t *payLoad, uint8_t payLength);
void loraWait(uint32_t tmst);
void txLoraModem(uint8_t *payLoad, uint8_t payLength, uint32_t tmst, uint8_t sfTx, uint8_t powe, uint32_t freq, uint8_t crc, uint8_t iiq);
void rxLoraModem();
void cadScanner();
void initLoraModem();
void stateMachine();
void ICACHE_RAM_ATTR Interrupt_0();
void ICACHE_RAM_ATTR Interrupt_1();
void ICACHE_RAM_ATTR Interrupt_2();

#ifdef __cplusplus
}
#endif

#endif //LORAMODEM_H
