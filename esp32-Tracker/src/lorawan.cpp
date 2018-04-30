// Basic Config
#include "globals.h"

// LMIC-Arduino LoRaWAN Stack
#include "loraconf.h"
#include <lmic.h>
#include <hal/hal.h>

#ifdef PIN_GPS_RX
#include <TinyGPS++.h>
#endif

#ifdef MCP_24AA02E64_I2C_ADDRESS
    #include <Wire.h> // Needed for 24AA02E64, does not hurt anything if included and not used
#endif

// Local logging Tag
static const char *TAG = "lorawan";

#ifdef PIN_GPS_RX
TinyGPSPlus gps;
HardwareSerial GPSSerial(1);
float latitude;
float longitude;
#endif

static uint8_t AppPort = 2; // LORAWAN_APP_PORT


static uint8_t AppDataSize = 16;


/*!
 * LoRaWAN compliance tests support data
 */
struct ComplianceTest_s {
	bool Running;
	uint8_t State;
	bool IsTxConfirmed;
	uint8_t AppPort;
	uint8_t AppDataSize;
	uint8_t *AppDataBuffer;
	uint16_t DownLinkCounter;
	bool LinkCheck;
	uint8_t DemodMargin;
	uint8_t NbGateways;
} ComplianceTest;

void dump_hex2str(uint8_t *buf, uint8_t len) {
	for (uint8_t i = 0; i < len; i++) {
		printf("%02X ", buf[i]);
	}
	printf("\r\n");
}

/*!
 * User application data buffer size
 */
#define LORAWAN_APP_DATA_MAX_SIZE                           242

/*!
 * User application data
 */
static uint8_t AppData[LORAWAN_APP_DATA_MAX_SIZE];


// functions defined in rcommand.cpp
void rcommand(uint8_t cmd, uint8_t arg);
void switch_lora(uint8_t sf, uint8_t tx);

// DevEUI generator using devices's MAC address
void gen_lora_deveui(uint8_t *pdeveui) {
    uint8_t *p = pdeveui, dmac[6];
    int i = 0;
    esp_efuse_mac_get_default(dmac);
    // deveui is LSB, we reverse it so TTN DEVEUI display 
    // will remain the same as MAC address
    // MAC is 6 bytes, devEUI 8, set first 2 ones 
    // with an arbitrary value
    *p++ = 0xFF;
    *p++ = 0xFE;
    // Then next 6 bytes are mac address reversed
    for ( i=0; i<6 ; i++) {
        *p++ = dmac[5-i];
    }
}

// Function to do a byte swap in a byte array
void RevBytes(unsigned char* b, size_t c)
{
  u1_t i;
  for (i = 0; i < c / 2; i++)
  { unsigned char t = b[i];
    b[i] = b[c - 1 - i];
    b[c - 1 - i] = t; }
}

void get_hard_deveui(uint8_t *pdeveui) {
    // read DEVEUI from Microchip 24AA02E64 2Kb serial eeprom if present
#ifdef MCP_24AA02E64_I2C_ADDRESS
    uint8_t i2c_ret;
    // Init this just in case, no more to 100KHz
    Wire.begin(OLED_SDA, OLED_SCL, 100000);
    Wire.beginTransmission(MCP_24AA02E64_I2C_ADDRESS);
    Wire.write(MCP_24AA02E64_MAC_ADDRESS); 
    i2c_ret = Wire.endTransmission();
    // check if device seen on i2c bus
    if (i2c_ret == 0) {
        char deveui[32]="";
        uint8_t data;
        Wire.beginTransmission(MCP_24AA02E64_I2C_ADDRESS);
        Wire.write(MCP_24AA02E64_MAC_ADDRESS); 
        Wire.requestFrom(MCP_24AA02E64_I2C_ADDRESS, 8);
        while (Wire.available()) {
            data = Wire.read();
            sprintf(deveui+strlen(deveui), "%02X ", data);
            *pdeveui++ = data;
        }
        i2c_ret = Wire.endTransmission();
        ESP_LOGI(TAG, "Serial EEPROM 24AA02E64 found, read DEVEUI %s", deveui);
    } else {
        ESP_LOGI(TAG, "Serial EEPROM 24AA02E64 not found ret=%d", i2c_ret);
    }
    // Set back to 400KHz to speed up OLED
    Wire.setClock(400000);
#endif // MCP 24AA02E64    
}

#define VERBOSE
#ifdef VERBOSE

// Display a key
void printKey(const char * name, const uint8_t * key, uint8_t len, bool lsb) {
  uint8_t start=lsb?len:0;
  uint8_t end = lsb?0:len;
  const uint8_t * p ;
  char keystring[len+1] = "", keybyte[3];
  for (uint8_t i=0; i<len ; i++) {
     p = lsb ? key+len-i-1 : key+i;
     sprintf(keybyte, "%02X", * p);
     strncat(keystring, keybyte, 2);
    }
  ESP_LOGI(TAG, "%s: %s", name, keystring);
}

// Display OTAA keys
void printKeys(void) {
	// LMIC may not have used callback to fill 
	// all EUI buffer so we do it here to a temp
	// buffer to be able to display them
	uint8_t buf[32];
	os_getDevEui((u1_t*) buf);
	printKey("DevEUI", buf, 8, true);
	os_getArtEui((u1_t*) buf);
	printKey("AppEUI", buf, 8, true);
	os_getDevKey((u1_t*) buf);
	printKey("AppKey", buf, 16, false);
}



#endif // VERBOSE

#ifdef PIN_GPS_RX
bool get_coords () {
  while (GPSSerial.available())
    gps.encode(GPSSerial.read());

  latitude  = gps.location.lat();
  longitude = gps.location.lng();
#if 0
  // Only update if location is valid and has changed
  if ((latitude && longitude) && latitude != latlong.f[0]
      && longitude != latlong.f[1]) {
    latlong.f[0] = latitude;
    latlong.f[1] = longitude;
    for (int i = 0; i < 8; i++)
      Serial.print(latlong.bytes[i], HEX);
    Serial.println();
  }
  u8x8.setCursor(0, 2);
  u8x8.print("Lat: ");
  u8x8.setCursor(5, 2);
  sprintf(s, "%f", latitude);
  u8x8.print(s);
  u8x8.setCursor(0, 3);
  u8x8.print("Lng: ");
  u8x8.setCursor(5, 3);
  sprintf(s, "%f", longitude);
  u8x8.print(s);
#endif
  return(gps.location.isValid());
}

static void PrepareTxFrame(uint8_t port) {
	double latitude, longitude, hdopGps = 0;
	int16_t altitudeGps = 0xFFFF;
	uint8_t ret;
	uint16_t bat;

	switch (port) {
	//https://mydevices.com/cayenne/docs/lora/#lora-cayenne-low-power-payload
	//cayenne LPP GPS
	case 2: {
		//ret = GpsGetLatestGpsPositionDouble(&latitude, &longitude);
		altitudeGps = 30; //GpsGetLatestGpsAltitude(); // in m
		hdopGps = 3; // GpsGetLatestGpsHorizontalDilution();
		//printf("[Debug]: latitude: %f, longitude: %f , altitudeGps: %d \n", latitude, longitude, altitudeGps);
		int32_t lat = ((latitude + 90) / 180.0) * 16777215;
		int32_t lon = ((longitude + 180) / 360.0) * 16777215;
		int16_t alt = altitudeGps;
		int8_t hdev = hdopGps / 10;
		
		if (ret == 1) { // SUCCESS

                        printf("Lat: %d Lon: %d, Alt: %d\r\n", (int)lat, (int)lon, alt);

			AppData[0] = lat >> 16;
			AppData[1] = lat >> 8;
			AppData[2] = lat;

			AppData[3] = lon >> 16;
			AppData[4] = lon >> 8;
			AppData[5] = lon;

			AppData[6] = alt >> 8;
			AppData[7] = alt;

			AppData[9] = hdev;

			AppDataSize = 10;

		} else {
                        printf("No GPS fix.\r\n");
			AppDataSize = 0;
		}
	}
		break;

		//cayenne LPP Temp
	case 3: {
		//bat = BoardBatteryMeasureVolage();
		//AppData[0] = (bat / 10) & 0xFF;
		//AppData[1] = ((bat / 10) >> 8) & 0xFF;
		//AppDataSize = 2;
		//printf("[Debug]: Bat: %dmv\r\n", bat);
	}
		break;

		//cayenne LPP Acceleration
	case 4: {

		//LIS3DH_ReadReg(LIS3DH_OUT_X_H, AppData + 0);
		//DelayMs(2);
		//LIS3DH_ReadReg(LIS3DH_OUT_X_L, AppData + 1);
		//DelayMs(2);
		//LIS3DH_ReadReg(LIS3DH_OUT_Y_H, AppData + 2);
		//DelayMs(2);
		//LIS3DH_ReadReg(LIS3DH_OUT_Y_L, AppData + 3);
		//DelayMs(2);
		//LIS3DH_ReadReg(LIS3DH_OUT_Z_H, AppData + 4);
		//DelayMs(2);
		//LIS3DH_ReadReg(LIS3DH_OUT_Z_L, AppData + 5);
		//DelayMs(2);

		//AppDataSize = 6;
		//printf("[Debug]: ACC X:%04X Y:%04X Z:%04X\r\n",
		//		AppData[0] << 8 | AppData[1], AppData[2] << 8 | AppData[3],
		//		AppData[4] << 8 | AppData[5]);
	}
		break;

	case 224:
		if (ComplianceTest.LinkCheck == true) {
			ComplianceTest.LinkCheck = false;
			AppDataSize = 3;
			AppData[0] = 5;
			AppData[1] = ComplianceTest.DemodMargin;
			AppData[2] = ComplianceTest.NbGateways;
			ComplianceTest.State = 1;
		} else {
			switch (ComplianceTest.State) {
			case 4:
				ComplianceTest.State = 1;
				break;
			case 1:
				AppDataSize = 2;
				AppData[0] = ComplianceTest.DownLinkCounter >> 8;
				AppData[1] = ComplianceTest.DownLinkCounter;
				break;
			}
		}
		break;
	default:
		break;
	}
}


#endif

void do_send(osjob_t* j){
    uint8_t mydata[4];

    // Sum of unique WIFI MACs seen
    mydata[0] = (macs_wifi & 0xff00) >> 8;
    mydata[1] = macs_wifi  & 0xff;
    
    #ifdef BLECOUNTER
        // Sum of unique BLE MACs seen
        mydata[2] = (macs_ble & 0xff00) >> 8;
        mydata[3] = macs_ble  & 0xff;
    #else
        mydata[2] = 0;
        mydata[3] = 0;
    #endif

    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) {
        ESP_LOGI(TAG, "OP_TXRXPEND, not sending");
        sprintf(display_lmic, "LORA BUSY");
    } else {
        // Prepare upstream data transmission at the next possible time.
        LMIC_setTxData2(1, mydata, sizeof(mydata), (cfg.countermode & 0x02));
        ESP_LOGI(TAG, "Packet queued");
        sprintf(display_lmic, "PACKET QUEUED");
        // clear counter if not in cumulative counter mode
        if (cfg.countermode != 1) {
            reset_counters();                       // clear macs container and reset all counters
            reset_salt();                           // get new salt for salting hashes
        }
    }

    // Schedule next transmission
    os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(cfg.sendcycle * 2), do_send);

} // do_send()

void onEvent (ev_t ev) {
    char buff[24]="";
    
    switch(ev) {
        case EV_SCAN_TIMEOUT:   strcpy_P(buff, PSTR("SCAN TIMEOUT"));   break;
        case EV_BEACON_FOUND:   strcpy_P(buff, PSTR("BEACON FOUND"));   break;
        case EV_BEACON_MISSED:  strcpy_P(buff, PSTR("BEACON MISSED")); break;
        case EV_BEACON_TRACKED: strcpy_P(buff, PSTR("BEACON TRACKED")); break;
        case EV_JOINING:        strcpy_P(buff, PSTR("JOINING"));        break;
        case EV_LOST_TSYNC:     strcpy_P(buff, PSTR("LOST TSYNC"));     break;
        case EV_RESET:          strcpy_P(buff, PSTR("RESET"));          break;
        case EV_RXCOMPLETE:     strcpy_P(buff, PSTR("RX COMPLETE"));    break;
        case EV_LINK_DEAD:      strcpy_P(buff, PSTR("LINK DEAD"));      break;
        case EV_LINK_ALIVE:     strcpy_P(buff, PSTR("LINK ALIVE"));     break;
        case EV_RFU1:           strcpy_P(buff, PSTR("RFUI"));           break;
        case EV_JOIN_FAILED:    strcpy_P(buff, PSTR("JOIN FAILED"));    break;
        case EV_REJOIN_FAILED:  strcpy_P(buff, PSTR("REJOIN FAILED"));  break;
        
        case EV_JOINED:

            joinstate=true;
            strcpy_P(buff, PSTR("JOINED"));

            // Disable link check validation (automatically enabled
            // during join, but not supported by TTN at this time).
            LMIC_setLinkCheckMode(0);
            // set data rate adaptation
            LMIC_setAdrMode(cfg.adrmode);
            // Set data rate and transmit power (note: txpower seems to be ignored by the library)
            switch_lora(cfg.lorasf,cfg.txpower);
            
            // show effective LoRa parameters after join
            ESP_LOGI(TAG, "ADR=%d, SF=%d, TXPOWER=%d", cfg.adrmode, cfg.lorasf, cfg.txpower);
            break;

        case EV_TXCOMPLETE:

            strcpy_P(buff, (LMIC.txrxFlags & TXRX_ACK) ? PSTR("RECEIVED ACK") : PSTR("TX COMPLETE")); 
            sprintf(display_lora, ""); // erase previous LoRa message from display
            
            if (LMIC.dataLen) {
                ESP_LOGI(TAG, "Received %d bytes of payload, RSSI %d SNR %d", LMIC.dataLen, LMIC.rssi, (signed char)LMIC.snr / 4);
                // LMIC.snr = SNR twos compliment [dB] * 4
                // LMIC.rssi = RSSI [dBm] (-196...+63)
                sprintf(display_lora, "RSSI %d SNR %d", LMIC.rssi, (signed char)LMIC.snr / 4 );

                // check if payload received on command port, then call remote command interpreter
                if ( (LMIC.txrxFlags & TXRX_PORT) && (LMIC.frame[LMIC.dataBeg-1] == RCMDPORT ) ) {
                    // caution: buffering LMIC values here because rcommand() can modify LMIC.frame
                    unsigned char* buffer = new unsigned char[MAX_LEN_FRAME];
                    memcpy(buffer, LMIC.frame, MAX_LEN_FRAME); //Copy data from cfg to char*
                    int i, k = LMIC.dataBeg, l = LMIC.dataBeg+LMIC.dataLen-2;
                    for (i=k; i<=l; i+=2)
                        rcommand(buffer[i], buffer[i+1]);
                    delete[] buffer; //free memory
                }
            }
            break;

        default: sprintf_P(buff, PSTR("UNKNOWN EVENT %d"), ev);      break;
    }

    // Log & Display if asked
    if (*buff) {
        ESP_LOGI(TAG, "EV_%s", buff);
        sprintf(display_lmic, buff);
    }
 
} // onEvent()

