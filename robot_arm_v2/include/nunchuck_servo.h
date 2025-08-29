#ifndef NUNCHUCK_SERVO_H
#define NUNCHUCK_SERVO_H

#include <WiiChuck.h>
#include <ESP32Servo.h>
#include <Wire.h>

int mappingJoystick(uint8_t v, int center, int dz);
void initNunchuck(Accessory &nunchuck, Servo *servos);
void updateNunchuck(Accessory &nunchuck, Servo *servos);

#endif