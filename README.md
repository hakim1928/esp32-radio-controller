# esp32-radio-controller
esp32 radio controller
# ESP32 Radio Controller v1.0

An open-hardware, open-firmware **ESP32-based Radio Controller** featuring:
- dual joysticks (4 analog axes)
- dual KY-040 rotary encoders
- multiple switches and buttons
- OLED display with multi-view UI
- nRF24L01 wireless transmission
- fully open-source firmware (Unlicense)

This project is designed as a modular **DIY radio transmitter** for robotics, RC vehicles, drones, and remote control systems.

---

## ✨ Features

### 🎛 Inputs
- **4 analog axes**  
  - LeftV (GPIO 35)  
  - LeftH (GPIO 34)  
  - RightV (GPIO 32)  
  - RightH (GPIO 33)

- **Rotary Encoder #1** (menu navigation)  
  - CLK = GPIO 25  
  - DT  = GPIO 26  
  - SW  = GPIO 27  

- **Rotary Encoder #2**  
  - CLK = GPIO 16  
  - DT  = GPIO 12  
  - SW  = GPIO 13  

- **Switches**  
  - SW_A = GPIO 0  
  - SW_B = GPIO 2  
  - SW_C = GPIO 15

- **Extra switches**  
  - SW_1 = GPIO 4  
  - SW_2 = GPIO 14  
  - SW_3 = GPIO 36  

### 📡 Wireless
- **nRF24L01**  
  - CE  = GPIO 17  
  - CSN = GPIO 5  
  - SCK = GPIO 18  
  - MOSI = GPIO 23  
  - MISO = GPIO 19  

### 🖥 Display
- **OLED SSD1306 I²C**
  - SDA = GPIO 21  
  - SCL = GPIO 22  

- Five views:  
  1. Splash screen  
  2. Joystick dashboard  
  3. Switch status  
  4. Encoder #2 values  
  5. (reserved for future use)

---

## 🛠 Dependencies

Install these Arduino libraries :

- **AiEsp32RotaryEncoder**  
- **Adafruit SSD1306**  
- **Adafruit GFX**  
- **RF24**  
- **ESP32 Core for Arduino**

---

## 📡 Wiring Summary

ESP32
├── Joysticks: GPIO 35 / 34 / 32 / 33
├── Encoder #1: 25 / 26 / 27
├── Encoder #2: 16 / 12 / 13
├── Switches: 0 / 2 / 15 / 4 / 14 / 36
├── OLED SSD1306: 21 (SDA) + 22 (SCL)
└── nRF24L01: 17 / 5 / 18 / 23 / 19

---

## 🚀 Getting Started

1. Install ESP32 boards in Arduino IDE  
2. Install required libraries  
3. Flash `esp32-radio-controller.ino`  
4. Power via USB or LiPo + regulator  
5. The OLED boots into the splash screen  
6. Rotate **Encoder #1** to navigate views  
7. Press SW on Encoder #1 to go back to view #1

---

## 🧩 Folder Structure

esp32-radio-controller/
├── src/
│ └── esp32-radio-controller.ino
├── README.md
├── LICENSE
└── docs/
└── wiring-diagram.png


---

## 📜 License — Unlicense

This project is **public domain**.  
You may freely use, modify, distribute, sell, or include this work in any project without restriction.

See: https://unlicense.org/

---

## 🗺 Roadmap

- [ ] Add calibration menu  
- [ ] Add battery voltage monitoring  
- [ ] Add failsafe / watchdog  
- [ ] Add configurable center/limits per axis  
- [ ] Add telemetry return via nRF24  
- [ ] Add storage in NVS  

