#ifndef  WIFI_CONNECTION_H
#define  WIFI_CONNECTION_H

#include <Arduino.h>
#include <WiFi.h>

extern const char * user;
extern const char * pass;

void initialize_wifi_connection(const char* user, const char* password);

#endif