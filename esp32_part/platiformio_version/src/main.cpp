#include <Arduino.h>
#include "wifi_connection.h"
#include "firebase_connection.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  initialize_wifi_connection(user, pass);
  start_fire_base_connection(host_firebase, pass_firebase);
}

void loop() {
  // put your main code here, to run repeatedly:
}