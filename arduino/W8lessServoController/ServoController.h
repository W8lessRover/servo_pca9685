#ifndef SERVOCONTROLLER_H
#define SERVOCONTROLLER_H

#include <Wire.h>
#include <Arduino.h>
#include "config.h"

class ServoController {
  public:
    ServoController();
    void begin();
    void setServo(int channel, int microseconds);
    int getServoValue(int channel);
    void setAllDefault();
    void enableOutputs(bool enable);

  private:
    uint16_t servoValues[NUM_SERVOS];
    uint16_t usToTicks(uint16_t microseconds);
    void setPWM(int channel, uint16_t on, uint16_t off);
};

#endif
