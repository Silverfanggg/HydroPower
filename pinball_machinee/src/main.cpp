#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include "control.h"
#include "game.h"
#include "sensor.h"
#include "esp_now_comm.h"


void setup()
{
    Serial.begin(115200);
    
    initButton();
    initServos();
    initLDR();
    initESPNow();
    
    game.GameActive = true;
    game.gameStartTime = millis();
}

void loop()
{
    checkButtons();
    updateServos();
    readLDR();
}
