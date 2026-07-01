# Smart Agriculture System using IoT and Arduino Cloud

## Overview

The **Smart Irrigation System** is an Internet of Things (IoT) project
that automates irrigation by monitoring environmental conditions in real
time. The system collects data from multiple sensors, processes it using
an Arduino WiFi board, uploads live readings to **Arduino Cloud**, and
automatically controls a **water pump** through a relay based on sensor
data.

The project helps reduce water wastage, improves irrigation efficiency,
and enables remote monitoring from anywhere using the Arduino Cloud
dashboard.

------------------------------------------------------------------------

## Features

- Automatic irrigation based on soil moisture
- Temperature and humidity monitoring using a DHT sensor
- Rain detection to avoid unnecessary watering
- Light intensity monitoring using an LDR
- Real-time cloud monitoring through Arduino Cloud
- Automatic water pump control
- Remote monitoring from any internet-connected device
- Expandable for alerts, analytics, and AI-based irrigation

------------------------------------------------------------------------

## Components Used

| Component                      | Purpose                                 |
|--------------------------------|-----------------------------------------|
| Arduino WiFi Board             | Main controller and cloud connectivity  |
| Soil Moisture Sensor           | Measures soil moisture level            |
| DHT11/DHT22 Sensor             | Measures temperature and humidity       |
| Rain Sensor                    | Detects rainfall                        |
| LDR (Light Dependent Resistor) | Measures ambient light intensity        |
| Water Pump                     | Supplies water to the plants            |
| Relay Module                   | Switches the water pump ON/OFF          |
| Servo Motor                    | Used to provide shead on sensative crop |
| Jumper Wires & Breadboard      | Circuit connections                     |

------------------------------------------------------------------------

## How the Project Works

1.  The Arduino reads data from the Soil Moisture Sensor, DHT sensor,
    Rain Sensor, and LDR.
2.  The board connects to Wi-Fi and synchronizes with Arduino Cloud.
3.  Live sensor readings are uploaded to Arduino Cloud.
4.  Users can monitor temperature, humidity, soil moisture, rainfall
    status, and light intensity remotely through the Arduino Cloud
    dashboard.
5.  If the soil moisture falls below a predefined threshold and no rain
    is detected, the Arduino turns the **water pump ON** (and/or opens
    the irrigation valve using the servo motor).
6.  Once the desired moisture level is reached or rain is detected, the
    Arduino turns the **water pump OFF**.
7.  The process repeats continuously to maintain efficient irrigation.

------------------------------------------------------------------------

## Arduino Cloud Integration

Arduino Cloud stores and displays live values for: - Soil Moisture -
Temperature - Humidity - Rain Detection - Light Intensity - Water Pump
Status - Servo Status (optional)

These variables are displayed on an Arduino Cloud dashboard using
gauges, charts, and status indicators, allowing users to monitor the
irrigation system remotely in real time.

------------------------------------------------------------------------

## Advantages

- Saves water through intelligent irrigation
- Enables real-time remote monitoring
- Reduces manual effort
- Monitors environmental conditions continuously
- Supports sustainable and smart farming
- Easy to expand with additional sensors and automation

------------------------------------------------------------------------

## Future Improvements

- Mobile notifications
- Weather API integration
- AI-based irrigation prediction
- Water consumption analytics
- Multiple irrigation zones
- Historical cloud data visualization

------------------------------------------------------------------------

## Authors

Developed as an IoT engineering project demonstrating smart irrigation
using Arduino Cloud, environmental sensing, and automated irrigation
control.
