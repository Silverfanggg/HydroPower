#ifndef PUMP_CONTROL_H
#define PUMP_CONTROL_H

#include <Arduino.h>

#define PUMP_PWM_PIN 8
#define PUMP_PWM_CH 0
#define PWM_FREQ 20000
#define PWM_RES 12

extern const unsigned long FS_TIMEOUT;
extern volatile unsigned long lastFSTime;

void initPumpControl();
void applyPumpDuty(uint16_t dutyRaw);
void checkPumpTimeout();

#endif