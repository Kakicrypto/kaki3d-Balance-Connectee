#include <Arduino.h>
#include <TFT_eSPI.h>
#include "logo_kaki3d.h"
#include <HX711.h>
#include <WiFiManager.h>
#define TFT_PWR_EN 15
#define TFT_BACKLIGHT 38
#define HX711_DT  1 // pin dt balance
#define HX711_SCK 2 // pin sck balance
TFT_eSPI tft;  // on crée l'objet écran
HX711 scale; // on crée l'objet balance
WiFiManager wm; // on crée l'objet wifi
bool wifi_connecte = false;
void setup() {
    pinMode(TFT_PWR_EN, OUTPUT);
    digitalWrite(TFT_PWR_EN, HIGH);
    delay(500);
    pinMode(TFT_BACKLIGHT, OUTPUT);
    digitalWrite(TFT_BACKLIGHT, HIGH);
    tft.init();
    tft.setRotation(3);
    tft.pushImage(0, 0, LOGO_WIDTH, LOGO_HEIGHT, logo_kaki3d);
    delay(3500);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.setCursor(10, 10);
    tft.println("Connexion WiFi...");
    tft.println("SSID: Kaki3D-Balance");
    wm.autoConnect("Kaki3D-Balance");
    scale.begin(HX711_DT, HX711_SCK);
    scale.tare();
}

void loop() {
    if (scale.is_ready()) {//boucle pour le calcul de la mediane 
        long readings[10];
        for (int i = 0; i < 10; i++) {
            readings[i] = scale.read_average(1);
            delay(5);
        }
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9-i; j++)
                if (readings[j] > readings[j+1]) {
                    long tmp = readings[j];
                    readings[j] = readings[j+1];
                    readings[j+1] = tmp;
                }
        long median = readings[5];
        float poids = (median - scale.get_offset()) / scale.get_scale();

        tft.fillRect(0, 0, 250, 30, TFT_BLACK); // affichage du poids
        tft.setTextColor(TFT_WHITE);
        tft.setTextSize(2);
        tft.setCursor(10, 10);
        tft.print("Poids: ");
        tft.print(poids, 1);
        tft.println(" g");
    } else {
        tft.fillRect(0, 0, 250, 30, TFT_BLACK); // message d'erreur si balance non connectée
        tft.setTextColor(TFT_RED);
        tft.setTextSize(2);
        tft.setCursor(10, 10);
        tft.println("HX711 non detecte");
    }
    delay(500);
    bool statut_actuel = (WiFi.status() == WL_CONNECTED); // affichage du logo Wifi
    if (statut_actuel != wifi_connecte) {
        wifi_connecte = statut_actuel;
        if (wifi_connecte) {
            tft.fillCircle(300, 30, 3, TFT_GREEN);
            tft.drawArc(300, 30, 10, 8, 130, 230, TFT_GREEN, TFT_BLACK);
            tft.drawArc(300, 30, 18, 15, 130, 230, TFT_GREEN, TFT_BLACK);
            tft.drawArc(300, 30, 26, 22, 130, 230, TFT_GREEN, TFT_BLACK);
        } else{
            tft.fillRect(0, 0, 250, 30, TFT_BLACK);// message d'erreur si Wifi non connectée
            tft.setTextColor(TFT_RED);
            tft.setTextSize(3);
            tft.setCursor(10, 10);
            tft.println("Wifi non connectée");
        }
}
}

