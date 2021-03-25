#include "firebase_connection.h"

char * host_firebase = "https://pet-feeder-17b78-default-rtdb.firebaseio.com/";
char * pass_firebase = "HhW2LuFKHcycLl4A2dD8Qrfcg10UjFt6QAUFC8o7";

void start_fire_base_connection(char * host_firebase, char * pass_firebase){
    Firebase.begin(host_firebase, pass_firebase); 
    Firebase.setFloat("petFeeder/command", 1.0);
}