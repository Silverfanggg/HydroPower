#include "sensor.h"
#include "communication.h"

unsigned long lastReadTime = 0;

void sensor_init()
{
    pinMode(POT_PIN, INPUT);
    pinMode(POT_PIN2, INPUT);
}

int sensor_readPot1()
{
    return analogRead(POT_PIN);
}

int sensor_readPot2()
{
    return analogRead(POT_PIN2);
}

void sensor_readAndSendPotValues()
{
    unsigned long currentTime = millis();
    
    if (currentTime - lastReadTime < SEND_INTERVAL)
        return;
    
    lastReadTime = currentTime;

    int potValue1 = sensor_readPot1();
    esp_err_t result1 = communication_sendPotValue(R1_MAC_ADDRESS, 1, potValue1);

    int potValue2 = sensor_readPot2();
    esp_err_t result2 = communication_sendPotValue(R2_MAC_ADDRESS, 2, potValue2);

    Serial.print("Pot1: ");
    Serial.print(potValue1);
    Serial.print(" -> Robot1");
    if (result1 != ESP_OK)
        Serial.print(" (ERROR!)");
    Serial.print(", Pot2: ");
    Serial.print(potValue2);
    Serial.print(" -> Robot2");
    if (result2 != ESP_OK)
        Serial.print(" (ERROR!)");
    Serial.println();
}
