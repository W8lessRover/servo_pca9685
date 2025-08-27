# Arduino PCA9685 Servo Controller (W8lessServoController)

This Arduino sketch interfaces an Arduino Uno with a PCA9685 16-channel servo controller via I²C.

## Features
- Controls up to 16 servos
- Pulse range limited to **1000–2000 µs**
- Default servo position is **1500 µs**
- Supports enabling/disabling outputs via PCA9685 **OE** pin
- Communicates with a Flask frontend over USB Serial

## Files
- `W8lessServoController.ino` → Main Arduino sketch
- `ServoController.h / .cpp` → Servo control class
- `config.h` → Configuration constants (I²C address, pin numbers, etc.)

## Serial Commands
- `SET <channel> <us>` → Set servo `<channel>` to `<us>` microseconds
- `GET <channel>` → Get current servo value
- `ENABLE` → Enable servo outputs
- `DISABLE` → Disable servo outputs

## Usage
1. Open the folder `W8lessServoController/` in Arduino IDE.
2. Select **Arduino Uno** as the board.
3. Upload the sketch.
4. Ensure PCA9685 **OE** pin is connected to Arduino **pin 7**.
5. Connect Arduino to the host running the Flask frontend via USB Serial.
