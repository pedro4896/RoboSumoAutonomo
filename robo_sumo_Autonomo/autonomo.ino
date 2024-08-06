int davomiylik, sm;

#define trigPin 34
#define echoPin 19

#define linha1 36
#define linha2 4

// Definição do relé
#define rele4 12       //o pino IN1 do Rele (modulo 1) será ligado ao pino 12 da Esp32
#define rele3 27       //o pino IN2 do Rele (modulo 1) será ligado ao pino 27 da Esp32
#define rele2 33      //o pino IN1 do Rele (modulo 2) será ligado ao pino 33 da Esp32
#define rele1 15      //o pino IN2 do Rele (modulo 2)  será ligado ao pino 15 da Esp32

void plusUltra() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(15);
  digitalWrite(trigPin, LOW);
  davomiylik = pulseIn(echoPin, HIGH);
  sm = davomiylik / 58;
}

void movimento_esquerda(){
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

void movimento_frente_esquerda(){
  digitalWrite(rele1, 1);                 //envia nível lógico alto para o rele 1      //liga o motor 1
  digitalWrite(rele2, 0);                 //envia nível lógico baixo para o rele 2     //para frente
  digitalWrite(rele3, 1);                 //envia nível lógico alto para o rele 3      **desliga o motor 2
  digitalWrite(rele4, 1);                 //envia nível lógico alto para o rele 4      **desliga o motor 2
}

void movimento_frente_direita(){
  digitalWrite(rele1, 1);                 //envia nível lógico alto para o rele 1        //desliga o motor 1
  digitalWrite(rele2, 1);                 //envia nível lógico alto para o rele 2        //desliga o motor 1
  digitalWrite(rele3, 1);                 //envia nível lógico alto para o rele 3       **liga o motor 2
  digitalWrite(rele4, 0);                 //envia nível lógico baixo para o rele 4      **para frente
}

void movimento_tras_esquerda(){
  digitalWrite(rele1, 1);                 //envia nível lógico alto para o rele 1        //desliga o motor 1
  digitalWrite(rele2, 1);                 //envia nível lógico alto para o rele 2        //desliga o motor 1
  digitalWrite(rele3, 0);                 //envia nível lógico baixo para o rele 3       **liga o motor 2
  digitalWrite(rele4, 1);                 //envia nível lógico alto para o rele 4        **para tras
}

void movimento_tras_direita(){
  digitalWrite(rele1, 0);                 //envia nível lógico baixo para o rele 1       //liga o motor 1
  digitalWrite(rele2, 1);                 //envia nível lógico alto para o rele 2        //para tras
  digitalWrite(rele3, 1);                 //envia nível lógico alto para o rele 3      **desliga o motor 2
  digitalWrite(rele4, 1);                 //envia nível lógico alto para o rele 4      **desliga o motor 2
}

void stop(){
  digitalWrite(rele1, 1);                 //envia nível lógico alto para o rele 1      //desliga o motor 1
  digitalWrite(rele2, 1);                 //envia nível lógico alto para o rele 2      //desliga o motor 1
  digitalWrite(rele3, 1);                 //envia nível lógico alto para o rele 3      **desliga o motor 2
  digitalWrite(rele4, 1);                 //envia nível lógico alto para o rele 4      **desliga o motor 2
}

void IRAM_ATTR linhaFrente(){
  movimento_frente();
}

void IRAM_ATTR linhaTras(){
  movimento_frente();
}

void setup() {
  //Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  attachInterrupt(linha1, linhaFrente, FALLING);
  attachInterrupt(linha2, linhaTras, FALLING);
}

void loop() {                                                                                                                                                                                                                       
  plusUltra();
  //Serial.println(sm);
  if (sm > 1 && sm <50) { // Se um obstáculo for detectado a menos de 100 cm
    movimento_frente_esquerda();
  } else {                 // Se nenhum obstáculo for detectado
    movimento_frente();
  }
}
