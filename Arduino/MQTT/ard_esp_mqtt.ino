#include <ESP8266WiFi.h> // Importa a Biblioteca ESP8266WiFi
#include <PubSubClient.h> // Importa a Biblioteca PubSubClient

// WIFI
const char* SSID = "NASA"; // SSID da Rede
const char* PASSWORD = "02303970"; // Senha da Rede

// MQTT
const char* BROKER_MQTT = "192.168.1.106"; // IP/URL DO BROKER MQTT
int BROKER_PORT = 1883; // Porta do Broker MQTT


// COORDENADAS
// Importante: substituir estas coordenadas pelas coordenadas geográficas da sua estação meteorológica
//#define LAT "-29.1838681"
//#define LONG "-54.8528838"
#define ID_MQTT  "dht11_sensor2" //id mqtt definido como as coordenada geográficas para garantir unicidade
//String teste = "65.3";

WiFiClient espClient; // Cria o objeto espClient

PubSubClient MQTT(espClient); // Instancia o Cliente MQTT passando o objeto espClient

// Prototypes das funções utilizadas no decorrer do Firmware
void readData();
void initSerial();
void initWiFi();
void initMQTT();

void setup() {
  initSerial();
  initWiFi();
  initMQTT();
}
// Função para iniciar a comunicação Serial
void initSerial() {
  Serial.begin(9600);
}
// Função para iniciar a Conexão com a rede WiFi
void initWiFi() {
  WiFi.begin(SSID, PASSWORD); // Conecta na Rede Wireless
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
}
// Funcão para se conectar ao Broker MQTT
void initMQTT() {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT); // Informa o broker e porta utilizados
}

// Função para se reconectar ao Broker MQTT caso cair a comunicação
void reconnectMQTT() {
  while (!MQTT.connected()) {
    if (MQTT.connect(ID_MQTT)) {
    } else {
      delay(2000);
    }
  }
}

// Função para se reconectar a rede WiFi caso caia a comunicação com a rede
void recconectWiFi() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
}
void readData() {
   if (Serial.available()) {
    String temp = "";
    while (Serial.available()) {
      char c = (char)Serial.read();
      if (c != '\n') {
        temp += c;
      }
    delay(10);
    }

    //String payload = teste;
    //delay(1000);
    String payload = temp;
    
    //String payload = LAT;
    //payload += ",";
    //payload += LONG;
    //payload += ",";
    //payload += temp;

    char payloadChar[payload.length()];
    payload.toCharArray(payloadChar, payload.length() + 1); // Converte a String para Char Array
    // Publica no tópico EMCOL todas as informações contidas na variável payload.
    MQTT.publish("tg/temperature/dht11", payloadChar);
  }
}

void loop() {
  if (!MQTT.connected()) {
    reconnectMQTT(); // Caso o ESP se desconecte do Broker, ele tenta se reconectar ao Broker
  }
  recconectWiFi(); // Caso o ESP perca conexão com a rede WiFi, ele tenta se reconetar a rede.
  readData();
  MQTT.loop();
}
