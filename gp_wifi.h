#ifndef GP_WIFI_H
#define GP_WIFI_H

#include <WiFi101.h>

extern uint8_t last_wifi_status;

void wifi_init();
void wifi_loop();
void wifi_update_timeout();

#endif
