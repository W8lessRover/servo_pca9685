#ifndef CONFIG_H
#define CONFIG_H

// PCA9685 Settings
#define NUM_SERVOS 16
#define DEFAULT_SERVO_US 1500
#define SERVO_MIN_US 1000
#define SERVO_MAX_US 2000
#define PCA9685_ADDR 0x40
// Arduino Pins
#define OE_PIN 7    // Output Enable
#define LED_PIN 13  // Safety indicator LED

// Safety Timeout (milliseconds)
#define FRONTEND_TIMEOUT_MS 1000

#endif
