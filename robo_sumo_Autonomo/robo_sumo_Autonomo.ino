/* ---------- Inclusão de Bibliotecas ---------- */

#include <Ultrasonic.h>

/* ---------- Definição de Pinos ---------- */
#define trigger A0
#define echo   A1
#define sensor_frontal_1 A2
#define sensor_frontal_2 A3
#define sensor_traseiro_1 A4
#define sensor_traseiro_2 A5
#define rele4 5       //o pino IN1 do Rele (modulo 1) será ligado ao pino 8 do arduino
#define rele3 4       //o pino IN2 do Rele (modulo 1) será ligado ao pino 9 do arduino
#define rele2 3      //o pino IN1 do Rele (modulo 2) será ligado ao pino 10 do arduino
#define rele1 2      //o pino IN2 do Rele (modulo 2)  será ligado ao pino 11 do arduino 

/* ---------- Criação de Objetos ---------- */
Ultrasonic ultrasonic(trigger, echo);

/* ---------- Variáveis ---------- */
int distancia;

/* ---------- Função de Configuração ---------- */
void setup() {
  Serial.begin(9600);
  pinMode(sensor_frontal_1, INPUT);
  pinMode(sensor_frontal_2, INPUT);
  pinMode(sensor_traseiro_1, INPUT);
  pinMode(sensor_traseiro_2, INPUT);
}

void loop() {
  distancia = ultrasonic.read();
  Serial.print("Distance in CM: ");
  Serial.println(distancia);
  //Verifica se os sensor dianteiros estão no branco
  if((digitalRead(sensor_frontal_1) == 0) || (digitalRead(sensor_frontal_2) == 0)){
    movimento_tras();
  }

  //Verifica se os sensor traseiros estão no branco
  if((digitalRead(sensor_traseiro_1) == 0) || (digitalRead(sensor_traseiro_2) == 0)){
    movimento_frente();
  }

  //verifica se tem oponentes a frente
  if(distancia > 20){
    //procura oponente
    Serial.println("procurando oponente");
    procura();
  }

  //encontrou oponente
  if(distancia > 0 && distancia <= 20){
    //trava na condição até encontrar a borda da arena
    while((digitalRead(sensor_traseiro_1) == 1) || (digitalRead(sensor_traseiro_2) == 1)){
      Serial.println("achei o oponente");
      movimento_frente();
    }
    stop();
    delay(1000);
    tras();
    delay(1000);
  }
}

void movimento_esquerda(){
  digitalWrite(rele1, 0);                 //envia nível lógico baixo para o rele 1       //liga o motor 1
  digitalWrite(rele2, 1);                 //envia nível lógico alto para o rele 2        //para tras
  digitalWrite(rele3, 1);                 //envia nível lógico alto para o rele 3       **liga o motor 2
  digitalWrite(rele4, 0);                 //envia nível lógico baixo para o rele 4      **para frente
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

void movimento_esquerda(){
  movimento_frente();
  delay(300);
  esquerda();
  delay(300);
}
