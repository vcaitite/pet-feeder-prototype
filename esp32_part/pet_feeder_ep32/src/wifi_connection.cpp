#include "wifi_connection.h"

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