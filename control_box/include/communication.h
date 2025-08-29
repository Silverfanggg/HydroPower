#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "config.h"
#include <esp_now.h>
#include <WiFi.h>

bool communication_init();
esp_err_t communication_sendPotValue(const uint8_t *mac_addr, uint8_t target, uint16_t potValue);
void communication_handleReceivedData(const uint8_t *data, int len);
void communication_onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void communication_onDataReceived(const uint8_t *mac_addr, const uint8_t *data, int len);
void communication_addPeer(const uint8_t *mac_addr);

extern TelemetryData robot1Data;
extern TelemetryData robot2Data;
extern WinnerData gameResult;
extern bool robot1DataReceived;
extern bool robot2DataReceived;
extern bool winnerDataReceived;
extern unsigned long robot1LastReceived;
extern unsigned long robot2LastReceived;
extern unsigned long winnerLastReceived;

#endif
