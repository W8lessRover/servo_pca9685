#ifndef CONFIG_H
#define CONFIG_H

// I2C address for PCA9685
#define PCA9685_ADDR 0x40  

// PCA9685 output enable pin (wired to OE pin, active low)
#define OE_PIN 7

// Servo timing (in microseconds)
#define SERVO_MIN_US 1000
#define SERVO_MAX_US 2000
#define SERVO_DEFAULT_US 1500

// Number of servos used
#define NUM_SERVOS 16  

#endif
