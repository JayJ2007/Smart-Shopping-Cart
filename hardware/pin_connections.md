# 🔌 Pin Configuration – Smart Shopping Cart

This document describes the pin connections used in the **Smart Shopping Cart (IoT)** project, as defined in the firmware source code.

---

## ESP8266 (NodeMCU) – Movement & Auto-Follow Unit

**Function:**  
Controls DC motors and reads IR and ultrasonic sensors to enable autonomous cart-following behavior.

---

### Motor Driver Connections

| ESP8266 Pin | GPIO | Motor Driver Pin | Description |
|------------|------|------------------|-------------|
| D1 | GPIO5 | IN1 | Front Left Motor |
| D2 | GPIO4 | IN2 | Front Right Motor |
| D3 | GPIO0 | IN3 | Back Left Motor |
| D4 | GPIO2 | IN4 | Back Right Motor |
| D6 | GPIO12 | ENA | Left Motor Speed (PWM) |
| D7 | GPIO13 | ENB | Right Motor Speed (PWM) |

---

### IR Sensor Connections

| Sensor | ESP8266 Pin | GPIO |
|------|------------|------|
| Left IR Sensor | D0 | GPIO16 |
| Right IR Sensor | D8 | GPIO15 |

**Note:**  
IR sensors are configured as **active LOW** in firmware (`IR_ACTIVE LOW`).  
Change to `HIGH` if your IR sensors output HIGH on detection.

---

### Ultrasonic Sensor (HC-SR04)

| HC-SR04 Pin | ESP8266 Pin | GPIO |
|------------|------------|------|
| TRIG | RX | GPIO3 |
| ECHO | TX | GPIO1 |

**Note:**  
Using RX/TX pins disables serial debugging while the ultrasonic sensor is active.

---

## ESP32 – Billing, RFID & Display Unit

**Function:**  
Handles RFID authentication, Wi-Fi communication, LCD display, and billing logic.

---

### RFID Reader (RC522 – SPI Interface)

| RC522 Pin | ESP32 Pin | GPIO |
|----------|-----------|------|
| SDA (SS) | GPIO15 | 15 |
| RST | GPIO5 | 5 |
| MOSI | VSPI MOSI | 23 |
| MISO | VSPI MISO | 19 |
| SCK | VSPI SCK | 18 |
| 3.3V | 3.3V | — |
| GND | GND | — |

**Note:**  
SPI uses the ESP32 default **VSPI** configuration.

---

### LCD Display (16×2 I2C)

| LCD Signal | ESP32 Pin |
|-----------|-----------|
| SDA | GPIO21 |
| SCL | GPIO22 |
| I2C Address | `0x27` |

---

### Network Communication

- ESP32 connects to a local Wi-Fi network
- Runs an HTTP server on **port 80**
- Mobile app sends billing data using:

