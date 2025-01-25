#ifndef CSFT_WEB_DEVICE_LIB
#define CSFT_WEB_DEVICE_LIB

#include <stdlib.h>
#include <WiFiSettings.h>
#include <ESP8266HTTPClient.h>


void csft_setup(String name, void (*configure_wifisettings_parameters)());

void csft_loop();
void csft_loop_for(int milliseconds);

bool csft_web_request(String url, String name, bool (*process_response)(HTTPClient &http) = 0);
bool csft_web_request(String url, String name, String id_suffix, bool (*process_response)(HTTPClient &http) = 0);

bool csft_binary_read_response_to(HTTPClient& http, uint8_t *target, size_t size);

#endif