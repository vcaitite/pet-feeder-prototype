#include <Arduino.h>
#include "wifi_connection.h"


const char * user = "MOL III";
const char * pass = "1288moradiaLI";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  initialize_wifi_connection(user, pass);
}

void loop() {
  // put your main code here, to run repeatedly:
}