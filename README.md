Absolutely! Here’s the **root `README.md`** in a **ready-to-copy format**:

---

```markdown
# Servo PCA9685 Project

This project provides a full-stack solution for controlling **PCA9685-based servos** via an Arduino Uno and a **web-based Flask frontend**. It supports individual servo control, group controls, output enable/disable, and optional joystick mapping.

---

## **Project Structure**

```

servo\_pca9685/
├── arduino/
│   ├── W8lessServoController.ino   # Main Arduino sketch
│   ├── config.h                    # Configuration constants
│   └── README.md                   # Arduino setup and wiring guide
├── flask\_app/
│   ├── app.py                      # Flask backend server
│   ├── requirements.txt            # Python dependencies
│   ├── README.md                   # Flask app usage guide
│   ├── templates/
│   │   └── index.html              # Frontend HTML
│   └── static/
│       └── script.js               # Frontend JS logic
└── README.md                       # This file

````

---

## **Arduino Setup**

1. Open **`W8lessServoController.ino`** in the Arduino IDE.  
2. Connect the PCA9685 to the Arduino via **I2C**:
   - `SDA` → `A4`
   - `SCL` → `A5`
   - `VCC` → 5V
   - `GND` → GND
   - `OE` → Arduino digital pin 7
3. Install the **Adafruit PCA9685 library** in Arduino IDE if not already installed.  
4. Verify and upload the sketch to the Arduino Uno.  
5. Ensure servos are powered properly (external 5V recommended if multiple servos).  
6. The OE pin is used to enable or disable servo outputs. The Flask frontend buttons control this.

---

## **Flask App Setup (Ubuntu 20.04)**

1. Install Python 3.8+ and pip.
2. Navigate to the `flask_app` directory:

```bash
cd servo_pca9685/flask_app
````

3. Create a virtual environment (optional but recommended):

```bash
python3 -m venv venv
source venv/bin/activate
```

4. Install dependencies:

```bash
pip install -r requirements.txt
```

5. Ensure your user has permission for serial access:

```bash
sudo usermod -a -G dialout $USER
```

Log out and back in to apply.

6. Start the Flask server:

```bash
python app.py
```

7. Open the frontend in your browser:

```
http://localhost:5666
```

---

## **Frontend Usage**

* **Individual Servo Sliders**: Adjust the position of each servo individually.
* **All Servos Slider**: Adjust all servos at once.
* **All Min/Mid/Max Buttons**: Quickly set all servos to 1000µs, 1500µs, or 2000µs.
* **Enable/Disable Buttons**: Toggle output to the PCA9685.
* **Joystick Mapping**: Map axes from a connected gamepad to individual servos.
* **Real-Time Feedback**: The current value of each servo is displayed next to its slider and updated periodically.

---

## **Arduino Notes**

* Servos use pulse widths from **1000µs to 2000µs**.
* Default mid position is **1500µs**.
* Ensure **OE pin** is wired to Arduino pin 7 (or configured in `config.h`).
* Multiple servos may require an **external 5V power source** to avoid overloading the Arduino.

---

## **Flask Notes**

* Serial port defaults to `/dev/ttyAMA0`. Update in `app.py` if needed.
* Flask runs on port **5666** by default.
* The frontend periodically polls the Arduino every 500ms to update slider values.
* Use the “Enable” button to allow servos to move, and “Disable” to stop outputs.

---

## **License**

This project is provided as-is for educational and personal use.

```

---

This is ready to **copy and paste directly** into `servo_pca9685/README.md`.  

Do you want me to also create the **`arduino/README.md`** now so each subfolder has its own guide?
```
