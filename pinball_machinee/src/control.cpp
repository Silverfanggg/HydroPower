#include <ESP32Servo.h>
#include "control.h"

Servo servo[4];
ServoState servoStates[4];

const unsigned long DEBOUNCE_DELAY = 50;
const unsigned long SERVO_DURATION = 1000;

void initButton()
{
    for (int i = 0; i < 4; i++)
    {
        pinMode(BUTTON_PINS[i], INPUT_PULLUP);
    }
}

void initServos()
{
    for (int i = 0; i < 4; i++)
    {
        servo[i].attach(SERVO_PINS[i]);
        servo[i].write(0);
        servoStates[i].isActive = false;
        servoStates[i].startTime = 0;
        servoStates[i].currentPosition = 0;
    }
}

void activateServo(int index)
{
    if (index >= 0 && index < 4 && !servoStates[index].isActive)
    {
        servoStates[index].isActive = true;
        servoStates[index].startTime = millis();
        servoStates[index].currentPosition = 90;
        servo[index].write(90);
    }
}

void updateServos()
{
    unsigned long currentTime = millis();
    
    for (int i = 0; i < 4; i++)
    {
        if (servoStates[i].isActive)
        {
            if (currentTime - servoStates[i].startTime >= SERVO_DURATION)
            {
                servoStates[i].isActive = false;
                servoStates[i].currentPosition = 0;
                servo[i].write(0);
            }
        }
    }
}

void checkButtons()
{
    unsigned long currentTime = millis();

    for (int i = 0; i < 4; i++)
    {
        static unsigned long lastButtonPressTime[4] = {0, 0, 0, 0};
        if (digitalRead(BUTTON_PINS[i]) == LOW)
        {
            if (currentTime - lastButtonPressTime[i] > DEBOUNCE_DELAY)
            {
                activateServo(i);
                lastButtonPressTime[i] = currentTime;
            }
        }
    }
}
