# ESP32 EngiQ Door Sensor  

This project uses an **ESP32**, a **magnetic door sensor**, and a **16x2 I2C LCD** to detect door open/close states and display the status. The ESP32 connects to a **WPA2 Enterprise Wi-Fi** and can optionally send updates (e.g., to Discord via Webhook). 

Running code in folder code-sensor/code-sensor.ini

---

## Features  
- Connects securely to any WPA2Enterprise network (like eduroam)  
- Reads a **door sensor (magnetic reed switch)**  
- Displays door status (`OPEN` / `CLOSED`) on an **LCD (I2C)**
- Optional: send notifications to a **Discord channel** via webhook
---

## Hardware Requirements  
- ESP32 (or equivalent board)  
- Magnetic door sensor (reed switch)  
- 16x2 LCD with I2C backpack  
- Jumper wires + breadboard (or soldered connections)  

---

## Wiring  

The door sensor is an Normally Closed type circuit.

| Component         | ESP32 Pin | Notes                  |
|-------------------|-----------|------------------------|
| LCD (SDA)         | 21        | I2C SDA default        |
| LCD (SCL)         | 22        | I2C SCL default        |
| Door Sensor (OUT) | 34        | GPIO input only        |
| Door Sensor (VCC) | 3.3V      |                        |
| Door Sensor (GND) | GND       |                        |

---

## Software Requirements  
- [Arduino IDE](https://www.arduino.cc/en/software)

- ESP32 Board Support (v3.0.0+) → Install from **Boards Manager**
  - found at: https://dl.espressif.com/dl/package_esp32_index.json 
- Libraries:  
  - `LiquidCrystal_I2C` (LCD driver)  
  - `WiFi.h` (bundled with ESP32 Arduino core)  
  - `esp_wpa2.h` (bundled with ESP32 Arduino core, needed for wpa2)

---

## Setup  

1. Clone the repo and open the `.ino` sketch in Arduino IDE.  
2. In the code, update your **WPA2 login**, for this case we'll use EDUROAM a enterprise network used for universities:  

   ```cpp
   const char* ssid = "eduroam";
   const char* username = "your_netname";
   const char* password = "your_password";
   ```  

3. Select your ESP32 board in **Tools → Board → ESP32 dev Arduino**.  
4. Select the correct COM/USB port.  
5. Upload the sketch. 

---

## Usage  

- When powered on, the ESP32 will connect to **eduroam**.  
- The LCD will display the door state:  

  ```
  Door: OPEN
  Door: CLOSED
  ```  
- A switch was also installed as an override to show the door is still open. 
- If Discord webhook integration is enabled, a message will be sent whenever the door state changes.  

---

## Troubleshooting  

- **LCD shows nothing** → Adjust the contrast pot on the I2C backpack.  
- **Not connecting to eduroam** → Double-check username format (usually `username@university.edu`) and password. Some institutions require installing root CA certificates (advanced).  
- **Compilation errors with `esp_eap_client`** → Stick with `esp_wpa2.h` in Arduino IDE. `esp_eap_client` is ESP-IDF only.  

---

## Roadmap  
- Create a proper encasing for the board
- Improve error handling for Wi-Fi reconnection
- Modularize Discord webhook integration to be able to read inputs from chanels instead of just output from webhooks, running mini-python might do the trick
---

## License  
MIT License. Free to use, modify, and share.  
