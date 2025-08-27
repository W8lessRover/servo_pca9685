# Flask Web Frontend for Servo Control

This Flask app provides a web-based interface for controlling servos via an Arduino Uno + PCA9685.

## Features
- Web sliders to adjust servo positions
- Real-time servo feedback display
- Big Output Enable button (red/green)
- Joystick control (via Gamepad API in browser)

## Files
- `app.py` → Flask backend
- `requirements.txt` → Python dependencies
- `templates/index.html` → Web UI layout
- `static/script.js` → Client-side JavaScript (sliders + joystick)

## Installation
```bash
cd flask_app
pip install -r requirements.txt
python app.py
