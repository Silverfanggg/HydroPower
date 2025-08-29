#include <Arduino.h>
#include "config.h"
#include "communication.h"
#include "display.h"
#include "sensor.h"

void setup()
{
  Serial.begin(115200);

  if (!display_init())
  {
    Serial.println("Display initialization failed!");
    return;
  }

  sensor_init();

  if (!communication_init())
  {
    Serial.println("Communication initialization failed!");
    return;
  }

  Serial.println("Control Box ready!");
}

void loop()
{
  sensor_readAndSendPotValues();
  display_update();
}
