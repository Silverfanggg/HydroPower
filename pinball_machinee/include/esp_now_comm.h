#ifndef ESP_NOW_COMM_H
#define ESP_NOW_COMM_H

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

extern const uint8_t control_mac[] = {0x24, 0x6F, 0x28, 0xAA, 0xBB, 0xCC};

void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void addPeer(const uint8_t *mac_addr);
bool initESPNow();

#endif
