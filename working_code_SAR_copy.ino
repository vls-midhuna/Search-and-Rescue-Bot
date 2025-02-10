#include <MQUnifiedsensor.h>
#include <DFRobot_DHT11.h>
DFRobot_DHT11 DHT;

// Define board details
#define placa "Vega Aries"
#define Voltage_Resolution 5
#define pin A0                  // Analog input for MQ-8 sensor
#define type "MQ-8"             // Gas sensor type
#define ADC_Bit_Resolution 12
#define RatioMQ8CleanAir 10      // RS / R0 = 10 ppm

// Declare MQ-8 Sensor
MQUnifiedsensor MQ8(placa, Voltage_Resolution, ADC_Bit_Resolution, pin, type);

// Define ultrasonic sensor pins
#define echoPin1 0
#define trigPin1 3
#define echoPin2 10
#define trigPin2 11
#define echoPin3 13
#define trigPin3 14
#define echoPin4 9
#define trigPin4 8

// Define motor driver pins
#define motorpin1 1
#define motorpin2 2
#define motorpin3 4
#define motorpin4 5

// Define other sensor pins
#define buzzer 15
#define DHT11_pin 7

long duration1, duration2, duration3, duration4;
int distance1, distance2, distance3, distance4;

void setup() {
  Serial.begin(9600);

  // Initialize sensor pins
  pinMode(trigPin1, OUTPUT); pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT); pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT); pinMode(echoPin3, INPUT);
  pinMode(trigPin4, OUTPUT); pinMode(echoPin4, INPUT);

  pinMode(motorpin1, OUTPUT);
  pinMode(motorpin2, OUTPUT);
  pinMode(motorpin3, OUTPUT);
  pinMode(motorpin4, OUTPUT);
  pinMode(buzzer, OUTPUT);

  // Initialize MQ-8 Sensor
  MQ8.setRegressionMethod(1);
  MQ8.setA(2127.2); MQ8.setB(-2.526);
  MQ8.init(); 

  float calcR0 = 0;
  for (int i = 1; i <= 10; i++) {
    MQ8.update();
    calcR0 += MQ8.calibrate(RatioMQ8CleanAir);
  }
  MQ8.setR0(calcR0 / 10);
  MQ8.serialDebug(true);
}

void loop() {
  // Read temperature and humidity
  DHT.read(DHT11_pin);
  Serial.print("Temperature: "); Serial.print(DHT.temperature);
  Serial.print(" C, Humidity: "); Serial.println(DHT.humidity);

  // Read distances from ultrasonic sensors
  distance1 = getDistance(trigPin1, echoPin1);
  distance2 = getDistance(trigPin2, echoPin2);
  distance3 = getDistance(trigPin3, echoPin3);
  distance4 = getDistance(trigPin4, echoPin4);

  // Read gas sensor data
  MQ8.update();
  MQ8.readSensor();
  MQ8.serialDebug();

  // Decision making for robot movement
  if (distance1 < 20) {
    stop();
    delay(500);
    move_backward();
    delay(1000);
    move_right();
    delay(1000);
  } else {
    move_straight();
  }

  if (MQ8.readSensor() > 100) {
    digitalWrite(buzzer, HIGH);
    Serial.println("Gas detected! Alert!");
  } else {
    digitalWrite(buzzer, LOW);
  }
  
  delay(500);
}

int getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  return pulseIn(echoPin, HIGH) * 0.034 / 2;
}

void move_straight() {
  digitalWrite(motorpin1, HIGH);
  digitalWrite(motorpin2, LOW);
  digitalWrite(motorpin3, HIGH);
  digitalWrite(motorpin4, LOW);
}

void move_backward() {
  digitalWrite(motorpin1, LOW);
  digitalWrite(motorpin2, HIGH);
  digitalWrite(motorpin3, LOW);
  digitalWrite(motorpin4, HIGH);
}

void move_right() {
  digitalWrite(motorpin1, HIGH);
  digitalWrite(motorpin2, LOW);
  digitalWrite(motorpin3, LOW);
  digitalWrite(motorpin4, LOW);
}

void move_left() {
  digitalWrite(motorpin1, LOW);
  digitalWrite(motorpin2, LOW);
  digitalWrite(motorpin3, HIGH);
  digitalWrite(motorpin4, LOW);
}

void stop() {
  digitalWrite(motorpin1, LOW);
  digitalWrite(motorpin2, LOW);
  digitalWrite(motorpin3, LOW);
  digitalWrite(motorpin4, LOW);
}
