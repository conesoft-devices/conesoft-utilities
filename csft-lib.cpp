#include "csft-lib.h"

#include <DoubleResetDetector.h>
#include <LittleFS.h>

DoubleResetDetector drd(10, 0);

void csft_setup(String name, void (*configure_wifisettings_parameters)())
{
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

    if (drd.detectDoubleReset())
    {
        WiFiSettings.portal();
    }

    WiFiSettings.connect();
}

void csft_loop()
{
    drd.loop();
}

void csft_web_request(String url, String name, String id, void (*process_response)(HTTPClient &http))
{
    WiFiClient wificlient;
    HTTPClient http;

    if (http.begin(wificlient, url))
    {
        http.setUserAgent(name);
        http.addHeader(name + "-Id", id);
        http.GET();
        if (process_response != 0)
        {
            process_response(http);
        }
        http.end();
    }
}