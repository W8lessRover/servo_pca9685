# Servo PCA9685 Controller

This project provides a complete system for controlling up to 16 servos using an Arduino Uno and a PCA9685 servo controller, with a Flask web frontend for real-time control.

## Features
- Arduino Uno communicates with PCA9685 over I²C
- Servo pulse range: **1000–2000 µs** (default **1500 µs**)
- Uses PCA9685 Output Enable pin for safety
- Flask web frontend:
  - Big Output Enable button (red/green)
  - Real-time servo position sliders
  - Joystick control (via browser Gamepad API)
  - Works with up to 16 servos

## Project Layout
- `arduino/` → Arduino IDE project
- `flask_app/` → Flask frontend for control

## Requirements
- Arduino Uno
- PCA9685 16-channel servo controller
- Python 3.8+
- Flask

## Getting Started
1. Upload the sketch in `arduino/W8lessServoController/` to the Arduino.
2. Install Python requirements:  
   ```bash
   cd flask_app
   pip install -r requirements.txt
