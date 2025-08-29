#ifndef DISPLAY_H
#define DISPLAY_H

#include "config.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

bool display_init();
void display_update();
void display_showStartupMessage();
void display_checkDataTimeout();

#endif
