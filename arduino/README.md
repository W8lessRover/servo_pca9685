# Arduino Setup for Servo PCA9685

## Connections
- PCA9685 → Arduino Uno
  - SDA → A4
  - SCL → A5
  - VCC → 5V
  - GND → GND
  - OE → Pin 7 (output enable)
- Servos powered externally if many are used
- LED_PIN 13 used as safety indicator

## Instructions
1. Open `W8lessServoController.ino` in Arduino IDE.
2. Install Adafruit PCA9685 library if needed.
3. Upload to Arduino Uno.
4. Outputs are initially disabled. Use Flask frontend to enable.
5. Safety feature: If frontend disconnects >1s, outputs are disabled and enabled servos reset.
6. All servos move to default before enabling.
