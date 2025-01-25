#include "csft-lib.h"

#include <DoubleResetDetector.h>
#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecureBearSSL.h>

DoubleResetDetector drd(10, 0);

void csft_setup(String name, void (*configure_wifisettings_parameters)())
{
    Serial.begin(115200);
    Serial.print("booting up ");
    Serial.println(name);
    pinMode(LED_BUILTIN, OUTPUT);
    LittleFS.begin();
    WiFi.forceSleepBegin();
    WiFi.setPhyMode(WIFI_PHY_MODE_11G);

    WiFiSettings.hostname = "csft-device-";
    configure_wifisettings_parameters();

    if (drd.detectDoubleReset())
    {
        WiFiSettings.portal();
    }

    if (WiFiSettings.connect(false) == false)
    {
        ESP.restart();
    }
}

void csft_loop()
{
    drd.loop();
}

void csft_loop_for(int milliseconds)
{
    unsigned long now = millis();
    while (millis() < now + milliseconds)
    {
        delay(10);
        csft_loop();
    }
}

void csft_web_request_internal(String url, String name, String id, void (*process_response)(HTTPClient &http))
{
    BearSSL::WiFiClientSecure client;
    HTTPClient http;

    client.setInsecure();

    if (http.begin(client, url))
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

void csft_web_request(String url, String name, void (*process_response)(HTTPClient &http))
{
    csft_web_request_internal(url, name, WiFi.macAddress(), process_response);
}
void csft_web_request(String url, String name, String id_suffix, void (*process_response)(HTTPClient &http))
{
    csft_web_request_internal(url, name, WiFi.macAddress() + "-" + id_suffix, process_response);
}

void csft_binary_read_response_to(HTTPClient &http, uint8_t *target, int size)
{
    Stream& client = http.getStream();
    size_t length = http.getSize();
    for (size_t index = 0; index < length; index += 0)
    {
        index += client.readBytes(target + index, size - index);
    }
}