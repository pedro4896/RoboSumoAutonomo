#define trigger 22
#define echo   23

void setup() {
  Serial.begin(9600);
  pinMode(trigger, OUTPUT); // Configura o pino TRIG como saída
  pinMode(echo, INPUT); // Configura o pino ECHO como entrada
  //digitalWrite(13,HIGH);
}

void loop() {
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  
  long duracao = pulseIn(echo, HIGH); // Mede o tempo de resposta do ECHO  
  float distancia = (duracao * 0.0343) / 2;// Calcula a distância usando a velocidade do som (aproximadamente 343 m/s)
  Serial.print("Distância: ");
  Serial.print(distancia);
  Serial.println(" cm");
  
  delay(500); // Aguarda 1 segundo antes de fazer a próxima leitura
}
