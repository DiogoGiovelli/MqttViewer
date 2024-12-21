#include <AFMotor.h>

AF_DCMotor principalMotor(2);

// Pino analógico para verificar a tensão
const int pinoSensorVoltagem = A0;

void setup() {
    // Inicializa o motor com velocidade 0
    principalMotor.setSpeed(0);
    
    // Configura o pino A0 para leitura analógica
    pinMode(pinoSensorVoltagem, INPUT);
    
    // Verifica se a tensão no pino A0 é maior que 1 volt
    if (analogRead(pinoSensorVoltagem) > 51) { // 51 é o valor aproximado para 1V em um ADC de 10 bits (1V / 5V * 1023 = 204,6 ~ 51 para 1V)
        Serial.println("Tensão maior que 1V, iniciando rotina do motor...");
        loop(); // Inicia a rotina do motor se a condição for atendida
    } else {
        Serial.println("Aguarde, a tensão no pino A0 está baixa.");
    }
}

void loop() {
    // Verifica a tensão no pino A0 antes de começar o ciclo
    if (analogRead(pinoSensorVoltagem) > 51) { // Verifica se a tensão é maior que 1V
        int count;
        
        // Aumentando a velocidade do motor
        for(count = 0; count <= 43; count++){
            principalMotor.setSpeed(count);
            principalMotor.run(FORWARD);
            delay(78);
        }
        principalMotor.setSpeed(count);
        delay(10000);

        // Continua aumentando a velocidade
        for(count = 43; count <= 85; count++){
            principalMotor.setSpeed(count);
            principalMotor.run(FORWARD);
            delay(78);
        }
        principalMotor.setSpeed(count);
        delay(10000);

        // Aumenta ainda mais a velocidade
        for(count = 85; count <= 128; count++){
            principalMotor.setSpeed(count);
            principalMotor.run(FORWARD);
            delay(78);
        }
        principalMotor.setSpeed(count);
        delay(10000);

        // Reduz a velocidade gradualmente
        for(count = 128; count >= 85; count--){
            principalMotor.setSpeed(count);
            principalMotor.run(FORWARD);
            delay(78);
        }
        delay(10000);

        for(count = 85; count >= 43; count--){
            principalMotor.setSpeed(count);
            principalMotor.run(FORWARD);
            delay(78);
        }
        delay(10000);

        for(count = 43; count >= 0; count--){
            principalMotor.setSpeed(count);
            principalMotor.run(FORWARD);
            delay(78);
        }
        delay(10000);

        // Desliga o motor
        principalMotor.setSpeed(0);
        principalMotor.run(RELEASE);
        delay(20000);
    } else {
        // Se a tensão for menor ou igual a 1V, aguarda e imprime mensagem
        Serial.println("A tensão no pino A0 está baixa, rotina do motor não iniciada.");
        delay(5000); // Aguarda um pouco antes de verificar a tensão novamente
    }
}



//#include <AFMotor.h>
//
//AF_DCMotor principalMotor(2);
//
//void setup() {
//    principalMotor.setSpeed(0); //como so 8 bits, vai de zero a 0 a 255    
//}
//
//void loop(){
//
//    int count;
//
//    for(count = 0; count <= 43; count++){
//       principalMotor.setSpeed(count);
//       principalMotor.run(FORWARD);
//       delay(78);
//    }
//  
//    principalMotor.setSpeed(count);
//    principalMotor.run(FORWARD);
//    delay(10000);
//
//    for(count; count <= 85; count++){
//       principalMotor.setSpeed(count);
//       principalMotor.run(FORWARD);
//       delay(78);
//    }
//
//    principalMotor.setSpeed(count);
//    principalMotor.run(FORWARD);
//    delay(10000);
//
//    for(count; count <= 128; count++){
//       principalMotor.setSpeed(count);
//       principalMotor.run(FORWARD);
//       delay(78);
//    }
//
//    principalMotor.setSpeed(count);
//    principalMotor.run(FORWARD);
//    delay(10000);
//
//    for(count; count >= 85; count--){
//       principalMotor.setSpeed(count);
//       principalMotor.run(FORWARD);
//       delay(78);
//    }    
//
//    principalMotor.setSpeed(count);
//    principalMotor.run(FORWARD);
//    delay(10000);
//
//   for(count; count >= 43; count--){
//       principalMotor.setSpeed(count);
//       principalMotor.run(FORWARD);
//       delay(78);
//    }
//
//    principalMotor.setSpeed(count);
//    principalMotor.run(FORWARD);
//    delay(10000);
//    
//   for(count; count >= 0; count--){
//       principalMotor.setSpeed(count);
//       principalMotor.run(FORWARD);
//       delay(78);
//    }
// 
//    principalMotor.setSpeed(0);
//    principalMotor.run(RELEASE);
//    delay(20000);
//}

/******************************************************************************************
 * Mapa do Sketch
 * 1- Cabeçalho (bibliotecas, definições, variáveis e objetos globais)
 * 2- setup()
 * 3- loop()
 * 4- ci74HC595Write()
 * 5- delayPWM()
 * 
 * Desenvolvido por Ideias Tecnologia / Brincando com Ideias
 */

//#include <Servo.h>
//
//#define pinSH_CP  4   //Pino Clock  DIR_CLK
//#define pinST_CP  12  //Pino Latch  DIR_LATCH
//#define pinDS     8   //Pino Data   DIR_SER
//#define pinEnable 7   //Pino Enable DIR_EN
//
//#define pinMotor1PWM 11
//#define pinMotor2PWM 3
//#define pinMotor3PWM 5
//#define pinMotor4PWM 6
//
//#define pinServo1 10
//#define pinServo2 9
//
//#define qtdeCI   1
//
//#define bitMotor1A 2
//#define bitMotor1B 3
//#define bitMotor2A 1
//#define bitMotor2B 4
//#define bitMotor3A 5
//#define bitMotor3B 7
//#define bitMotor4A 0
//#define bitMotor4B 6
//
//void ci74HC595Write(byte pino, bool estado);
//
//Servo servo1;
//Servo servo2;
//
//void setup() {
//   pinMode(pinSH_CP, OUTPUT);
//   pinMode(pinST_CP, OUTPUT);
//   pinMode(pinEnable, OUTPUT);
//   pinMode(pinDS, OUTPUT);
//
//   pinMode(pinMotor1PWM, OUTPUT);
//   pinMode(pinMotor2PWM, OUTPUT);
//   pinMode(pinMotor3PWM, OUTPUT);
//   pinMode(pinMotor4PWM, OUTPUT);
//
//   digitalWrite(pinEnable, LOW);
//
//   servo1.attach(pinServo1);
//   servo2.attach(pinServo2);
//   Serial.begin(9600);
//}
//
//void loop() {
//
//  servo1.write(0);
//  servo2.write(180);
// 
//  Serial.println("Motores A=HIGH B=LOW");
//  ci74HC595Write(bitMotor1A, HIGH);
//  ci74HC595Write(bitMotor1B, LOW);
//  ci74HC595Write(bitMotor2A, HIGH);
//  ci74HC595Write(bitMotor2B, LOW);
//  ci74HC595Write(bitMotor3A, HIGH);
//  ci74HC595Write(bitMotor3B, LOW);
//  ci74HC595Write(bitMotor4A, HIGH);
//  ci74HC595Write(bitMotor4B, LOW);
//  delayPWM(2000);
//
//  Serial.println("Motores A=LOW B=LOW");
//  ci74HC595Write(bitMotor1A, LOW);
//  ci74HC595Write(bitMotor1B, LOW);
//  ci74HC595Write(bitMotor2A, LOW);
//  ci74HC595Write(bitMotor2B, LOW);
//  ci74HC595Write(bitMotor3A, LOW);
//  ci74HC595Write(bitMotor3B, LOW);
//  ci74HC595Write(bitMotor4A, LOW);
//  ci74HC595Write(bitMotor4B, LOW);
//  delayPWM(1000);
//
//  servo1.write(180);
//  servo2.write(0);
//
//  Serial.println("Motor1 A=LOW B=HIGH");
//  ci74HC595Write(bitMotor1A, LOW);
//  ci74HC595Write(bitMotor1B, HIGH);
//  ci74HC595Write(bitMotor2A, LOW);
//  ci74HC595Write(bitMotor2B, HIGH);
//  ci74HC595Write(bitMotor3A, LOW);
//  ci74HC595Write(bitMotor3B, HIGH);
//  ci74HC595Write(bitMotor4A, LOW);
//  ci74HC595Write(bitMotor4B, HIGH);
//  delayPWM(2000);
//
//  Serial.println("Motor1 A=HIGH B=HIGH");
//  ci74HC595Write(bitMotor1A, HIGH);
//  ci74HC595Write(bitMotor1B, HIGH);
//  ci74HC595Write(bitMotor2A, HIGH);
//  ci74HC595Write(bitMotor2B, HIGH);
//  ci74HC595Write(bitMotor3A, HIGH);
//  ci74HC595Write(bitMotor3B, HIGH);
//  ci74HC595Write(bitMotor4A, HIGH);
//  ci74HC595Write(bitMotor4B, HIGH);
//  delayPWM(1000);
//  
//}
//
//void ci74HC595Write(byte pino, bool estado) {
//  static byte ciBuffer[qtdeCI];
//
//  bitWrite(ciBuffer[pino / 8], pino % 8, estado);
//  
//  digitalWrite(pinST_CP, LOW); //Inicia a Transmissão
//  
//  digitalWrite(pinDS, LOW);    //Apaga Tudo para Preparar Transmissão
//  digitalWrite(pinSH_CP, LOW);
//
//  for (int nC = qtdeCI-1; nC >= 0; nC--) {
//      for (int nB = 7; nB >= 0; nB--) {
//  
//          digitalWrite(pinSH_CP, LOW);  //Baixa o Clock      
//          
//          digitalWrite(pinDS,  bitRead(ciBuffer[nC], nB) );     //Escreve o BIT
//          
//          digitalWrite(pinSH_CP, HIGH); //Eleva o Clock
//          digitalWrite(pinDS, LOW);     //Baixa o Data para Previnir Vazamento      
//      }  
//  }
//  
//  digitalWrite(pinST_CP, HIGH);  //Finaliza a Transmissão
//}
//
//void delayPWM(unsigned long tempo) {
//unsigned long inicio = millis();
//byte valA0;  
//  
//  while ( (millis() - inicio) < tempo ) {
//     valA0 = map(analogRead(A0), 0, 1023, 0, 255);
//     analogWrite(pinMotor1PWM, valA0);
//     analogWrite(pinMotor2PWM, valA0);
//     analogWrite(pinMotor3PWM, valA0);
//     analogWrite(pinMotor4PWM, valA0);
//  }
//}


//    pinMode(in1, OUTPUT);
//    pinMode(in2, OUTPUT);
//    pinMode(enA, OUTPUT);
//
//    // Configuração inicial: define a rotação do motor no sentido horário
//    digitalWrite(in1, HIGH);
//    digitalWrite(in2, LOW);
//
//    // Aciona o motor com 33.3% da velocidade por 10 segundos
//    analogWrite(enA, 85);  // 33.3% de 255 é aproximadamente 85
//    delay(10000);
//
//    // Ajusta para 66.7% da velocidade e mantém por 10 segundos
//    analogWrite(enA, 170); // 66.7% de 255 é aproximadamente 170
//    delay(10000);
//
//    // Ajusta para 100% da velocidade e mantém por 10 segundos
//    analogWrite(enA, 255); // 100% da velocidade
//    delay(10000);
//
//    // Reduz para 66.7% da velocidade e mantém por 10 segundos
//    analogWrite(enA, 170); // 66.7% da velocidade
//    delay(10000);
//
//    // Reduz para 33.3% da velocidade e mantém por 10 segundos
//    analogWrite(enA, 85);  // 33.3% da velocidade
//    delay(10000);
//
//    // Desliga o motor
//    analogWrite(enA, 0);
