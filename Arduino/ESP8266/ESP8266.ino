#include <ESP8266WiFi.h> // Importa a Biblioteca ESP8266WiFi

// WIFI
const char* SSID = "NASA"; // SSID da Rede
const char* PASSWORD = "02303970"; // Senha da Rede

// Prototypes das funções utilizadas no decorrer do Firmware
void initWiFi();


void setup() {
  Serial.begin(115200);
  initWiFi();
}

// Função para iniciar a Conexão com a rede WiFi
void initWiFi() {
  delay(1000);
  Serial.println();
  Serial.print("Conectando-se em: ");
  Serial.println(SSID);

  WiFi.begin(SSID, PASSWORD); // Conecta na Rede Wireless
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print("._.");
  }
  Serial.println();
  Serial.print("Conectado na Rede ");
  Serial.print(SSID);
  Serial.print(" | IP ");
  Serial.println(WiFi.localIP()); // Mostra o IP que foi atribuido para o ESP8266
}

void reconectar() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".|");
  }
}
void loop() {
  reconectar(); // Caso o ESP perca conexão com a rede WiFi, ele tenta se reconectar a rede.
  Serial.print("Conectado na Rede: ");
  Serial.println(SSID);
  delay(1000);
}
