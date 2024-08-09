#include "Ultrasonic.h" //INCLUSÃO DA BIBLIOTECA NECESSÁRIA PARA FUNCIONAMENTO DO CÓDIGO

#define echoPin 4 //PINO DIGITAL UTILIZADO PELO HC-SR04 ECHO(RECEBE)
#define trigPin 5 //PINO DIGITAL UTILIZADO PELO HC-SR04 TRIG(ENVIA)
Ultrasonic ultrasonic(trigPin,echoPin); //INICIALIZANDO OS PINOS DO ARDUINO

#define sensorDianteiro 2
#define sensorTraseiro 3

// Definição do relé
#define rele4 6       //o pino IN1 do Rele (modulo 1) será ligado ao pino 12 da Esp32
#define rele3 7       //o pino IN2 do Rele (modulo 1) será ligado ao pino 27 da Esp32
#define rele2 8      //o pino IN1 do Rele (modulo 2) será ligado ao pino 33 da Esp32
#define rele1 9      //o pino IN2 do Rele (modulo 2)  será ligado ao pino 15 da Esp32

#define led 13

int distancia; //VARIÁVEL DO TIPO INTEIRO
String result; //VARIÁVEL DO TIPO STRING

void setup() {
  pinMode(echoPin, INPUT); //DEFINE O PINO COMO ENTRADA (RECEBE)
  pinMode(trigPin, OUTPUT); //DEFINE O PINO COMO SAIDA (ENVIA)
  pinMode(sensorDianteiro, INPUT); // DEFINE O PINO COM ENTRADA
  pinMode(sensorTraseiro, INPUT); // DEFINE O PINO COM ENTRADA
  pinMode(rele1, OUTPUT); // DEFINE O PINO COMO SAIDA
  pinMode(rele2, OUTPUT); // DEFINE O PINO COMO SAIDA
  pinMode(rele3, OUTPUT); // DEFINE O PINO COMO SAIDA
  pinMode(rele4, OUTPUT); // DEFINE O PINO COMO SAIDA
  pinMode(led, OUTPUT); // DEFINE O PINO COMO SAIDA

  digitalWrite(led, HIGH); //LIGA O LED DA PLACA ARDUINO

  Serial.begin(9600); // INICIALIZA A PORTA SERIAL

  stop(); // STOP DOS MOTORES

  attachInterrupt(sensorDianteiro, linhaFrente, FALLING);
  attachInterrupt(sensorTraseiro, linhaTras, FALLING);
}

void loop() {         
  Serial.print("Sensor Frente:");
  Serial.println(digitalRead(sensorDianteiro));
  Serial.print("Sensor Tras:");
  Serial.println(digitalRead(sensorTraseiro));

  hcsr04(); // FAZ A CHAMADA DO MÉTODO "hcsr04()"
  Serial.print("Distancia "); //IMPRIME O TEXTO NO MONITOR SERIAL
  Serial.print(distancia); ////IMPRIME NO MONITOR SERIAL A DISTÂNCIA MEDIDA
  Serial.println("cm"); //IMPRIME O TEXTO NO MONITOR SERIAL

  if (distancia > 1 && distancia <50) { // Se um obstáculo for detectado a menos de 100 cm
    movimento_frente();
  } else {                 // Se nenhum obstáculo for detectado
    movimento_frente();
    delay(200);
    movimento_esquerda();
  }
}

//MÉTODO RESPONSÁVEL POR CALCULAR A DISTÂNCIA
void hcsr04(){
    digitalWrite(trigPin, LOW); //SETA O PINO 6 COM UM PULSO BAIXO "LOW"
    delayMicroseconds(2); //INTERVALO DE 2 MICROSSEGUNDOS
    digitalWrite(trigPin, HIGH); //SETA O PINO 6 COM PULSO ALTO "HIGH"
    delayMicroseconds(10); //INTERVALO DE 10 MICROSSEGUNDOS
    digitalWrite(trigPin, LOW); //SETA O PINO 6 COM PULSO BAIXO "LOW" NOVAMENTE
    //FUNÇÃO RANGING, FAZ A CONVERSÃO DO TEMPO DE
    //RESPOSTA DO ECHO EM CENTIMETROS, E ARMAZENA
    //NA VARIAVEL "distancia"
    distancia = (ultrasonic.Ranging(CM)); //VARIÁVEL GLOBAL RECEBE O VALOR DA DISTÂNCIA MEDIDA
    result = String(distancia); //VARIÁVEL GLOBAL DO TIPO STRING RECEBE A DISTÂNCIA(CONVERTIDO DE INTEIRO PARA STRING)
    delay(200); //INTERVALO DE 500 MILISSEGUNDOS
 }

void movimento_esquerda(){
  digitalWrite(rele1, 1);                 //envia nível lógico alto para o rele 1       //liga o motor 1
  digitalWrite(rele2, 0);                 //envia nível lógico baixo para o rele 2      //para frente
  digitalWrite(rele3, 0);                 //envia nível lógico baixo para o rele 3       **liga o motor 2
  digitalWrite(rele4, 1);                 //envia nível lógico alto para o rele 4        **para tras
  delay(300);
  stop();
}

void movimento_direita(){
  digitalWrite(rele1, 1);                 //envia nível lógico alto para o rele 1       //liga o motor 1
  digitalWrite(rele2, 0);                 //envia nível lógico baixo para o rele 2      //para frente
  digitalWrite(rele3, 0);                 //envia nível lógico baixo para o rele 3       **liga o motor 2
  digitalWrite(rele4, 1);                 //envia nível lógico alto para o rele 4        **para tras
  delay(300);
  stop();
}

void movimento_frente(){
  digitalWrite(rele1, 1);                 //envia nível lógico alto para o rele 1       //liga o motor 1
  digitalWrite(rele2, 0);                 //envia nível lógico baixo para o rele 2      //para frente
  digitalWrite(rele3, 1);                 //envia nível lógico alto para o rele 3       **liga o motor 2
  digitalWrite(rele4, 0);                 //envia nível lógico baixo para o rele 4      **para frente
}

void movimento_tras(){
  digitalWrite(rele1, 0);                 //envia nível lógico baixo para o rele 1       //liga o motor 1
  digitalWrite(rele2, 1);                 //envia nível lógico alto para o rele 2        //para tras
  digitalWrite(rele3, 0);                 //envia nível lógico baixo para o rele 3       **liga o motor 2
  digitalWrite(rele4, 1);                 //envia nível lógico alto para o rele 4        **para tras
}

void stop(){
  digitalWrite(rele1, 1);                 //envia nível lógico alto para o rele 1      //desliga o motor 1
  digitalWrite(rele2, 1);                 //envia nível lógico alto para o rele 2      //desliga o motor 1
  digitalWrite(rele3, 1);                 //envia nível lógico alto para o rele 3      **desliga o motor 2
  digitalWrite(rele4, 1);                 //envia nível lógico alto para o rele 4      **desliga o motor 2
}

void linhaFrente(){
  movimento_tras();
  while(digitalRead(sensorDianteiro) == 1);
}

void linhaTras(){
  movimento_frente();
  while(digitalRead(sensorTraseiro) == 1);
}
