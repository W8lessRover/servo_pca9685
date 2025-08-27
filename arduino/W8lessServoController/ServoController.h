#ifndef SERVO_CONTROLLER_H
#define SERVO_CONTROLLER_H

#include <Wire.h>
#include "config.h"

class ServoController {
public:
    ServoController();
    void begin();
    void setServo(uint8_t channel, uint16_t microseconds);
    void setAllDefault();
    void enableOutputs(bool enable);
    uint16_t getServoValue(uint8_t channel);

private:
    uint16_t servoValues[NUM_SERVOS];
    void setPWM(uint8_t channel, uint16_t on, uint16_t off);
    uint16_t usToTicks(uint16_t microseconds);
};

#endif
