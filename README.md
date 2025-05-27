# 🌱 Smart Water Planting System

A **Smart IoT-Based Plant Watering System** that automates the watering process of indoor and outdoor plants using **soil moisture sensors**, **weather forecast data**, and **Blynk IoT platform**. This project minimizes water wastage by detecting soil moisture levels and checking for rainy conditions before turning on a water motor.

---

## 📌 Table of Contents

- [Project Overview](#project-overview)
- [Features](#features)
- [Hardware Components](#hardware-components)
- [Software Requirements](#software-requirements)
- [Circuit Diagram](#circuit-diagram)
- [Blynk Dashboard Setup](#blynk-dashboard-setup)
- [Project Workflow](#project-workflow)
- [How It Works](#how-it-works)
- [Installation & Setup](#installation--setup)
- [Customization](#customization)
- [Future Enhancements](#future-enhancements)

---

## 📖 Project Overview

Plants need different amounts of water depending on the weather and soil condition. This smart watering system helps automate the watering process with the following goals:

- Reduce water wastage.
- Enable remote monitoring and control via a mobile app.
- Prevent watering during rainy days using weather data from OpenWeatherMap.

---

## ✨ Features

✅ Real-time soil moisture monitoring  
✅ Rain detection using weather API  
✅ Automation control via Blynk app  
✅ Manual override for watering  
✅ Supports indoor and outdoor plant modes  
✅ Logs weather and moisture data  
✅ Energy and water-efficient logic

---

## 🔧 Hardware Components

| Component            | Quantity |
|----------------------|----------|
| ESP8266 (NodeMCU)    | 1        |
| Soil Moisture Sensor | 1        |
| Relay Module (1-Channel) | 1    |
| Water Pump/Motor     | 1        |
| Power Supply (5V)    | 1        |
| Jumper Wires         | As needed |
| Breadboard (Optional)| 1        |
| Wi-Fi Connection     | Required |

---

## 💻 Software Requirements

- Arduino IDE with ESP8266 board support
- Blynk App (Android/iOS)
- [OpenWeatherMap API Key](https://openweathermap.org/api)
- Required Arduino Libraries:
  - `ESP8266WiFi.h`
  - `BlynkSimpleEsp8266.h`
  - `ESP8266HTTPClient.h`
  - `ArduinoJson.h`

---

## 📷 Circuit Diagram

- Soil Moisture Sensor → A0  
- Relay IN pin → D2  
- VCC/GND connections as per ESP8266 module  
- Relay controls the water motor (LOW to turn ON)

---

## 📱 Blynk Dashboard Setup

Use Blynk IoT App (Blynk 2.0):

| Virtual Pin | Widget           | Description                           |
|-------------|------------------|---------------------------------------|
| V0          | Terminal Widget  | System messages/logs                  |
| V1          | Gauge/Value Display | Soil Moisture Level                 |
| V2          | Switch           | Automation ON/OFF                     |
| V3          | Switch           | Indoor (ON) / Outdoor (OFF)           |
| V4          | Button           | Manual watering trigger               |

---

## ⚙️ Project Workflow

1. **Connect to Wi-Fi** using credentials.
2. **Read soil moisture** periodically.
3. **Check weather conditions** using OpenWeatherMap API.
4. If soil is dry **and** it's not rainy, **turn ON** water motor.
5. Motor turns **OFF** once the soil moisture reaches an acceptable level or after a timeout.
6. Send **real-time updates to Blynk app** for monitoring.

---

## 🧠 How It Works

- **Moisture Check:** Reads analog input from soil sensor (A0).
- **Weather Check:** Calls OpenWeatherMap API to detect rainy days.
- **Decision Logic:**
  - If soil is dry **and** weather is dry → Turn ON pump.
  - Else → Do nothing and log status.
- **Modes:**
  - *Indoor Mode:* Doesn’t rely on weather data.
  - *Outdoor Mode:* Uses weather data before watering.

---

## 🛠 Installation & Setup

1. Clone or download this repository.
2. Open the `.ino` file in Arduino IDE.
3. Install required libraries from Library Manager.
4. Replace placeholders with your credentials:
   ```cpp
   char auth[] = "Your_Blynk_Auth_Token";
   char ssid[] = "Your_WiFi_SSID";
   char pass[] = "Your_WiFi_Password";
   String apiKey = "Your_OpenWeatherMap_API_Key";
   ```
5. Upload the code to NodeMCU/ESP8266.
6. Open Serial Monitor (baud: 115200) to debug.
7. Monitor and control via the **Blynk IoT App**.

---

## 🧩 Customization

- **Moisture Threshold:** Change `moistureThreshold` value to calibrate based on your sensor and soil.
- **City:** Update the `city` variable for your local weather.
- **API Units:** Change `units` to `imperial` or `metric` depending on your preference.
- **Watering Timer:** Adjust `motorTimer` (you should declare and set it) to define how long the motor should run.

---

## 🚀 Future Enhancements

- Add **Telegram/Email alerts** for water usage or low water tank.
- Add **capacitive soil moisture sensor** for better accuracy.
- Integrate **solar-powered system** for energy independence.
- Use **local sensors (rain/temperature)** as an alternative to online weather APIs.

