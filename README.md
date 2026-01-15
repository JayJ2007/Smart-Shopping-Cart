# Smart Shopping Cart (IoT)

An IoT-based smart shopping cart designed to automate product billing and provide hands-free cart movement using sensors, RFID authentication, and wireless communication.

## Overview

The Smart Shopping Cart improves the in-store shopping experience by allowing users to scan products while shopping and automatically track their total bill in real time.  
The cart also follows the user autonomously using IR and ultrasonic sensors, reducing physical effort and checkout time.

The system uses two microcontrollers:
- **ESP32** for billing, RFID authentication, LCD display, and mobile app communication
- **ESP8266** for motor control and autonomous following behavior

---

## Key Features

- RFID-based user authentication  
- Barcode-based product scanning via mobile app  
- Real-time bill display on LCD  
- Autonomous cart-following using IR + ultrasonic sensors  
- Dual-microcontroller architecture for parallel tasks  

---

## System Architecture

1. User authenticates using an RFID card  
2. ESP32 grants access and initializes the system  
3. Products are scanned via a mobile app and sent to ESP32  
4. Bill is updated and displayed on the LCD  
5. ESP8266 continuously tracks user position and controls motors  

---

## Hardware Components

- ESP32  
- ESP8266 (NodeMCU)  
- RC522 RFID Module  
- Ultrasonic Sensor (HC-SR04)  
- IR Sensors  
- Motor Driver Module  
- DC Motors & Wheels  
- 16x2 I2C LCD Display  
- Power Supply  

---

## Software Stack

- **Firmware:** Arduino (ESP32 & ESP8266)
- **Communication:** Wi-Fi (HTTP)
- **Display:** I2C LCD
- **Mobile App:** Barcode scanning (external app)

---

## Firmware Structure

| Module | Description |
|------|------------|
| ESP8266 | Motor control and auto-follow logic |
| ESP32 | RFID authentication, billing, LCD, and web server |

---

## Working Principle

1. System initializes sensors, LCD, motors, and RFID  
2. User scans authorized RFID card  
3. ESP32 connects to Wi-Fi and enables billing mode  
4. Products are scanned via mobile app  
5. ESP8266 follows the user using distance and direction data  
6. Bill is finalized and displayed  

---

## Limitations

- Requires stable Wi-Fi connectivity  
- Optimized for indoor environments  
- IR sensors affected by ambient lighting  
- No side obstacle detection  

---

## Future Improvements

- Online payment gateway integration  
- AI-based vision tracking  
- Voice control support  
- Advanced obstacle avoidance  
- Cloud-based billing and receipt generation  

---

## Project Images

Images of the working prototype are available in the `images/` directory.

---




