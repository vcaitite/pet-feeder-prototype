#include <WiFi.h>
#include "FirebaseESP32.h"

/* Declaracao das variaveis de conexao wifi */

char * host_firebase = "https://pet-feeder-17b78-default-rtdb.firebaseio.com/";
char * pass_firebase = "HhW2LuFKHcycLl4A2dD8Qrfcg10UjFt6QAUFC8o7";

const char * user = "MOL III";   //  modifique aqui com os dados do seu host Wi-Fi
const char * pass = "1288moradiaLI";

/* Declaracao das variaveis de controle do alimentador */

int servo = 36; // pino do servo motor (PWM)
int led_a = 39; // pino do led que indica que esta aguardando comandos
int led_b = 34; // pino do led que indica que esta contando o tempo
int swt_a = 12; // chave para alimentar instantaneamente
int swt_b = 14; // chave para alimentar de 4 em 4h
int swt_c = 27; // chave para alimentar de 6 em 6h
int swt_d = 26; // chave para alimentar de 8 em 8h
int swt_f = 25; // chave para cancelar as contagens de tempo
int tempo;      // realiza a contagem de tempo
int cancel;     // variavel auxiliar
String aux2;    // armazena o comando do app
char aux3='f';  // armazena o comando dos botoes

/* Funcao para inicializar a conexao com a internet com a rede e a senha */

void initialize_wifi_connection(const char* user, const char* password) {
  WiFi.begin(user, password);

  //Enquanto o status do WIFI for diferente de conectado, escreva na serial "Conectando ao WIFI..."
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Conectando ao WIFI..");
  }
  
  //Escreva na serial que o WIFI está conectado
  Serial.println("WIFI Conectado");  
} 

/* Funcao de leitura de comando no firebase e confirmacao */

String read_firebase_command() {
  String msg_status = "";
  msg_status = Firebase.getString("petFeeder/read_confirmation");
  if (msg_status.equals("sent") == true) {
    String command = "";
    command = Firebase.getString("petFeeder/command");
    while (command.equals("") == true) {
      command = Firebase.getString("petFeeder/command");
      delay(10);
    }
    // Enviar confirmação de leitura:
    Firebase.setString("petFeeder/read_confirmation", "received");
    while (Firebase.failed()) {
      Firebase.setString("petFeeder/read_confirmation", "received");
      delay(10);
    }
    return command;
  }
  return "s";
}

/* Funcao de controle do alimentador na hora de alimentar */

void alimentar() {
  digitalWrite(led_b, LOW);                     // indica que nao esta contando tempo
  Serial.println("Alimentando!");
  digitalWrite(servo, LOW);
  delay(1500);                                  // tempo que a comida cai
  digitalWrite(servo, HIGH);
}

/* setup: executado apenas uma vez */

void setup() {
  Serial.begin(115200);
  initialize_wifi_connection(user, pass);
  Firebase.begin(host_firebase, pass_firebase); 

  //entradas
  pinMode(swt_a, INPUT);
  pinMode(swt_b, INPUT);
  pinMode(swt_c, INPUT);
  pinMode(swt_d, INPUT);
  pinMode(swt_f, INPUT);

  //saidas
  pinMode(servo, OUTPUT);
  pinMode(led_a, OUTPUT);
  pinMode(led_b, OUTPUT);

  //inicializacoes
  digitalWrite(led_a, LOW);
  digitalWrite(led_b, LOW);
}

/* loop: executado indefinidamente */

void loop() {
  aux2 = read_firebase_command();       // armazena o comando do app
  Serial.println("command: " + aux2);   // imprime no serial o comando recebido
  delay(10);

  /* Leitura dos botoes fisicos */
  
  if (digitalRead(swt_a) == HIGH) {
    aux3 = 'a';
  }
  else if (digitalRead(swt_b) == HIGH) {
    aux3 = 'b';
  }
  else if (digitalRead(swt_c) == HIGH) {
    aux3 = 'c';
  }
  else if (digitalRead(swt_d) == HIGH) {
    aux3 = 'd';
  }

  /* Verifica se recebeu comando via app ou botao */
  
  if (aux2 != "a" && aux2 != "b" && aux2 != "c" && aux2 != "d" && aux3 != 'a' && aux3 != 'b' && aux3 != 'c' && aux3 != 'd') {
    Serial.println("Aguardando comando...");        // caso nao tenha recebido comando de alimentar ou contar tempo: stand-by
    digitalWrite(led_a, HIGH);                      // led_a representa o stand-by / aguardando comandos
  }
  
  else {                                            // caso tenha recebido comando de alimentar ou contar tempo
    digitalWrite(led_a, LOW);                       // desativa o led de stand-by
    Serial.println("Comando capturado!");

    /* Verifica qual comando recebeu e realiza a respectiva acao */
    
    if (aux2 == "b" || aux3 == 'b') {               // alimentar de 4 em 4h
      digitalWrite(led_b, HIGH);                    // led_b representa que esta contando tempo
      Serial.println("Contando 4h...");

      do {
        for (tempo = 0; tempo <= 167; tempo++) {    // 5 em 5s  <==>  4 em 4 horas (14400000ms)
          delay(10);
          aux2 = read_firebase_command();           // armazena o comando do app
          delay(10);
          if ((digitalRead(swt_f) == HIGH) || aux2 != "s") {      // se o comando do app for diferente de 's', houve alteracao, logo, cancela o timer
            delay(500);
            aux3 = 'f';                             // auxiliar dos botoes fisicos recebe a informacao de cancelamento de timer
            digitalWrite(led_b, LOW);
            delay(10);
            Serial.println("Cancelando!");
            break;
          }
          delay(10);
        } // for

        if (tempo == 168) {                         // se terminou de contar o tempo, ativa o alimentador
          alimentar();
          delay(10);
          digitalWrite(led_b, HIGH);                // led_b representa que voltou a contar o tempo
        }
      } while (aux3 != 'f');                        // repete a contagem e alimentacao ate cancelar
    } // if 'b'
    
    if (aux2 == "c" || aux3 == 'c') {               // alimentar de 6 em 6h
      digitalWrite(led_b, HIGH);                    // led_b representa que esta contando tempo
      Serial.println("Contando 6h...");

      do {
        for (tempo = 0; tempo <= 334; tempo++) {    // 10 em 10s  <==>  6 em 6 horas (21600000ms)
          delay(10);
          aux2 = read_firebase_command();           // armazena o comando do app
          delay(10);
          if ((digitalRead(swt_f) == HIGH) || aux2 != "s") {      // se o comando do app for diferente de 's', houve alteracao, logo, cancela o timer
            delay(500);
            aux3 = 'f';                             // auxiliar dos botoes fisicos recebe a informacao de cancelamento de timer
            digitalWrite(led_b, LOW);
            delay(10);
            Serial.println("Cancelando!");
            break;
          }
          delay(10);
        } // for

        if (tempo == 335) {                         // se terminou de contar o tempo, ativa o alimentador
          alimentar();
          delay(10);
          digitalWrite(led_b, HIGH);                // led_b representa que voltou a contar o tempo
        }        
      } while (aux3 != 'f');                        // repete a contagem e alimentacao ate cancelar
    } // if 'c'
    
    if (aux2 == "d" || aux3 == 'd') {               // alimentar de 8 em 8h
      digitalWrite(led_b, HIGH);                    // led_b representa que esta contando tempo
      Serial.println("Contando 8h...");

      do {
        for (tempo = 0; tempo <= 500; tempo++) {    // 15 em 15s  <==>  8 em 8 horas (28800000ms)
          delay(10);
          aux2 = read_firebase_command();           // armazena o comando do app
          delay(10);
          if ((digitalRead(swt_f) == HIGH) || aux2 != "s") {
            delay(500);
            aux3 = 'f';                             // auxiliar dos botoes fisicos recebe a informacao de cancelamento de timer
            digitalWrite(led_b, LOW);
            delay(10);
            Serial.println("Cancelando!");
            break;
          }
          delay(10);
        }

        if (tempo == 501) {                         // se terminou de contar o tempo, ativa o alimentador
          alimentar();
          delay(10);
          digitalWrite(led_b, HIGH);                // led_b representa que voltou a contar o tempo
        }
      } while (aux3 != 'f');                        // repete a contagem e alimentacao ate cancelar
    } // if 'd'
    
    if (aux2 == "a" || aux3 == 'a') {               // alimentar sem contar tempo
      alimentar();
      delay(10);
      aux3 = 'f';
    }
  }
}
