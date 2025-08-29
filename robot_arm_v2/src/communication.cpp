#include "communication.h"
#include "sensors.h"
#include "pump.h"

const uint8_t control_mac[6] = {0x24, 0x6F, 0x28, 0xAA, 0xBB, 0xCC};

void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status){}

void addPeer()
{
    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, control_mac, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
        Serial.println("Error adding ESP-NOW peer");
    }
}

void onDataReceived(const uint8_t *mac_addr, const uint8_t *data, int len)
{
    if (len == sizeof(command_msg_t) && MSG_CMD == data[0])
    {
        command_msg_t cmd;
        memcpy(&cmd, data, sizeof(command_msg_t));
        if (cmd.target == CONTROL_BOX)
        {
            lastFSTime = millis();
            applyPumpDuty(cmd.potvalue);
        }
    }
}

void sendTelemetry()
{
    telemetry_msg_t msg = {};
    msg.type = MSG_TLM;
    msg.source = CONTROL_BOX;
    msg.level = measureWaterLevel();
    msg.pressure = calculatePressure();

    esp_now_send(control_mac, (uint8_t *)&msg, sizeof(msg));
}

bool initESPNow()
{
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW");
        return false;
    }
    esp_now_register_send_cb(onDataSent);
    esp_now_register_recv_cb(onDataReceived);
    addPeer();
    Serial.println("ESP-NOW initialized");
    return true;
}
