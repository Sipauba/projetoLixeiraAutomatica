#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <NewPing.h> //biblioteca do sensor ultrassônico
 
#define TRIGGER_PIN  12 // o pino que está o trigger, que envia o sinal
#define ECHO_PIN     11 // o pino que irá receber o eco do sinal
#define MAX_DISTANCE 200 // a distância máxima de leitura, lembrando que este sensor tem um máximo de 400 cm
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); //(pino do triger, pino do eco, distância máxima em cm)

#define pinServo 2 //pino do sinal do servo(fio laranja)
Servo servo; //criando o objeto

LiquidCrystal_I2C lcd(0x27,16,2);

const int pinoPotDistancia = A0; //potenciômetro para controlar a distância de acionamento da lixeira
int distanciaAcionamento = 0;

const int pinoPotTempo = A1; //potenciometro para controlar o tempo que a tampa fica aberta
int tempoAcionamento = 0;

long tempoContinuo = 0;
long tempoAnterior = 0;

void setup() {
  Serial.begin(115200); //é assim porque sim. lembrar de configurar o monitor serial também para 115200, senão não funciona
  servo.attach(pinServo); // definindo qual pino irá comandar o servo
  servo.write(0); //define a posição do servo em 0 graus
}
 
void loop() {
  int distancia = sonar.ping_cm(); 
  Serial.print("Ping: ");
  Serial.print(distancia); // leitura do sensor e retornando o valor em cm. vide documentação NewPing
  Serial.println("cm");

  distanciaAcionamento = analogRead(pinoPotDistancia);
  distanciaAcionamento = map(distanciaAcionamento,0,1023,0,200);
  Serial.print("Distância para acionar:");
  Serial.print(distanciaAcionamento);
  Serial.println("cm");
  
  tempoAcionamento = analogRead(pinoPotTempo);
  tempoAcionamento = map(tempoAcionamento,0,1023,2,30);
  Serial.print("Tempo acionada:");
  Serial.print(tempoAcionamento);
  Serial.println("s");

  lcd.init();                   
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Dist.Ativ.:");
  lcd.setCursor(12,0);
  lcd.print(distanciaAcionamento);
  lcd.setCursor(14,0);
  lcd.print("cm");
  lcd.setCursor(0,1);
  lcd.print("Timer:");
  lcd.setCursor(6,1);
  lcd.print(tempoAcionamento);
  lcd.setCursor(8,1);
  lcd.print("segs");

  long tempoContinuo = millis(); //começa a contar o tempo continuamente em milisegundos 
  // o tipo de variável é long pq o numero armazenado em ms é muito grande

  if (distancia <= distanciaAcionamento){ 
    //quando atende a condição, énecessário que essas duas variáveis se igualem para que a condição abaixo funcione
    //e irá garantir que a tampa só feche se não tiver ninguém na frente do sensor  
    tempoAnterior = tempoContinuo;
    servo.write(180); //ao acionar, muda o angulo (aberta) 
  }
  if ((tempoContinuo - tempoAnterior) >= (tempoAcionamento * 1000)){
    //essa condição verifica se foi atingido o tempo configurada de abertura da tampa
    servo.write(0); // estado inicial da lixeira (fechada)
  }  
}
