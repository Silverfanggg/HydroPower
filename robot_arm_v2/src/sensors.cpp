#include "sensors.h"

const unsigned long measure_interval = 500;

volatile float waterLevel = 0.0f;
volatile float waterPressure = 0.0f;
volatile uint16_t lastDuty = 0;

void initSensors()
{
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

float getDistance()
{
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH, 25000UL);
    if (duration == 0)
        return NAN;

    float distance = (duration * 0.03f) / 2.0f;
    if (distance < 0.5f || distance > 400.0f)
        return NAN;

    return distance;
}

float measureWaterLevel()
{
    static unsigned long lastMeasure = 0;

    float distance = getDistance();
    
    if(millis() - lastMeasure >= measure_interval)
    {
        lastMeasure = millis();
        if (!isnan(distance))
        {
            waterLevel = WATER_LEVEL_MAX - distance;
            waterLevel = constrain(waterLevel, WATER_LEVEL_MIN, WATER_LEVEL_MAX);
        }
    }

    return waterLevel;
}

float calculatePressure()
{
    static unsigned long lastMeasure = 0;

    if(millis() - lastMeasure >= measure_interval)
    {
        lastMeasure = millis();
        waterPressure = waterLevel * GRAVITY * WATER_DENSITY / 1000.0f;
    }

    return waterPressure;
}