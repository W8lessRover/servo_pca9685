#include "ServoController.h"
#include <Arduino.h>
ServoController::ServoController() {
    for (int i = 0; i < NUM_SERVOS; i++) {
        servoValues[i] = SERVO_DEFAULT_US;
    }
}

void ServoController::begin() {
    pinMode(OE_PIN, OUTPUT);
    digitalWrite(OE_PIN, HIGH); // disable outputs initially

    Wire.begin();

    // Reset MODE1 register
    Wire.beginTransmission(PCA9685_ADDR);
    Wire.write(0x00); // MODE1
    Wire.write(0x00); // Normal mode
    Wire.endTransmission();

    // Calculate prescale for ~50 Hz
    float prescaleval = 25000000.0;
    prescaleval /= 4096.0;
    prescaleval /= 50.0;
    prescaleval -= 1.0;
    uint8_t prescale = (uint8_t)floor(prescaleval + 0.5);

    // Enter sleep
    Wire.beginTransmission(PCA9685_ADDR);
    Wire.write(0x00); // MODE1
    Wire.endTransmission();
    Wire.requestFrom((int)PCA9685_ADDR, 1);
    uint8_t oldmode = Wire.read();
    uint8_t newmode = (oldmode & 0x7F) | 0x10; // sleep
    Wire.beginTransmission(PCA9685_ADDR);
    Wire.write(0x00); // MODE1
    Wire.write(newmode);
    Wire.endTransmission();

    // Set prescale
    Wire.beginTransmission(PCA9685_ADDR);
    Wire.write(0xFE); // PRE_SCALE
    Wire.write(prescale);
    Wire.endTransmission();

    // Restore MODE1
    Wire.beginTransmission(PCA9685_ADDR);
    Wire.write(0x00);
    Wire.write(oldmode);
    Wire.endTransmission();

    delay(5);

    // Restart
    Wire.beginTransmission(PCA9685_ADDR);
    Wire.write(0x00);
    Wire.write(oldmode | 0xA1); // auto-increment, restart
    Wire.endTransmission();

    setAllDefault();
}


void ServoController::setServo(uint8_t channel, uint16_t microseconds) {
    if (channel >= NUM_SERVOS) return;
    if (microseconds < SERVO_MIN_US) microseconds = SERVO_MIN_US;
    if (microseconds > SERVO_MAX_US) microseconds = SERVO_MAX_US;

    servoValues[channel] = microseconds;
    uint16_t ticks = usToTicks(microseconds);
    setPWM(channel, 0, ticks);
}

void ServoController::setAllDefault() {
    for (int i = 0; i < NUM_SERVOS; i++) {
        setServo(i, SERVO_DEFAULT_US);
    }
}

void ServoController::enableOutputs(bool enable) {
    digitalWrite(OE_PIN, enable ? LOW : HIGH);  
}

uint16_t ServoController::getServoValue(uint8_t channel) {
    if (channel < NUM_SERVOS) return servoValues[channel];
    return 0;
}

void ServoController::setPWM(uint8_t channel, uint16_t on, uint16_t off) {
    Wire.beginTransmission(PCA9685_ADDR);
    Wire.write(0x06 + 4 * channel);
    Wire.write(on & 0xFF);
    Wire.write(on >> 8);
    Wire.write(off & 0xFF);
    Wire.write(off >> 8);
    Wire.endTransmission();
}

uint16_t ServoController::usToTicks(uint16_t microseconds) {
    float ticks = (microseconds / 1000000.0) * 50.0 * 4096;
    return (uint16_t)ticks;
}
