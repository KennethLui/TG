#include <ESP8266WiFi.h> // Importa a Biblioteca ESP8266WiFi
#include <PubSubClient.h> // Importa a Biblioteca PubSubClient

// WIFI
const char* SSID = "NASA"; // SSID da Rede
const char* PASSWORD = "02303970"; // Senha da Rede

// MQTT
const char* BROKER_MQTT = "192.168.1.106"; // IP/URL DO BROKER MQTT
//const char* BROKER_MQTT = "test.mosquitto.org"; // IP/URL DO BROKER MQTT
int BROKER_PORT = 1883; // Porta do Broker MQTT

// COORDENADAS
// Importante: substituir estas coordenadas pelas coordenadas geográficas da sua estação meteorológica
#define LAT "-15.8238781"
#define LONG "-48.062814499999995"
#define ID_MQTT  "dht11_sensor1" //id mqtt definido como as coordenada geográficas para garantir unicidade
String dado = "55.9";

WiFiClient espClient; // Cria o objeto espClient

String temp = "0";

PubSubClient MQTT(espClient); // Instancia o Cliente MQTT passando o objeto espClient

// Declaração das funções utilizadas no decorrer do Firmware
void initSerial();
void initWiFi();
void initMQTT();
void mqtt_callback(char* topic, byte* payload, unsigned int length);


void setup() {
  initSerial();
  initWiFi();
  initMQTT();
}

// Função para iniciar a comunicação Serial
void initSerial() {
  Serial.begin(115200);
}

// Função para iniciar a Conexão com a rede WiFi
void initWiFi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando-se em: ");
  Serial.println(SSID);

  WiFi.begin(SSID, PASSWORD); // Conecta na Rede Wireless
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Conectado na Rede ");
  Serial.print(SSID);
  Serial.println(" | IP ");
  Serial.println(WiFi.localIP());
}

// Funcão para se conectar ao Broker MQTT
void initMQTT() {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT); // Atribui a biblioteca MQTT o Broker MQTT
  MQTT.setCallback(mqtt_callback); // tribui a biblioteca MQTT o callback
}

//Função que recebe as mensagens publicadas
void mqtt_callback(char* topic, byte* payload, unsigned int length) {

  String message;
  for (int i = 0; i < length; i++) {
    char c = (char)payload[i];
    message += c;
  }
  Serial.print("Topico ");
  Serial.print(topic);
  Serial.print(" | ");
  Serial.println(message);
  message = "";
}

// Função para se reconectar ao Broker MQTT caso cai'a a comunicação
void reconnectMQTT() {
  while (!MQTT.connected()) {
    Serial.print("Tentando se conectar ao Broker MQTT: ");
    Serial.println(BROKER_MQTT);
    if (MQTT.connect(ID_MQTT)) {
      Serial.println("Conectado");
      MQTT.subscribe("tg/temperature/dht11"); // Assina o tópico estação/lat/long

    } else {
      Serial.println("Falha ao Reconectar");
      Serial.println("Tentando se reconectar em 2 segundos");
      delay(2000);
    }
  }
}

// Função para se reconectar a rede WiFi caso caia a comunicação com a rede
void recconectWiFi() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
}
void loop() {
  if (!MQTT.connected()) {
    reconnectMQTT(); // Caso o ESP se desconecte do Broker, ele tenta se reconectar ao Broker
  }
  recconectWiFi(); // Caso o ESP perca conexão com a rede WiFi, ele tenta se reconetar a rede.

  // Variável que irá guardar as coordenadas e a temperatura para posteriormente serem tratadas e enviadas.
  //String payload = LAT;
  String payload = dado;
  //payload += ",";
  //payload += LONG;
  //payload += ",";
  //payload += temp;
  
  //dado=dado+1;

  char payloadChar[payload.length()];
  payload.toCharArray(payloadChar, payload.length() + 1); // Converte a String para Char Array
  // Publica no tópico EMCOL todas as informações contidas na variável payload.
  MQTT.publish("tg/temperature/dht11", payloadChar);
  delay(1000);
  MQTT.loop();
}
