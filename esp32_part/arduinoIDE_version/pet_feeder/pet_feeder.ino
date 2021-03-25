#include <WiFi.h>
#include "FirebaseESP32.h"

char * host_firebase = "https://pet-feeder-17b78-default-rtdb.firebaseio.com/";
char * pass_firebase = "HhW2LuFKHcycLl4A2dD8Qrfcg10UjFt6QAUFC8o7";

/*  Modifique aqui com os dados do seu host Wi-Fi */
const char * user = "MOL III";
const char * pass = "1288moradiaLI";

void initialize_wifi_connection(const char* user, const char* password){
    //Inicializa a conexão com a internet com a rede e a senha
    WiFi.begin(user, password);

    //Enquanto o status do WIFI for diferente de conectado, escreva na serial "Conectando ao WIFI..."
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.println("Conectando ao WIFI..");
    }
    
    //Escreva na serial que o WIFI está conectado
    Serial.println("WIFI Conectado");  
} 

String read_firebase_command(){
  String command = Firebase.getString("petFeeder/command");
  // Enviar confirmação de leitura:
  Firebase.setString("petFeeder/read_confirmation", "received");
  while (Firebase.failed()){
    Firebase.setString("petFeeder/read_confirmation", "received");
  }
  return command;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  initialize_wifi_connection(user, pass);
  Firebase.begin(host_firebase, pass_firebase); 
}
  
void loop() {
  String aux3 = read_firebase_command();
  Serial.println("command: " + aux3);
}
