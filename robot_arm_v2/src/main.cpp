#include "communication.h"
#include "nunchuck_servo.h"
#include "pump.h"
#include "sensors.h"

Accessory nunchuck;
Servo servos[3];

void setup()
{
  Serial.begin(115200);

  initSensors();
  initNunchuck(nunchuck, servos);
  initPumpControl();
  initESPNow();
}

void loop()
{
  updateNunchuck(nunchuck, servos);
  checkPumpTimeout();
  measureWaterLevel();
  calculatePressure();
}