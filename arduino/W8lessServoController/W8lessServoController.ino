#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "config.h"

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

unsigned long lastCommandTime = 0; // Track last command from frontend
bool outputsEnabled = false;       // Track if OE is enabled
bool safetyActive = false;         // Safety shutdown active
bool servoEnabled[NUM_SERVOS];     // Track which servos are currently enabled

void setup() {
  Serial.begin(115200);
  pwm.begin();
  pwm.setPWMFreq(50); // Standard servo frequency
  pinMode(OE_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  // Start with outputs disabled
  digitalWrite(OE_PIN, HIGH);
  for (int i = 0; i < NUM_SERVOS; i++) {
    servoEnabled[i] = false;
    setServoMicroseconds(i, DEFAULT_SERVO_US);
  }
}

void loop() {
  // Process serial commands
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    processSerialCommand(line);
    lastCommandTime = millis();
    safetyActive = false;
    digitalWrite(LED_PIN, LOW);
  }

  // Safety timeout: frontend disconnected > FRONTEND_TIMEOUT_MS
  if (!safetyActive && millis() - lastCommandTime > FRONTEND_TIMEOUT_MS) {
    for (int i = 0; i < NUM_SERVOS; i++) {
      if (servoEnabled[i]) setServoMicroseconds(i, DEFAULT_SERVO_US);
    }
    digitalWrite(OE_PIN, HIGH); // Disable all outputs
    safetyActive = true;
  }

  // Blink LED during safety
  if (safetyActive) {
    digitalWrite(LED_PIN, millis() % 1000 < 500 ? HIGH : LOW);
  }
}

// -------------------------------
// Set PWM for a single servo
// -------------------------------
void setServoMicroseconds(int channel, int us) {
  us = constrain(us, 1000, 2000);
  int pulselen = map(us, 0, 20000, 0, 4095); // 20ms period -> 50Hz
  pwm.setPWM(channel, 0, pulselen);
  servoEnabled[channel] = true; // Mark this servo as enabled
}

// -------------------------------
// Process serial commands
// -------------------------------
void processSerialCommand(String line) {
  line.trim();
  if (line.startsWith("SET ")) {
    int ch = line.substring(4, line.indexOf(' ', 4)).toInt();
    int val = line.substring(line.lastIndexOf(' ') + 1).toInt();
    setServoMicroseconds(ch, val);
  } else if (line.startsWith("SET_ALL ")) {
    int val = line.substring(8).toInt();
    for (int i = 0; i < NUM_SERVOS; i++) setServoMicroseconds(i, val);
  } else if (line == "ENABLE") {
    digitalWrite(OE_PIN, LOW);
    outputsEnabled = true;
    for (int i = 0; i < NUM_SERVOS; i++) servoEnabled[i] = true;
  } else if (line == "DISABLE") {
    digitalWrite(OE_PIN, HIGH);
    outputsEnabled = false;
    for (int i = 0; i < NUM_SERVOS; i++) servoEnabled[i] = false;
  }

  // Feedback to frontend
  Serial.print("[Arduino] Received: ");
  Serial.println(line);
}
