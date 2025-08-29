#ifndef SENSOR_H
#define SENSOR_H

#include "config.h"

void sensor_init();
void sensor_readAndSendPotValues();
int sensor_readPot1();
int sensor_readPot2();

#endif
