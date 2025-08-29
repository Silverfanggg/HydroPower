#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

#define ECHO_PIN 6
#define TRIG_PIN 7

extern volatile float waterLevel;
extern volatile float waterPressure;
extern volatile uint16_t lastDuty;

#define WATER_LEVEL_MAX 30.0f
#define WATER_LEVEL_MIN 5.0f

#define GRAVITY 9.81f
#define WATER_DENSITY 1000.0f

void initSensors();
float measureWaterLevel();
float calculatePressure();

#endif