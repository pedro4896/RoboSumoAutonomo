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
const unsigned long intervalo = 1000; // Intervalo de 1000ms para as funções de movimento

unsigned long tempoMovimento = 0; // Variável para armazenar o tempo de movimento
bool movendo = false; // Flag para indicar se o robô está se movendo
bool linhaBranca = false; // Flag para indicar se o robô está sobre a linha branca

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
  pinMode(led, OUTPUT);

  Serial.begin(9600); // Inicia comunicação serial

  stop(); // Garante que os motores estejam parados

  // Configuração das interrupções
  attachInterrupt(digitalPinToInterrupt(sensorDianteiro), linhaFrente, CHANGE);
  attachInterrupt(digitalPinToInterrupt(sensorTraseiro), linhaTras, CHANGE);
}

void loop() {
  // Se o robô não está na linha branca
  if (!linhaBranca) {
    // Medição da distância
    hcsr04();

    // Movimentação com base na distância
    if ((distancia > 1 && distancia < 50)) {
      movimento_frente();
      Serial.println("frente");
    } else {
      if (millis() - tempoAnterior >= intervalo) {
        Serial.println("procura");
        movimento_esquerda();
        tempoAnterior = millis(); // Atualiza o tempo anterior
      }
    }
  }

  // Verifica se o tempo de movimentação já passou
  if (movendo && millis() - tempoMovimento >= 300) {
    stop();
    movendo = false; // Marca que o robô parou
  }
}

// Função para medir a distância com o sensor ultrassônico
void hcsr04() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  distancia = ultrasonic.Ranging(CM);
  Serial.println(distancia);
}

// Função para mover o robô para a esquerda
void movimento_esquerda() {
  digitalWrite(rele1, 1);
  digitalWrite(rele2, 0);
  digitalWrite(rele3, 0);
  digitalWrite(rele4, 1);

  tempoMovimento = millis(); // Define o início do movimento
  movendo = true; // Marca que o robô está se movendo
}

// Função para mover o robô para a direita
void movimento_direita() {
  digitalWrite(rele1, 1);
  digitalWrite(rele2, 0);
  digitalWrite(rele3, 0);
  digitalWrite(rele4, 1);

  tempoMovimento = millis(); // Define o início do movimento
  movendo = true; // Marca que o robô está se movendo
}

// Função para mover o robô para frente
void movimento_frente() {
  digitalWrite(rele1, 1);
  digitalWrite(rele2, 0);
  digitalWrite(rele3, 1);
  digitalWrite(rele4, 0);

  tempoMovimento = millis(); // Define o início do movimento
  movendo = true; // Marca que o robô está se movendo
}

// Função para mover o robô para trás
void movimento_tras() {
  digitalWrite(rele1, 0);
  digitalWrite(rele2, 1);
  digitalWrite(rele3, 0);
  digitalWrite(rele4, 1);

  tempoMovimento = millis(); // Define o início do movimento
  movendo = true; // Marca que o robô está se movendo
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
