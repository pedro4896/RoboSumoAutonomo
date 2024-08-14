#include "Ultrasonic.h" // Biblioteca para controle do sensor ultrassônico

#define echoPin 4 // Pino ECHO do sensor HC-SR04
#define trigPin 5 // Pino TRIG do sensor HC-SR04
Ultrasonic ultrasonic(trigPin, echoPin); // Inicialização do sensor ultrassônico

#define sensorDianteiro 2 // Sensor de linha frontal
#define sensorTraseiro 3  // Sensor de linha traseiro

// Definição dos pinos dos relés
#define rele4 6
#define rele3 7
#define rele2 8
#define rele1 9

#define led 13 // LED indicador

int distancia; // Armazena a distância medida pelo sensor ultrassônico
String result; // Armazena a distância como string

unsigned long tempoAnterior = 0; // Variável para armazenar o tempo anterior
const unsigned long intervalo = 1000; // Intervalo de 200ms para as funções de movimento

volatile bool linhaBranca = false; // Flag para detectar linha branca

void setup() {
  // Configuração dos pinos
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(sensorDianteiro, INPUT);
  pinMode(sensorTraseiro, INPUT);
  pinMode(rele1, OUTPUT);
  pinMode(rele2, OUTPUT);
  pinMode(rele3, OUTPUT);
  pinMode(rele4, OUTPUT);
  //pinMode(led, OUTPUT);

  //digitalWrite(led, HIGH); // Liga o LED

  Serial.begin(9600); // Inicia comunicação serial

  stop(); // Garante que os motores estejam parados

  // Configuração das interrupções
  attachInterrupt(digitalPinToInterrupt(sensorDianteiro), linhaFrente, CHANGE);
  attachInterrupt(digitalPinToInterrupt(sensorTraseiro), linhaTras, CHANGE);
}

void loop() {
  // Se o robô estiver na linha branca, a lógica do ultrassônico é paralisada
  if (linhaBranca == false) {
    // Leitura dos sensores de linha
    /*Serial.print("Sensor Frente:");
    Serial.println(digitalRead(sensorDianteiro));
    Serial.print("Sensor Tras:");
    Serial.println(digitalRead(sensorTraseiro));*/

    // Medição da distância
    hcsr04(); 
    /*Serial.print("Distancia ");
    Serial.print(distancia);
    Serial.println("cm");*/

    // Movimentação com base na distância
    if ((distancia > 1 && distancia < 50)) {
      movimento_frente();
      Serial.println("frente");
    } else {
      movimento_frente();
      if (millis() - tempoAnterior >= intervalo) { // Verifica se o intervalo já passou
        Serial.println("procura");
        movimento_esquerda();
        tempoAnterior = millis(); // Atualiza o tempo anterior
      }
    }
  }
}

// Função para medir a distância com o sensor ultrassônico
void hcsr04() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  distancia = (ultrasonic.Ranging(CM));
  result = String(distancia);
  Serial.println(distancia);
}

// Função para mover o robô para a esquerda
void movimento_esquerda() {
  digitalWrite(rele1, 1);
  digitalWrite(rele2, 0);
  digitalWrite(rele3, 0);
  digitalWrite(rele4, 1);
  unsigned long tempoInicio = millis();
  while (millis() - tempoInicio < 300); // Aguarda 300ms sem bloquear o loop principal
  stop();
}

// Função para mover o robô para a direita

void procura(){
  movimento_frente();
  unsigned long tempoInicio = millis();
  while (millis() - tempoInicio < 300); // Aguarda 300ms sem bloquear o loop principal
  movimento_esquerda();
  tempoInicio = millis();
  while (millis() - tempoInicio < 300); // Aguarda 300ms sem bloquear o loop principal
  stop();
}
void movimento_direita() {
  digitalWrite(rele1, 1);
  digitalWrite(rele2, 0);
  digitalWrite(rele3, 0);
  digitalWrite(rele4, 1);
  unsigned long tempoInicio = millis();
  while (millis() - tempoInicio < 300); // Aguarda 300ms sem bloquear o loop principal
  stop();
}

// Função para mover o robô para frente
void movimento_frente() {
  digitalWrite(rele1, 1);
  digitalWrite(rele2, 0);
  digitalWrite(rele3, 1);
  digitalWrite(rele4, 0);
  unsigned long tempoInicio = millis();
  while (millis() - tempoInicio < 300); // Aguarda 300ms sem bloquear o loop principal
  stop();
}

// Função para mover o robô para trás
void movimento_tras() {
  digitalWrite(rele1, 0);
  digitalWrite(rele2, 1);
  digitalWrite(rele3, 0);
  digitalWrite(rele4, 1);
}

// Função para parar o movimento dos motores
void stop() {
  digitalWrite(rele1, 1);
  digitalWrite(rele2, 1);
  digitalWrite(rele3, 1);
  digitalWrite(rele4, 1);
}

// Interrupção para o sensor dianteiro
void linhaFrente() {
  if (digitalRead(sensorDianteiro) == 1) { // Detecta linha branca
    Serial.println("Para Trás");
    linhaBranca = true;
    movimento_tras();
  } else { // Detecta linha preta
    linhaBranca = false;
  }
}

// Interrupção para o sensor traseiro
void linhaTras() {
  if (digitalRead(sensorTraseiro) == 1) { // Detecta linha branca
    Serial.println("Para Frente");
    linhaBranca = true;
    movimento_frente();
  } else { // Detecta linha preta
    linhaBranca = false;
  }
}
