#ifndef CONTROL_H
#define CONTROL_H

#include <Arduino.h>

const int BUTTON_PINS[] = {0, 2, 4, 5};
const int SERVO_PINS[] = {18, 19, 21, 22};

struct ServoState
{
    bool isActive = false;
    unsigned long startTime = 0;
    int currentPosition = 0;
};

void initButton();
void initServos();
void activateServo(int index);
void updateServos();
void checkButtons();

extern ServoState servoStates[4];


#endif
