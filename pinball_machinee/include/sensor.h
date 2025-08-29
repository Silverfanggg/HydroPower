#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

const int LDR_PINS[6] = {34, 35, 32, 33, 25, 26};

const unsigned long LDR_THRESHOLD = 1500;
const int COLOR_THRESHOLD = 800;

struct HoleState
{
  int lastReading = 0;
  bool ballDetected = false;
  unsigned long lastDetectionTime = 0;
};

void initLDR();
void readLDR();
void processBallDetection(int holeIndex);
void checkGoals();

extern HoleState holes[6];

#endif

