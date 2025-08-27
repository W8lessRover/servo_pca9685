# Servo PCA9685 Flask App

This Flask application provides a web-based interface to control a **PCA9685 servo controller** connected via an Arduino. It allows real-time control of up to 16 servos, with sliders, group controls, and optional joystick input.

---

## **Features**

- **Individual servo control**: Each of the 16 servos has its own slider and displays its current setpoint.
- **All Servos Control**: 
  - Single slider to adjust all servos at once.
  - Buttons for **All Min (1000µs)**, **All Mid (1500µs)**, **All Max (2000µs)**.
- **Output Enable/Disable**: Separate buttons to enable or disable the PCA9685 outputs.
- **Joystick support**: Map gamepad axes to individual servos for real-time control.
- **Real-time updates**: Servo values are updated every 500ms from the Arduino.
- **Serial feedback**: Commands sent to the Arduino are confirmed via serial responses.

---

## **Requirements**

Install the required Python packages (see `requirements.txt`):

```bash
pip install -r requirements.txt
