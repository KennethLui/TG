/*
  Leitor de temperatura (DHT22)
  Autores: Pedro Bertoleti e Douglas Zuqueto
  Data: Dezembro/2015

  Este é o programa de leitura de tempertura (do sensor DHT22).
  O programa le a temperatura em um periodo de 10 segundos e mostra,
  no Serial Monitor, a temperatura lida.
*/
#include "DHT.h"
#include <SoftwareSerial.h> // Biblioteca SoftwareSerial

//defines gerais
#define DHTPIN                A1      //pino do Arduino que está conectado ao pino de dados do sensor
#define DHTTYPE               DHT11  //estabelece que o sensor utilizado é o DHT22 (é possível selecionar entre DHT11, DHT22 e DHT21)
#define TEMPO_LEITURA_SENSOR  2000  //tempo entre leituras de temperatura

//variáveis globais
DHT dht(DHTPIN, DHTTYPE);   //objeto para comunicar com sensor de temperatura DHT22
SoftwareSerial comunicacaoESP(2, 3); // objeto para comunicação serial com o ESP8266 nas Portas 2(RX) e 3(TX)

//Função setup (inicializações do programa)
void setup()
{
  Serial.begin(9600);
  Serial.println("======Leitor de temperatura======");
  comunicacaoESP.begin(9600); // inicia a comunicação serial com o ESP8266

  //inicia a comunicação com o sensor DHT22
  dht.begin();
}

//loop (programa principal)
void loop()
{
  float TemperaturaLida;
  String str;

  //le a temperatura (em graus Celsius)
  TemperaturaLida = dht.readTemperature();

  //checa se a leitura da temperatura foi bem sucedida ou não
  if (isnan(TemperaturaLida)) {
    Serial.println("- Falha ao ler a temperatura medida pelo sensor DHT22");
    comunicacaoESP.println('0'); // caso o valor seja nulo ou indefinido, o valor '0' é enviado para o ESP
  } else
  {
    Serial.print("- Temperatura: ");
    Serial.print(TemperaturaLida);
    Serial.println("C");

    str = String(TemperaturaLida,1);
    
    comunicacaoESP.print(TemperaturaLida); // envio do valor da temperatura para o módulo ESP
  }

  delay(TEMPO_LEITURA_SENSOR);

}
