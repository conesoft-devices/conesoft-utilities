#include "csft-lib.h"

#include <DoubleResetDetector.h>
#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecureBearSSL.h>

DoubleResetDetector drd(10, 0);

void csft_setup(String name, void (*configure_wifisettings_parameters)())
{
    Serial.begin(74880);
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.print("booting up ");
    Serial.println(name);
    pinMode(LED_BUILTIN, OUTPUT);
    LittleFS.begin();
    WiFi.setPhyMode(WIFI_PHY_MODE_11G);

    WiFiSettings.hostname = "csft-device-";
    configure_wifisettings_parameters();

    if (drd.detectDoubleReset())
    {
        Serial.println("reset detected");
        unsigned long now = millis();
        WiFiSettings.onPortalWaitLoop = [&]() -> void
        {
            csft_loop();
            if (millis() > now + 30 * 1000)
            {
                ESP.restart();
            }
        };
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

bool csft_web_request_internal(String url, String name, String id, bool (*process_response)(HTTPClient &http))
{
    BearSSL::WiFiClientSecure client;
    HTTPClient http;

    client.setInsecure();

    Serial.print("GET ");
    Serial.print(url);

    if (http.begin(client, url))
    {
        http.setUserAgent(name);
        http.addHeader(name + "-Id", id);
        int response = http.GET();

        Serial.print(" => ");
        Serial.print(response);

        if (response >= 0 && response < 400)
        {
            if (process_response != 0)
            {
                if (process_response(http) == false)
                {
                    Serial.println(" ERROR processing");
                    http.end();
                    return false;
                }
            }
            Serial.println(" OK");
            http.end();
            return true;
        }
        Serial.println(" ERROR response");
        http.end();
        return false;
    }
    Serial.println(" ERROR creating request");
    return false;
}

bool csft_web_request(String url, String name, bool (*process_response)(HTTPClient &http))
{
    return csft_web_request_internal(url, name, WiFi.macAddress(), process_response);
}
bool csft_web_request(String url, String name, String id_suffix, bool (*process_response)(HTTPClient &http))
{
    return csft_web_request_internal(url, name, WiFi.macAddress() + "-" + id_suffix, process_response);
}

bool csft_binary_read_response_to(HTTPClient &http, uint8_t *target, size_t size)
{
    Stream &client = http.getStream();
    size_t length = http.getSize();
    Serial.print(" (length = ");
    Serial.print(length);
    Serial.print(")");
    if (length != size)
    {
        return false;
    }
    for (size_t index = 0; index < length; index += 0)
    {
        index += client.readBytes(target + index, size - index);
    }
    return true;
}