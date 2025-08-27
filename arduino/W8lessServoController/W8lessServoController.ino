#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "config.h"

// PCA9685 instance (default address 0x40)
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

unsigned long lastCommandTime = 0;   // Last time a command was received
bool outputsEnabled = false;         // OE pin state
bool safetyActive = false;           // Safety shutdown active
bool servoEnabled[NUM_SERVOS];       // Track enabled servos

void setup() {
  Serial.begin(115200);
  pwm.begin();
  pwm.setPWMFreq(50); // 50Hz standard for servos
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

  // Safety timeout: disable outputs if no command for FRONTEND_TIMEOUT_MS
  if (!safetyActive && millis() - lastCommandTime > FRONTEND_TIMEOUT_MS) {
    // Disable outputs first
    digitalWrite(OE_PIN, HIGH);
    outputsEnabled = false;

    // Move enabled servos to default
    for (int i = 0; i < NUM_SERVOS; i++) {
      if (servoEnabled[i]) {
        setServoMicroseconds(i, DEFAULT_SERVO_US);
        servoEnabled[i] = false;
      }
    }

    safetyActive = true;
  }

  // Blink LED when safety active
  if (safetyActive) {
    digitalWrite(LED_PIN, millis() % 1000 < 500 ? HIGH : LOW);
  }
}

// -------------------------------
// Set PWM for a single servo
// -------------------------------
void setServoMicroseconds(int channel, int us) {
  us = constrain(us, SERVO_MIN_US, SERVO_MAX_US);
  int pulselen = map(us, 0, 20000, 0, 4095);
  pwm.setPWM(channel, 0, pulselen);
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
    servoEnabled[ch] = true;

  } else if (line.startsWith("SET_ALL ")) {
    int val = line.substring(8).toInt();
    for (int i = 0; i < NUM_SERVOS; i++) {
      setServoMicroseconds(i, val);
      servoEnabled[i] = true;
    }

  } else if (line == "ENABLE") {
    // Enable outputs first
    digitalWrite(OE_PIN, LOW);
    outputsEnabled = true;

    // Set all servos to default
    for (int i = 0; i < NUM_SERVOS; i++) {
        setServoMicroseconds(i, DEFAULT_SERVO_US);
        servoEnabled[i] = true;
    }

  } else if (line == "DISABLE") {
    // Disable outputs
    digitalWrite(OE_PIN, HIGH);
    outputsEnabled = false;

    // Move enabled servos to default
    for (int i = 0; i < NUM_SERVOS; i++) {
        if (servoEnabled[i]) {
            setServoMicroseconds(i, DEFAULT_SERVO_US);
            servoEnabled[i] = false;
        }
    }
  }

  // Feedback to frontend
  Serial.print("[Arduino] Received: ");
  Serial.println(line);
}
