#include "display.h"
#include "communication.h"

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
unsigned long lastUpdate = 0;

bool display_init()
{
    Wire.begin(ESP_SDA, ESP_SCL);
    
    if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("SSD1306 allocation failed"));
        return false;
    }
    
    display_showStartupMessage();
    return true;
}

void display_showStartupMessage()
{
    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(0, 0);
    oled.println("System Starting...");
    oled.display();
}

void display_checkDataTimeout()
{
    unsigned long currentTime = millis();

    if (robot1DataReceived && (currentTime - robot1LastReceived) > DATA_TIMEOUT)
    {
        robot1DataReceived = false;
        Serial.println("Robot 1 data timeout!");
    }
    if (robot2DataReceived && (currentTime - robot2LastReceived) > DATA_TIMEOUT)
    {
        robot2DataReceived = false;
        Serial.println("Robot 2 data timeout!");
    }
    if (winnerDataReceived && (currentTime - winnerLastReceived) > DATA_TIMEOUT)
    {
        winnerDataReceived = false;
        Serial.println("Winner data timeout!");
    }
}

void display_update()
{
    unsigned long currentTime = millis();
    
    if (currentTime - lastUpdate < DISPLAY_UPDATE_INTERVAL)
        return;
    
    lastUpdate = currentTime;
    display_checkDataTimeout();

    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(0, 0);

    oled.println("ROBOT RACE CONTROL");
    oled.println("------------------");

    oled.print("R1: ");
    if (robot1DataReceived)
    {
        oled.print("V:");
        oled.print(robot1Data.voltage / 1000.0, 1);
        oled.print("V I:");
        oled.print(robot1Data.current / 1000.0, 1);
        oled.println("A");
        oled.print("    Water:");
        oled.print(robot1Data.level / 100.0, 1);
        oled.print("cm P:");
        oled.print(robot1Data.pressure);
        oled.println("Pa");
    }
    else
    {
        oled.println("Waiting for data...");
    }

    oled.print("R2: ");
    if (robot2DataReceived)
    {
        oled.print("V:");
        oled.print(robot2Data.voltage / 1000.0, 1);
        oled.print("I: ");
        oled.print(robot2Data.current / 1000.0, 1);
        oled.println("A");
        oled.print("Water: ");
        oled.print(robot2Data.level / 100.0, 1);
        oled.print("cm P:");
        oled.print(robot2Data.pressure);
        oled.println("Pa");
    }
    else
    {
        oled.println("Waiting for data...");
    }

    oled.print("WINNER: ");
    if (winnerDataReceived)
    {
        if (gameResult.winner == 1)
            oled.println("ROBOT 1");
        else if (gameResult.winner == 2)
            oled.println("ROBOT 2");
        else
            oled.println("DRAW");
    }
    else
    {
        oled.println("Determining...");
    }

    oled.display();
}
