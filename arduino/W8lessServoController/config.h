#ifndef CONFIG_H
#define CONFIG_H

#define PCA9685_ADDR 0x40  // I2C address

#define OE_PIN 7           // Output Enable (active low)

#define SERVO_MIN_US 1000
#define SERVO_MAX_US 2000
#define SERVO_DEFAULT_US 1500

#define NUM_SERVOS 16

#endif
