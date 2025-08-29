#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

#define POT_PIN 34
#define POT_PIN2 35
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

extern const int ESP_SCL;
extern const int ESP_SDA;
extern const unsigned long SEND_INTERVAL;
extern const unsigned long DISPLAY_UPDATE_INTERVAL;
extern const unsigned long DATA_TIMEOUT;

extern const uint8_t R1_MAC_ADDRESS[];
extern const uint8_t R2_MAC_ADDRESS[];
extern const uint8_t R3_MAC_ADDRESS[];

enum MSG_TYPE : uint8_t
{
  MSG_TYPE_POT_VALUE = 1,
  MSG_TYPE_TELEMETRY = 2,
  MSG_TYPE_WINNER = 3
};

typedef struct __attribute__((packed))
{
  uint8_t type;
  uint8_t target;
  uint16_t potvalue;
} PotMessage;

typedef struct __attribute__((packed))
{
  uint8_t type;
  uint8_t source;
  uint16_t voltage;
  uint16_t current;
  uint16_t level;
  uint32_t pressure;
} TelemetryData;

typedef struct __attribute__((packed))
{
  uint8_t type;
  uint8_t winner;
} WinnerData;

#endif
