#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <ArduinoJson.h>

// Pinos
#define RELAY_PIN 27 // Pino do relé
#define ANALOG_SENSOR_PIN 34 // Pino do sinal analógico

// Credenciais Wi-Fi
const char* ssid = "WILAMIL_AUX";
const char* password = "!Aero123";

// Configuração do MQTT
const char* mqtt_server = "mqtt.dataext.com.br";
const char* mqtt_topic = "test/ifsc";

// Inicialização de clientes Wi-Fi, MQTT e sensor MPU6050
WiFiClient espClient;
PubSubClient client(espClient);
Adafruit_MPU6050 mpu;

// Variáveis para o sensor analógico
float lastSensorValue = 0;
int pulseCount = 0; // Contador de pulsos
unsigned long lastRPMCalcTime = 0;
float rpm = 0;

// Estado do relé
bool motorOn = false;

// Função para conectar ao Wi-Fi
void connectWiFi() {
    Serial.print("Conectando ao Wi-Fi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWi-Fi conectado!");
    Serial.print("Endereço IP: ");
    Serial.println(WiFi.localIP());
}

// Função para conectar ao MQTT
void connectMQTT() {
    client.setServer(mqtt_server, 1883);
    while (!client.connected()) {
        Serial.print("Conectando ao broker MQTT...");
        if (client.connect("ESP32Client")) {
            Serial.println("Conectado!");
            client.subscribe(mqtt_topic); // Inscreve-se no tópico
            Serial.println("Inscrito no tópico: " + String(mqtt_topic));
        } else {
            Serial.print("Falha ao conectar, código de erro: ");
            Serial.println(client.state());
            delay(5000);
        }
    }
}

// Callback do MQTT para comandos
void mqttCallback(char* topic, byte* payload, unsigned int length) {
    String message;
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }

    Serial.print("Mensagem recebida: ");
    Serial.println(message);

    if (message == "ligar") {
        digitalWrite(RELAY_PIN, LOW); // Liga o relé
        motorOn = true;
        Serial.println("Motor ligado.");
    } else if (message == "desligar") {
        digitalWrite(RELAY_PIN, HIGH); // Desliga o relé
        motorOn = false;
        Serial.println("Motor desligado.");
    }
}

// Função para calcular RPM
void calculateRPM() {
    unsigned long currentTime = millis();
    unsigned long timeElapsed = currentTime - lastRPMCalcTime;

    if (timeElapsed >= 1000) { // Atualiza a cada segundo
        rpm = (pulseCount / 2.0) * (60000.0 / timeElapsed); // 2 pulsos por rotação
        pulseCount = 0; // Reseta o contador
        lastRPMCalcTime = currentTime;
    }
}

// Função para coletar dados do MPU6050
void getMPUData(float& ax, float& ay, float& az, float& gx, float& gy, float& gz) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    ax = a.acceleration.x;
    ay = a.acceleration.y;
    az = a.acceleration.z;
    gx = g.gyro.x;
    gy = g.gyro.y;
    gz = g.gyro.z;
}

// Configuração inicial
void setup() {
    Serial.begin(115200);

    // Configuração do relé
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, HIGH); // Relé desligado inicialmente

    // Inicializa Wi-Fi e MQTT
    connectWiFi();
    client.setCallback(mqttCallback);
    connectMQTT();

    // Inicialização do MPU6050
    if (!mpu.begin()) {
        Serial.println("Falha ao inicializar o MPU6050!");
        while (1);
    }
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    Serial.println("Configuração completa.");
}

// Loop principal
void loop() {
    // Mantém a conexão MQTT
    if (!client.connected()) {
        connectMQTT();
    }
    client.loop();

    // Leitura do sensor analógico
    float sensorValue = analogRead(ANALOG_SENSOR_PIN);
    if (sensorValue > 2000 && lastSensorValue <= 2000) { // Detecta rampa de subida
        pulseCount++;
    }
    lastSensorValue = sensorValue;

    // Calcula RPM
    calculateRPM();

    // Coleta dados do MPU6050
    float ax, ay, az, gx, gy, gz;
    getMPUData(ax, ay, az, gx, gy, gz);

    // Cria mensagem JSON
    StaticJsonDocument<256> jsonDoc;
    jsonDoc["sensor_magnetico_efeito_hall"]["ligado"] = motorOn;
    jsonDoc["sensor_magnetico_efeito_hall"]["valor"] = rpm;
    jsonDoc["acelerometro_gyroscopio"]["aceleracao_x"] = ax;
    jsonDoc["acelerometro_gyroscopio"]["aceleracao_y"] = ay;
    jsonDoc["acelerometro_gyroscopio"]["aceleracao_z"] = az;
    jsonDoc["acelerometro_gyroscopio"]["giro_x"] = gx;
    jsonDoc["acelerometro_gyroscopio"]["giro_y"] = gy;
    jsonDoc["acelerometro_gyroscopio"]["giro_z"] = gz;

    char jsonBuffer[512];
    serializeJson(jsonDoc, jsonBuffer);

    // Publica no broker MQTT
    client.publish(mqtt_topic, jsonBuffer);
    Serial.println("Mensagem publicada: ");
    Serial.println(jsonBuffer);

    delay(100); // Aguarda 100ms
}


