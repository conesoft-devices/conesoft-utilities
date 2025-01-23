#include "csft-lib.h"

#include <DoubleResetDetector.h>
#include <ESP8266WiFi.h>
#include <WiFiSettings.h>
#include <LittleFS.h>

DoubleResetDetector drd(10, 0);

void csft_setup(char *name, void (*configure_wifisettings_parameters)()) {
    Serial.begin(115200);
    Serial.print("booting up ");
    Serial.println(name);
    pinMode(LED_BUILTIN, OUTPUT);
    LittleFS.begin();
    WiFi.setPhyMode(WIFI_PHY_MODE_11G);
    WiFi.forceSleepBegin();
    WiFi.setPhyMode(WIFI_PHY_MODE_11G);

    WiFiSettings.hostname = "csft-device-";
    configure_wifisettings_parameters();

    if (drd.detectDoubleReset()) {
    WiFiSettings.portal();
    }

    WiFiSettings.connect();
}

void csft_loop() {
    drd.loop();
}