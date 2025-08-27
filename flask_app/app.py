from flask import Flask, render_template, request, jsonify
import serial
import threading
import time

SERIAL_PORT = '/dev/ttyACM1'
ser = serial.Serial(SERIAL_PORT, 115200, timeout=0.1)

app = Flask(__name__)

servo_values = [1500]*16
outputs_enabled = False

def read_serial():
    while True:
        try:
            line = ser.readline().decode().strip()
            if line:
                print(line)
        except:
            pass

threading.Thread(target=read_serial, daemon=True).start()

# Optional heartbeat
def heartbeat_loop():
    while True:
        try:
            ser.write(b'HEARTBEAT\n')
        except:
            pass
        time.sleep(0.5)

threading.Thread(target=heartbeat_loop, daemon=True).start()

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/get_values')
def get_values():
    return jsonify({"values": servo_values})

@app.route('/set_servo', methods=['POST'])
def set_servo():
    data = request.get_json()
    ch, val = data['channel'], data['value']
    servo_values[ch] = val
    ser.write(f"SET {ch} {val}\n".encode())
    return jsonify({"status": "ok"})

@app.route('/set_all', methods=['POST'])
def set_all():
    val = request.get_json()['value']
    for i in range(16):
        servo_values[i] = val
    ser.write(f"SET_ALL {val}\n".encode())
    return jsonify({"status": "ok"})

@app.route('/enable_output', methods=['POST'])
def enable_output():
    global outputs_enabled
    ser.write(b'ENABLE\n')
    outputs_enabled = True
    return jsonify({"status": "ok"})

@app.route('/disable_output', methods=['POST'])
def disable_output():
    global outputs_enabled
    ser.write(b'DISABLE\n')
    outputs_enabled = False
    return jsonify({"status": "ok"})

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=5666, debug=True)
