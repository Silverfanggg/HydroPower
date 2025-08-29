#include "communication.h"

TelemetryData robot1Data = {0};
TelemetryData robot2Data = {0};
WinnerData gameResult = {0};

bool robot1DataReceived = false;
bool robot2DataReceived = false;
bool winnerDataReceived = false;

unsigned long robot1LastReceived = 0;
unsigned long robot2LastReceived = 0;
unsigned long winnerLastReceived = 0;

void communication_onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    if (status == ESP_NOW_SEND_SUCCESS) {
        Serial.println("Data sent successfully");
    } else {
        Serial.println("Data send failed!");
    }
}

void communication_onDataReceived(const uint8_t *mac_addr, const uint8_t *data, int len)
{
    if (len == sizeof(TelemetryData) && data[0] == MSG_TYPE_TELEMETRY)
    {
        TelemetryData telemetry;
        memcpy(&telemetry, data, sizeof(TelemetryData));

        if (telemetry.source == 1)
        {
            robot1Data = telemetry;
            robot1DataReceived = true;
            robot1LastReceived = millis();
            Serial.println("Robot 1 telemetry received");
        }
        else if (telemetry.source == 2)
        {
            robot2Data = telemetry;
            robot2DataReceived = true;
            robot2LastReceived = millis();
            Serial.println("Robot 2 telemetry received");
        }
    }
    else if (len == sizeof(WinnerData) && data[0] == MSG_TYPE_WINNER)
    {
        memcpy(&gameResult, data, sizeof(WinnerData));
        winnerDataReceived = true;
        winnerLastReceived = millis();
        Serial.print("Winner: ");
        Serial.println(gameResult.winner);
    }

    communication_addPeer(mac_addr);
}

void communication_addPeer(const uint8_t *mac_addr)
{
    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, mac_addr, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    esp_now_add_peer(&peerInfo);
}

bool communication_init()
{
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW");
        return false;
    }

    esp_now_register_send_cb(communication_onDataSent);
    esp_now_register_recv_cb(communication_onDataReceived);

    communication_addPeer(R1_MAC_ADDRESS);
    communication_addPeer(R2_MAC_ADDRESS);
    communication_addPeer(R3_MAC_ADDRESS);

    return true;
}

esp_err_t communication_sendPotValue(const uint8_t *mac_addr, uint8_t target, uint16_t potValue)
{
    PotMessage potMsg;
    potMsg.type = MSG_TYPE_POT_VALUE;
    potMsg.target = target;
    potMsg.potvalue = potValue;
    
    return esp_now_send(mac_addr, (uint8_t *)&potMsg, sizeof(potMsg));
}
