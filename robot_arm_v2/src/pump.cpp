#include "pump.h"
#include "communication.h"
#include "sensors.h"

const unsigned long FS_TIMEOUT = 5000;
volatile unsigned long lastFSTime = 0;

void initPumpControl()
{
  ledcSetup(PUMP_PWM_CH, PWM_FREQ, PWM_RES);
  ledcAttachPin(PUMP_PWM_PIN, PUMP_PWM_CH);
  ledcWrite(PUMP_PWM_CH, 0);
  lastFSTime = millis();
}

void applyPumpDuty(uint16_t dutyRaw)
{
  uint16_t duty = constrain(dutyRaw, 0, (1 << PWM_RES) - 1);

  if (waterLevel < WATER_LEVEL_MIN)
  {
    duty = 0;
  }

  if (duty != lastDuty)
  {
    lastDuty = duty;
    ledcWrite(PUMP_PWM_CH, duty);
    sendTelemetry();
  }
}

void checkPumpTimeout()
{
  if (millis() - lastFSTime > FS_TIMEOUT)
  {
    if (lastDuty != 0)
    {
      lastDuty = 0;
      ledcWrite(PUMP_PWM_CH, 0);
      sendTelemetry();
    }
  }
}