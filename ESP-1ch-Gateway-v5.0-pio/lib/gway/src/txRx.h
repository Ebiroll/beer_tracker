#ifndef TXRX_H
#define TXRX_H

#include <Arduino.h>
#include <gBase64.h>							// https://github.com/adamvr/arduino-base64 (changed the name)

#ifdef __cplusplus
extern "C" {
#endif

int sendPacket(uint8_t *buf, uint8_t length);
int buildPacket(uint32_t tmst, uint8_t *buff_up, struct Loraup LoraUp, bool internal);
int receivePacket();

#ifdef __cplusplus
}
#endif

#endif //TXRX_H
