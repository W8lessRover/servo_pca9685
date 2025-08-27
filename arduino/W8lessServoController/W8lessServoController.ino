#include "ServoController.h"

ServoController sc;

void setup() {
    Serial.begin(115200);
    sc.begin();
    sc.enableOutputs(false); // start disabled
    Serial.println("READY"); // indicate startup complete
}

void loop() {
    // Listen for commands from Flask (via serial/USB)
    if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();

        if (cmd.startsWith("SETALL")) {
            int val;
            if (sscanf(cmd.c_str(), "SETALL %d", &val) == 1) {
                for (int i = 0; i < NUM_SERVOS; i++) {
                    sc.setServo(i, val);
                }
                Serial.print("ACK SETALL ");
                Serial.println(val);
            }
        }
        else if (cmd.startsWith("SET")) {
            int ch, val;
            if (sscanf(cmd.c_str(), "SET %d %d", &ch, &val) == 2) {
                sc.setServo(ch, val);
                Serial.print("ACK SET ");
                Serial.print(ch);
                Serial.print(" ");
                Serial.println(val);
            }
        }
        else if (cmd == "ENABLE") {
            sc.enableOutputs(true);
            Serial.println("ACK ENABLE");
        }
        else if (cmd == "DISABLE") {
            sc.enableOutputs(false);
            Serial.println("ACK DISABLE");
        }
        else if (cmd.startsWith("GET")) {
            int ch;
            if (sscanf(cmd.c_str(), "GET %d", &ch) == 1) {
                Serial.print("VAL ");
                Serial.print(ch);
                Serial.print(" ");
                Serial.println(sc.getServoValue(ch));
            }
        }
        else {
            Serial.print("ERR UNKNOWN CMD: ");
            Serial.println(cmd);
        }
    }
}
