#ifndef CSFT_WEB_DEVICE_LIB
#define CSFT_WEB_DEVICE_LIB

#include <stdlib.h>
#include <ESP8266WiFi.h>
#include <WiFiSettings.h>
#include <ESP8266HTTPClient.h>


void csft_setup(String name, void (*configure_wifisettings_parameters)());

void csft_loop();

void csft_web_request(String url, String name, String id, void (*process_response)(HTTPClient &http) = 0);

#endif