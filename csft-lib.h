#ifndef CSFT_WEB_DEVICE_LIB
#define CSFT_WEB_DEVICE_LIB

#include <stdlib.h>
#include <WiFiSettings.h>
#include <ESP8266HTTPClient.h>


void csft_setup(String name, void (*configure_wifisettings_parameters)());

void csft_loop();
void csft_loop_for(int milliseconds);

void csft_web_request(String url, String name, void (*process_response)(HTTPClient &http) = 0);
void csft_web_request(String url, String name, String id_suffix, void (*process_response)(HTTPClient &http) = 0);

void csft_binary_read_response_to(HTTPClient& http, uint8_t *target, int size);

#endif