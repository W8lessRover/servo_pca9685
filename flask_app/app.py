from flask import Flask, render_template, request, jsonify
import serial
import threading

app = Flask(__name__)

# Default serial port (adjust if needed)
SERIAL_PORT = "/dev/ttyAMA0"
BAUD_RATE = 115200

# Open serial connection
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)

servo_values = [1500] * 16
output_enabled = False

def read_serial():
    global servo_values
    while True:
        if ser.in_waiting:
            line = ser.readline().decode().strip()
            if line.startswith("VAL"):
                try:
                    _, ch, val = line.split()
                    servo_values[int(ch)] = int(val)
                except ValueError:
                    pass  # ignore malformed lines

threading.Thread(target=read_serial, daemon=True).start()

@app.route('/')
def index():
    return render_template("index.html", servo_values=servo_values, output_enabled=output_enabled)

@app.route('/set_servo', methods=['POST'])
def set_servo():
    ch = int(request.json['channel'])
    val = int(request.json['value'])
    ser.write(f"SET {ch} {val}\n".encode())
    return jsonify(success=True)

@app.route('/toggle_output', methods=['POST'])
def toggle_output():
    global output_enabled
    output_enabled = not output_enabled
    ser.write(b"ENABLE\n" if output_enabled else b"DISABLE\n")
    return jsonify(enabled=output_enabled)

@app.route('/get_values')
def get_values():
    return jsonify(values=servo_values, enabled=output_enabled)

if __name__ == '__main__':
    app.run(debug=True, host="0.0.0.0", port=5666)
