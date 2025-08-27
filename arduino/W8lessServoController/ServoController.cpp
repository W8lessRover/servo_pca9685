#include <Arduino.h>
#include "ServoController.h"

ServoController::ServoController() {
    for (int i = 0; i < NUM_SERVOS; i++) {
        servoValues[i] = DEFAULT_SERVO_US;
    }
}

void ServoController::begin() {
    pinMode(OE_PIN, OUTPUT);
    digitalWrite(OE_PIN, HIGH); // disable outputs initially

    Wire.begin();

    // Reset MODE1
    Wire.beginTransmission(PCA9685_ADDR);
    Wire.write(0x00);
    Wire.write(0x00);
    Wire.endTransmission();

    // Calculate prescale for 50Hz
    float prescaleval = 25000000.0 / (4096.0 * 50.0) - 1.0;
    uint8_t prescale = (uint8_t)(prescaleval + 0.5);

    // Enter sleep to set prescale
    Wire.beginTransmission(PCA9685_ADDR);
    Wire.write(0x00);
    Wire.endTransmission();
    Wire.requestFrom(PCA9685_ADDR, 1);
    uint8_t oldmode = Wire.read();
    uint8_t newmode = (oldmode & 0x7F) | 0x10; // sleep
    Wire.beginTransmission(PCA9685_ADDR);
    Wire.write(0x00);
    Wire.write(newmode);
    Wire.endTransmission();

    // Set prescale
    Wire.beginTransmission(PCA9685_ADDR);
    Wire.write(0xFE);
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

void ServoController::setServo(int channel, int microseconds) {
    if (channel < 0 || channel >= NUM_SERVOS) return;
    if (microseconds < SERVO_MIN_US) microseconds = SERVO_MIN_US;
    if (microseconds > SERVO_MAX_US) microseconds = SERVO_MAX_US;
    servoValues[channel] = microseconds;
    uint16_t ticks = usToTicks(microseconds);
    setPWM(channel, 0, ticks);
}

int ServoController::getServoValue(int channel) {
    if (channel < 0 || channel >= NUM_SERVOS) return DEFAULT_SERVO_US;
    return servoValues[channel];
}

void ServoController::setAllDefault() {
    for (int i = 0; i < NUM_SERVOS; i++) {
        setServo(i, DEFAULT_SERVO_US);
    }
}

void ServoController::enableOutputs(bool enable) {
    digitalWrite(OE_PIN, enable ? LOW : HIGH); // active low
}

uint16_t ServoController::usToTicks(uint16_t microseconds) {
    // 4096 ticks per 20ms frame
    return (uint16_t)((microseconds * 4096.0) / 20000.0);
}

void ServoController::setPWM(int channel, uint16_t on, uint16_t off) {
    Wire.beginTransmission(PCA9685_ADDR);
    Wire.write(0x06 + 4 * channel); // LEDn_ON_L
    Wire.write(on & 0xFF);
    Wire.write(on >> 8);
    Wire.write(off & 0xFF);
    Wire.write(off >> 8);
    Wire.endTransmission();
}
