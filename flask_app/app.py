from flask import Flask, render_template, request, jsonify
import serial
import threading
import time

app = Flask(__name__)

# -------------------------------
# Serial Configuration
# -------------------------------
SERIAL_PORT = "/dev/ttyACM1"  # Change to your device
BAUD_RATE = 115200

# Try to open serial port
try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    time.sleep(2)  # give Arduino time to reset
    print(f"[INFO] Serial port {SERIAL_PORT} opened.")
except serial.SerialException as e:
    print(f"[ERROR] Could not open serial port {SERIAL_PORT}: {e}")
    ser = None

NUM_SERVOS = 16
servo_values = [1500] * NUM_SERVOS
output_enabled = False

# -------------------------------
# Serial Reading Thread
# -------------------------------
def read_serial():
    global servo_values
    while True:
        if ser is None:
            time.sleep(1)
            continue
        try:
            if ser.in_waiting:
                line = ser.readline().decode(errors='ignore').strip()
                if line:
                    print(f"[Arduino] {line}")
                    # Parse servo value updates
                    if line.startswith("VAL"):
                        try:
                            _, ch, val = line.split()
                            servo_values[int(ch)] = int(val)
                        except ValueError:
                            pass
        except serial.SerialException as e:
            print(f"[ERROR] Serial exception: {e}")
            time.sleep(1)

# Start serial reading thread
threading.Thread(target=read_serial, daemon=True).start()

# -------------------------------
# Routes
# -------------------------------

@app.route('/')
def index():
    return render_template("index.html", servo_values=servo_values, output_enabled=output_enabled)

@app.route('/set_servo', methods=['POST'])
def set_servo():
    global servo_values
    if ser is None:
        return jsonify(success=False, error="Serial port not connected")
    try:
        ch = int(request.json['channel'])
        val = int(request.json['value'])
        cmd = f"SET {ch} {val}\n"
        ser.write(cmd.encode())
        print(f"[Sent] {cmd.strip()}")
        servo_values[ch] = val
        return jsonify(success=True)
    except Exception as e:
        return jsonify(success=False, error=str(e))

@app.route('/set_all', methods=['POST'])
def set_all():
    global servo_values
    if ser is None:
        return jsonify(success=False, error="Serial port not connected")
    try:
        val = int(request.json['value'])
        cmd = f"SETALL {val}\n"
        ser.write(cmd.encode())
        print(f"[Sent] {cmd.strip()}")
        for i in range(NUM_SERVOS):
            servo_values[i] = val
        return jsonify(success=True)
    except Exception as e:
        return jsonify(success=False, error=str(e))

@app.route('/enable_output', methods=['POST'])
def enable_output():
    global output_enabled
    if ser is None:
        return jsonify(success=False, error="Serial port not connected")
    ser.write(b"ENABLE\n")
    output_enabled = True
    print("[Sent] ENABLE")
    return jsonify(enabled=True)

@app.route('/disable_output', methods=['POST'])
def disable_output():
    global output_enabled
    if ser is None:
        return jsonify(success=False, error="Serial port not connected")
    ser.write(b"DISABLE\n")
    output_enabled = False
    print("[Sent] DISABLE")
    return jsonify(enabled=False)

@app.route('/get_values')
def get_values():
    return jsonify(values=servo_values, enabled=output_enabled)

# -------------------------------
# Main
# -------------------------------
if __name__ == '__main__':
    app.run(debug=True, host="0.0.0.0", port=5666)
