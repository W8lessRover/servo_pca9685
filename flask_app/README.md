# Flask Frontend for Servo PCA9685

## Features
- Control 16 servos individually
- Set all servos at once (slider + Min/Mid/Max buttons)
- Enable/Disable PCA9685 output
- Safety: Arduino disables outputs if frontend disconnects >1s
- Joystick support for mapping axes to servos
- Real-time feedback

## Setup
1. Create virtual environment and install requirements:
python3 -m venv venv
source venv/bin/activate
pip install -r requirements.txt

markdown
Copy code
2. Start the Flask app:
python app.py

markdown
Copy code
3. Open browser: `http://localhost:5666`

## Notes
- Safety timeout and LED indicator managed by Arduino
- Default servo position: 1500µs
- Serial port defaults to `/dev/ttyAMA0`