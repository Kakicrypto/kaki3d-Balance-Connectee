#include <Arduino.h>
#include <TFT_eSPI.h>
#include "logo_kaki3d.h"
#define TFT_PWR_EN 15
#define TFT_BACKLIGHT 38
TFT_eSPI tft;  // on crée l'objet écran

void setup() {
    pinMode(TFT_PWR_EN, OUTPUT);
    digitalWrite(TFT_PWR_EN, HIGH);
    delay(500);
    pinMode(TFT_BACKLIGHT, OUTPUT);
    digitalWrite(TFT_BACKLIGHT, HIGH); 
    tft.init();
    tft.setRotation(1);
    tft.pushImage(0, 0, LOGO_WIDTH, LOGO_HEIGHT, logo_kaki3d);
    delay(5000); // splash screen 3 secondes
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.setCursor(10, 10);
    tft.println("Hello Kaki3D !");
}

void loop() {}