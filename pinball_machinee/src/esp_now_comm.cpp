#include <WiFi.h>
#include <esp_now.h>
#include "esp_now.h"

const uint8_t control_mac[] = {0x24, 0x6F, 0x28, 0xAA, 0xBB, 0xCC};

void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status){}

void addPeer(const uint8_t *mac_addr)
{
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, mac_addr, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
  }
}

bool initESPNow()
{
    WiFi.mode(WIFI_STA);
    esp_now_init();
    esp_now_register_send_cb(onDataSent);
    addPeer(control_mac);

    return true;
}