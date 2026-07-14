#include "BluetoothSerial.h"
#include <FastLED.h>
#include <ESP32Servo.h> 

BluetoothSerial SerialBT;
Servo myServo;

// --- PIN CONFIGURATION ---
#define SERVO_PIN   14    // Orange/Yellow wire here
#define LED_PIN     13    // DIN wire here
#define NUM_LEDS    7
#define BRIGHTNESS  100

#define IN1 19
#define IN2 18
#define IN3 26
#define IN4 27
#define ENA 23
#define ENB 5

CRGB leds[NUM_LEDS];
int servoPos = 90; 

void setup() {
  Serial.begin(9600);
  SerialBT.begin("ESP32_BOT");

  // Motor Pins
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  digitalWrite(ENA, HIGH); digitalWrite(ENB, HIGH);

  // LED Setup
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  fill_solid(leds, NUM_LEDS, CRGB::Blue); // Start Blue
  FastLED.show();

  // Servo Setup (Specific for ESP32)
  ESP32PWM::allocateTimer(0);
  myServo.setPeriodHertz(50);
  myServo.attach(SERVO_PIN, 500, 2400); 
  myServo.write(servoPos); 

  Serial.println("System Ready - Connection Fixed");
}

void loop() {
  if (SerialBT.available()) {
    char cmd = SerialBT.read();
    if (cmd == '\n' || cmd == '\r' || cmd == ' ') return;

    Serial.print("Received: "); Serial.println(cmd);

    // --- Movement & LED Color Logic ---
    if (cmd == 'F') { // Forward
      moveMotors(HIGH, LOW, HIGH, LOW);
      fill_solid(leds, NUM_LEDS, CRGB::Green); // Green for Go
    }
    else if (cmd == 'B') { // Backward
      moveMotors(LOW, HIGH, LOW, HIGH);
      fill_solid(leds, NUM_LEDS, CRGB::Red); // Red for Reverse/Warning
    }
    else if (cmd == 'L') { // Left (Corrected Direction)
      moveMotors(HIGH, LOW, LOW, HIGH);
      fill_solid(leds, NUM_LEDS, CRGB::Yellow);
    }
    else if (cmd == 'R') { // Right (Corrected Direction)
      moveMotors(LOW, HIGH, HIGH, LOW);
      fill_solid(leds, NUM_LEDS, CRGB::Orange);
    }
    else if (cmd == 'S') { // Stop
      moveMotors(LOW, LOW, LOW, LOW);
      fill_solid(leds, NUM_LEDS, CRGB::Blue);
    }

    // --- Servo Control (F1 and F2) ---
    // BT Car Controller sends 'W' for F1 and 'U' for F2
    if (cmd == 'W' || cmd == '1') { 
      servoPos = 45; // Turn Left
      myServo.write(servoPos);
    }
    else if (cmd == 'U' || cmd == '2') { 
      servoPos = 135; // Turn Right
      myServo.write(servoPos);
    }
    // F3 (usually 'V' or '3') to Center Servo
    else if (cmd == 'V' || cmd == '3') {
      servoPos = 90;
      myServo.write(servoPos);
      fill_solid(leds, NUM_LEDS, CRGB::White); // "Headlight" mode
    }
    
    FastLED.show();
  }
}

void moveMotors(int i1, int i2, int i3, int i4) {
  digitalWrite(IN1, i1); digitalWrite(IN2, i2);
  digitalWrite(IN3, i3); digitalWrite(IN4, i4);
}