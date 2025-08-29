#include "sensor.h"
#include "game.h"

const int BALL_DETECTION_DELAY = 100;

HoleState holes[6];

void initLDR()
{
    for (int i = 0; i < 6; i++)
    {
        holes[i].lastReading = 0;
        holes[i].ballDetected = false;
        holes[i].lastDetectionTime = 0;
    }
}

void readLDR()
{
    for (int i = 0; i < 6; i++)
    {
        int currentReading = analogRead(LDR_PINS[i]);
        unsigned long currentTime = millis();
        
        holes[i].lastReading = currentReading;
        bool lightBlocked = currentReading < LDR_THRESHOLD;
        
        if (lightBlocked && !holes[i].ballDetected) {
            if (currentTime - holes[i].lastDetectionTime > BALL_DETECTION_DELAY) {
                holes[i].ballDetected = true;
                holes[i].lastDetectionTime = currentTime;
                
                String ballColor = (currentReading > COLOR_THRESHOLD) ? "GRAY" : "BLACK";
                
                if (game.GameActive) {
                    processGoal(i, ballColor);
                }
            }
        } else if (!lightBlocked) {
            holes[i].ballDetected = false;
        }
    }
}

void processBallDetection(int holeIndex)
{
    if (!game.GameActive) return;
    
    unsigned long currentTime = millis();
    if (currentTime - holes[holeIndex].lastDetectionTime > BALL_DETECTION_DELAY)
    {
        String ballColor = (holes[holeIndex].lastReading > COLOR_THRESHOLD) ? "GRAY" : "BLACK";
        processGoal(holeIndex, ballColor);
        holes[holeIndex].lastDetectionTime = currentTime;
    }
}

void checkGoals()
{
    if (!game.GameActive) {
        return;
    }
    
    for (int i = 0; i < 6; i++)
    {
        int currentReading = analogRead(LDR_PINS[i]);
        if (currentReading < LDR_THRESHOLD) {
            processBallDetection(i);
        }
    }
}