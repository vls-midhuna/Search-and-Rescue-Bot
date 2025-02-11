# Search and Rescue Bot

## Introduction
A Search and Rescue (SAR) Robot is an autonomous or remotely controlled robotic system designed to assist in rescue missions during emergencies and disasters. These robots help locate victims, navigate hazardous environments, and provide real-time data to rescuers, improving response time and safety. Equipped with sensors like ultrasonic, gas detectors, and temperature monitors, SAR robots can detect obstacles, identify survivors, and alert rescue teams through IoT connectivity. Their ability to operate in dangerous or inaccessible areas makes them valuable tools in disaster relief operations.

## Overview
This project presents an autonomous Search and Rescue (SAR) robot designed to assist in disaster response and emergency situations. The robot is equipped with smart sensors and IoT connectivity to navigate hazardous environments, detect survivors, and provide real-time data to rescue teams. By leveraging autonomous navigation and environmental monitoring, the system enhances rescue operations, reduces risks for human responders, and improves efficiency in critical situations.

**Key Components**:
- **Aries v1.0** – Core microcontroller responsible for processing sensor data and controlling navigation.
- **NodeMCU (ESP8266)** – Transmits real-time data over Wi-Fi, enabling remote monitoring and alerts.
- **Ultrasonic Sensors** (Front, Left, Right, Rear) – Detects obstacles and aids in autonomous navigation.
- **Gas Sensor** – Identifies hazardous gases, ensuring safety in disaster-stricken areas.
- **DHT11 Sensor**– Monitors temperature and humidity for environmental assessment.
- **Buzzer** – Provides audible alerts for detected hazards or survivor identification.
- **Chassis** – Supports movement across rough terrains, allowing the robot to operate in critical areas.

By integrating these components, the SAR robot enhances rescue efficiency, aids in survivor detection, and provides valuable real-time data, making it an essential tool for disaster management and emergency response.

## Components required with Bill of Materials
| Item                   | Quantity | Description                                                   | Links to Products                                      |
|------------------------|----------|---------------------------------------------------------------|---------------------------------------------------|
| Aries Micro v1.0(VEGA Processor)  | 1        | Microcontroller board                                        | [Aries Micro](https://robu.in/product/aries-micro-v1-0-devlopment-board/)           |
| NodeMCU  | 1        | For updating real-time data through WiFi  | [NodeMCU](https://robu.in/product/nodemcu-esp8266-v3-lua-ch340-wifi-dev-board/)        |
|Ultrasonic sensors           | 4        |For obstacle detection and autonomous navigation | [Ultrasonic sensor](https://robu.in/product/3-3-5-5v-hc-sr04-ultrasonic-sensor-4pin/)       |
| Gas sensor       | 1        | Detects harmful gases in the environment| [Gas sensor](https://robu.in/product/mq-2-mq2-smoke-gas-lpg-butane-hydrogen-gas-sensor-detector-module/)        |
| DHT11 Sensor| 1| Temperature and humidity sensor for environmental monitoring| [DHT11 Sensor](https://robu.in/product/dht11-temperature-and-humidity-sensor-module-with-led/)|
| Motor driver          | 1|    Controls the movement of motors in the chassis  | [Motor Driver](https://robu.in/product/l298n-2a-based-motor-driver-module-good-quality/)       |
|Buzzer         | 1|Provides alerts when a hazard or survivor is detected | [Buzzer](https://robu.in/product/5v-active-alarm-buzzer-module-arduino/)        |
| Chasis           | 1        | Supports the robot’s movement across rough terrains| [Chasis]( https://amzn.in/d/5oan4ms )|
| PCB          | 1        | Used for assembling and connecting electronic components | [PCB](https://robu.in/product/12-x-18-cm-universal-pcb-prototype-board-single-sided-2-54mm-hole-pitch/)         |
| USB Cable  | 1        | For programming and power supply                             | [Micro USB Cable](https://amzn.in/d/1Sg2bNx)        |

## Table for Pin Connections
| **Component**          | **Aries Micro v1.0 Pin**   | **Description**                   |
|------------------------|--------------------------|-----------------------------------|
| **Ultrasonic Sensor (Front)** | Trig → `3` <br> Echo → `0`  | Detects obstacles in front      |
| **Ultrasonic Sensor (Left)**  | Trig → `11` <br> Echo → `10` | Detects obstacles on the left   |
| **Ultrasonic Sensor (Right)** | Trig → `14` <br> Echo → `13` | Detects obstacles on the right  |
| **Ultrasonic Sensor (Rear)**  | Trig → `8` <br> Echo → `9`   | Detects obstacles at the rear (Specify pins) |
| **Gas Sensor (MQ-8)**  | `A0`                     | Detects harmful gases           |
| **DHT11 Sensor**       | `7`                      | Measures temperature and humidity |
| **Motor Driver (L298N / L293D)** | IN1 → `1` <br> IN2 → `2` <br> IN3 → `4` <br> IN4 → `5` | Controls motors for movement |
| **Buzzer**             | `15`                     | Alerts in case of gas detection |
| **NodeMCU (ESP8266) (for IoT)** | `TX/RX (Soft Serial)` | Sends data to the cloud |
| **Power Supply (Battery Pack)** | `VIN & GND` | Provides power to the system |


## Pinout Diagram
![image](https://github.com/vls-midhuna/Search-and-Rescue-Bot/blob/97575cc2cb26cd390d03236eb81e97b936c93da1/Pinout(SAR).png)

## Working Code
```
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
```

## Demo Video
https://github.com/vls-midhuna/Search-and-Rescue-Bot/blob/main/search_and_rescue_bot.mp4

## Acknowledgements
I’m grateful for the opportunity to participate in the DIRV Hackathon 2025, organized by IIT Madras in collaboration with VSD, as part of the 2nd Digital India RISC-V (DIR-V) Symposium. It’s exciting to be part of this 4th Product-based Hackathon PAN India and the 3rd RISC-V-focused event, especially with such a reputed institute. This experience has been a great learning opportunity, allowing me to apply my skills, work with like-minded individuals, and contribute to real-world innovation. A big thanks to the organizers for making this possible!



