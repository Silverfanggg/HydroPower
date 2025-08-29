#include "nunchuck_servo.h"
#include <math.h>

#define ELECTROMAGNET_PIN 2

const int ESP_SDA = 5;
const int ESP_SCL = 6;
const int servoPinNumbers[3] = {9, 10, 11};
const int SERVO_MIN = 0;
const int SERVO_MAX = 180;

int deadzone = 10;
int centerX = 128;
int centerY = 128;

float alpha = 0.25f;
float servo1_filter = 90.0f;
float servo2_filter = 90.0f;
float servo3_filter = 90.0f;

const long INTERVAL = 100;

void initNunchuck(Accessory &nunchuck, Servo *servos)
{
    Wire.begin(ESP_SDA, ESP_SCL);

    pinMode(ELECTROMAGNET_PIN, OUTPUT);
    digitalWrite(ELECTROMAGNET_PIN, LOW);

    for (int i = 0; i < 3; i++)
    {
        servos[i].attach(servoPinNumbers[i]);
    }

    nunchuck.begin();
    nunchuck.readData();

    centerX = nunchuck.getJoyX();
    centerY = nunchuck.getJoyY();
}

int mappingJoystick(uint8_t v, int center, int dz)
{
    int val = (int)v;
    if (abs(val - center) < dz)
    {
        val = center;
    }

    return map(val, 0, 255, SERVO_MIN, SERVO_MAX);
}

void updateNunchuck(Accessory &nunchuck, Servo *servos)
{
    static unsigned long previousMillis = 0;
    if (millis() - previousMillis < (unsigned long)INTERVAL)
    {
        return;
    }
    previousMillis = millis();

    nunchuck.readData();

    uint8_t jx = nunchuck.getJoyX();
    uint8_t jy = nunchuck.getJoyY();
    int ax = nunchuck.getAccelX();
    int ay = nunchuck.getAccelY();
    int az = nunchuck.getAccelZ();
    bool cButton = nunchuck.getButtonC();
    bool zButton = nunchuck.getButtonZ();

    int servo1 = (int)servo1_filter;
    int servo2 = (int)servo2_filter;
    int servo3 = (int)servo3_filter;

    if (cButton)
    {
        float pitchDeg = atan2((float)-ax, sqrt((float)ay * ay + (float)az * az)) * 180.0f / PI;
        servo3 = map((int)pitchDeg, -90, 90, SERVO_MIN, SERVO_MAX);
        servo3 = constrain(servo3, SERVO_MIN, SERVO_MAX);
    }
    else
    {
        servo1 = mappingJoystick(jx, centerX, deadzone);
        servo2 = mappingJoystick(jy, centerY, deadzone);
        servo1 = constrain(servo1, SERVO_MIN, SERVO_MAX);
        servo2 = constrain(servo2, SERVO_MIN, SERVO_MAX);
    }

    servo1_filter += alpha * (servo1 - servo1_filter);
    servo2_filter += alpha * (servo2 - servo2_filter);
    servo3_filter += alpha * (servo3 - servo3_filter);

    servos[0].write((int)servo1_filter);
    servos[1].write((int)servo2_filter);
    servos[2].write((int)servo3_filter);

    digitalWrite(ELECTROMAGNET_PIN, zButton ? HIGH : LOW);
}