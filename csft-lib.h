#ifndef CSFT_WEB_DEVICE_LIB
#define CSFT_WEB_DEVICE_LIB

void csft_setup(char *name, void (*configure_wifisettings_parameters)());

void csft_loop();

#endif