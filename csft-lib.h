#ifndef CSFT_WEB_DEVICE_LIB
#define CSFT_WEB_DEVICE_LIB

void csft_setup(char *name, void (*configure_wifisettings_parameters)());

void csft_loop();

void csft_web_request(String url, String name, String id, void (*process_response)(HTTPClient &http) = 0);

#endif