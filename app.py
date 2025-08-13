from flask import Flask, render_template, request, jsonify
import serial
import threading

app = Flask(__name__)
ser = serial.Serial('COM3', 9600)  # Adjust COM port

latest_data = {} # Stores the latest sensor values(decleration).

def read_serial():
    global latest_data
    buzzer_on = False
    led0_on = False  # Track A2 LED state

    while True:
        try:
            line = ser.readline().decode().strip()
            if line:
                print("From Arduino:", line)
                parts = line.split(",")
                data = {}
                for part in parts:
                    if ":" in part:
                        k, v = part.split(":")
                        data[k] = v
                latest_data = data

                # ✅ Safe conversion check
                try:
                    temp = float(data.get("TEMP", 0))
                    gas = int(data.get("GAS", 0))
                    ldr = int(data.get("LDR", 0))
                except ValueError:
                    print("⚠️ Invalid sensor data, skipping this line")
                    continue

                if gas > 300 or temp > 27.5:
                    if not buzzer_on:
                        print("🚨 Alert: High gas or temperature detected")
                        ser.write(b"BUZZER:1\n")
                        buzzer_on = True
                else:
                    if buzzer_on:
                        print("✅ Safe levels restored")
                        ser.write(b"BUZZER:0\n")
                        buzzer_on = False
                
                # 💡 Check LDR value to control LED0 (A2)
                if ldr == 1 and not led0_on:
                    ser.write(b"LED0:1\n")
                    led0_on = True
                elif ldr == 0 and led0_on:
                    ser.write(b"LED0:0\n")
                    led0_on = False

        except Exception as e:
            print("Serial Read Error:", e)



@app.route('/') # Route 1
def index():
    return render_template("dashboard.html", data=latest_data)

@app.route('/get_data') # Route 2
def get_data():
    return jsonify(latest_data)

@app.route('/control', methods=['POST']) # Route 3
def control():
    device = request.json.get("device")
    index = request.json.get("index", "")
    state = request.json.get("state")

    cmd = ""
    if device == "LED":
        cmd = f"LED{index}:{state}"
    elif device == "BUZZER":
        cmd = f"BUZZER:{state}"

    ser.write((cmd + "\n").encode())
    return jsonify({"status": "ok", "sent": cmd})

if __name__ == '__main__':
    threading.Thread(target=read_serial, daemon=True).start()
    app.run(debug=True,use_reloader=False)
