#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <WiFi.h>
#include <esp_now.h>

#define CONTROL_BOX 1

extern const uint8_t control_mac[6];

enum MSG_TYPE : uint8_t
{
    MSG_CMD = 1,
    MSG_TLM = 2
};

typedef struct __attribute__((packed))
{
    uint8_t type;
    uint8_t target;
    uint16_t potvalue;
} command_msg_t;

typedef struct __attribute__((packed))
{
    uint8_t type;
    uint8_t source;
    uint16_t voltage;
    uint16_t current;
    uint16_t level;
    uint32_t pressure;
} telemetry_msg_t;

void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void addPeer();
void onDataReceived(const uint8_t *mac_addr, const uint8_t *data, int len);
void sendTelemetry();
bool initESPNow();

#endif