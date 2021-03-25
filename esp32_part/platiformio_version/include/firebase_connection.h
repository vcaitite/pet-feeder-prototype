#ifndef  FIREBASE_CONNECTION_H
#define  FIREBASE_CONNECTION_H

#include <Arduino.h>
#include <WiFi.h>
#include "FirebaseESP32.h"

extern char * host_firebase;
extern char * pass_firebase;

void start_fire_base_connection(char * host_firebase, char * pass_firebase);

#endif