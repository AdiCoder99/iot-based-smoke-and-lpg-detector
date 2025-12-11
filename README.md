# Smoke and LPG Detector - IoT Project

## Overview

This is an IoT-based Smoke and LPG detector system that uses an **ESP8266** microcontroller to detect gas leaks in real-time. The system integrates a **MQ-2 gas sensor** for detection, a buzzer for alerts, and **Firebase Realtime Database** for cloud data logging and remote monitoring.

## Features

- **Real-time Gas Detection**: Uses MQ-2 sensor to detect smoke and LPG gas
- **Instant Alarm System**: Buzzer activation on gas detection
- **Cloud Integration**: Sends sensor data to Firebase Realtime Database
- **IoT Ready**: WiFi-enabled for remote monitoring
- **Warm-up Period**: 10-second sensor warm-up before alert activation
- **Periodic Data Logging**: Sends readings to Firebase every 5 seconds

## Hardware Requirements

- **Microcontroller**: ESP8266 (NodeMCU or similar)
- **Gas Sensor**: MQ-2 Smoke/LPG Sensor
- **Output Devices**: Buzzer/Speaker
- **Power Supply**: USB or DC power (3.3V/5V)
- **WiFi Network**: For connectivity to Firebase

### Pin Configuration

| Component | Pin | Description |
|-----------|-----|-------------|
| MQ-2 Digital Output | D1 | Gas detection digital signal |
| Buzzer | D2 | Audio alert output |

## Software Requirements

### IDE: Arduino IDE

This project is developed and programmed using the **Arduino IDE**. Arduino IDE is a free, open-source development environment that allows you to write, compile, and upload code to microcontroller boards.

**Why Arduino IDE?**
- Simple and beginner-friendly interface
- Extensive library support for ESP8266
- Built-in serial monitor for debugging
- Cross-platform compatibility (Windows, macOS, Linux)
- Large community support

### Required Libraries

Install the following libraries via Arduino IDE Library Manager (Sketch → Include Library → Manage Libraries):

1. **ESP8266WiFi** - Built-in WiFi support
2. **ESP8266HTTPClient** - HTTP communication with Firebase
3. **WiFiClientSecure** - Secure HTTPS connections

### Arduino IDE Setup for ESP8266

1. Open Arduino IDE
2. Go to **File → Preferences**
3. In "Additional Boards Manager URLs", add:
   ```
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
4. Go to **Tools → Board → Boards Manager**
5. Search for "esp8266" and install the latest version
6. Select your ESP8266 board: **Tools → Board → Generic ESP8266 Module** (or NodeMCU 1.0)

## Configuration

Before uploading, edit the following credentials in `main.cpp`:

```cpp
// WiFi Credentials
const char* WIFI_SSID     = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// Firebase Configuration
const char* FIREBASE_HOST    = "YOUR_PROJECT_ID.firebaseio.com";
const char* FIREBASE_API_KEY = "YOUR_DATABASE_SECRET";
```

## Firebase Setup Guide

### Step 1: Create a Firebase Project

1. Go to [Firebase Console](https://console.firebase.google.com/)
2. Click **Create a project** or select an existing one
3. Enter your project name and proceed through the setup wizard
4. Enable Google Analytics (optional) and complete project creation

### Step 2: Create Realtime Database

1. In the Firebase Console, navigate to **Realtime Database** (under the Build section on the left sidebar)
2. Click **Create Database**
3. Choose your database location (select the region closest to you)
4. Select **Start in Test Mode** (for development and testing)
5. Click **Enable**
6. Your database URL will appear in the format: `https://YOUR_PROJECT_ID.firebaseio.com`

### Step 3: Get Your Database Secret

1. Click on **Project Settings** (gear icon in the top-left)
2. Go to the **Service Accounts** tab
3. Click on **Database Secrets** tab
4. Click the **Show** button to reveal your secret key
5. Copy this secret key - this is your `FIREBASE_API_KEY`
6. **⚠️ SECURITY WARNING**: Never share this key or commit it to public repositories. Treat it like a password.

### Step 4: Configure Database Security Rules

1. Go back to **Realtime Database**
2. Click on the **Rules** tab at the top
3. Replace the default rules with:

```json
{
  "rules": {
    "mq2_readings": {
      ".write": "auth != null",
      ".read": "auth != null"
    }
  }
}
```

4. Click **Publish** to apply the new rules

**Note for Testing**: If you want to test without authentication, temporarily use:
```json
{
  "rules": {
    ".read": true,
    ".write": true
  }
}
```
Always switch back to authenticated rules before deployment to production!

### Step 5: Update Arduino Code with Your Credentials

1. Open `main.cpp` in Arduino IDE
2. Locate the configuration section at the top of the file
3. Replace with your actual credentials:
   - `YOUR_WIFI_SSID`: Your home/office WiFi network name
   - `YOUR_WIFI_PASSWORD`: Your WiFi password
   - `YOUR_PROJECT_ID`: The ID from your Firebase database URL (before `.firebaseio.com`)
   - `YOUR_DATABASE_SECRET`: The secret key from Step 3

### Step 6: Verify Firebase Connection

1. Upload the sketch to your ESP8266
2. Open **Tools → Serial Monitor** (set baud rate to 115200)
3. Watch the output for "WiFi connected!" message
4. In Firebase Console, go to **Realtime Database** and monitor the data
5. You should see new `mq2_readings` entries appearing when data is sent

### Firebase Data Structure

Your data will be stored in Firebase with this structure:

```
your-project-id
  └── mq2_readings
       └── -MX4a3b2c1d
            ├── gasDigital: 0
            └── timestamp: 45231
```

Each time your device sends data, a new entry is created with a unique key.

### Firebase Troubleshooting

| Issue | Solution |
|-------|----------|
| Can't access Firebase Console | Check you're logged in with correct Google account |
| "Unable to connect to Firebase host" | Verify FIREBASE_HOST URL is correct and contains only the domain |
| "403 Forbidden" response | Check database rules - may be too restrictive |
| "401 Unauthorized" response | Database secret is incorrect or expired |
| Data not appearing in Firebase | Check WiFi connection in Serial Monitor; verify internet connectivity |
| Database secret not visible | Ensure you clicked "Show" button next to Database Secrets |

## How It Works

1. **Initialization**: 
   - Connects to WiFi network
   - Initializes MQ-2 sensor and buzzer pins
   - Starts 10-second warm-up period

2. **Gas Detection**:
   - Reads MQ-2 digital output continuously
   - After warm-up period, triggers buzzer if gas is detected
   - Adjustable logic for sensor calibration (HIGH or LOW trigger)

3. **Data Logging**:
   - Every 5 seconds, sends gas detection data to Firebase
   - Includes gas value (0 or 1) and timestamp

4. **Alert System**:
   - Buzzer activates upon gas detection
   - Silent during warm-up period to avoid false alarms

## Sensor Calibration

The MQ-2 sensor output logic can be adjusted based on your clean air reading:

```cpp
// Option 1: Clean air = 0 (Most common)
// gasDetected = (gasDigital == HIGH);

// Option 2: Clean air = 1 (If buzzer beeps constantly)
// gasDetected = (gasDigital == LOW);
```

Run the simple test to determine which logic applies to your sensor.

## Upload Instructions

1. Connect ESP8266 to your computer via USB
2. Select **Tools → Port** and choose the correct COM port
3. Select **Tools → Board** and choose your ESP8266 board
4. Click **Sketch → Upload** or press `Ctrl+U`
5. Open **Tools → Serial Monitor** (set baud rate to 115200) to view debug messages

## Data Format and Storage

The system sends sensor data to Firebase Realtime Database with the following JSON structure:

```json
{
  "gasDigital": 0,
  "timestamp": 45231
}
```

**Field Descriptions**:
- `gasDigital`: 0 when no gas is detected, 1 when gas is detected
- `timestamp`: Time in milliseconds since the ESP8266 started (for reference)

## Troubleshooting

| Issue | Solution |
|-------|----------|
| WiFi not connecting | Verify SSID and password are correct |
| Buzzer beeping constantly | Adjust gas detection logic (HIGH/LOW) |
| No Firebase data | Check API key and Firebase host URL |
| Serial output not showing | Ensure baud rate is set to 115200 |
| Sensor not responding | Check pin connections and sensor power supply |

## Future Enhancements

- Add analog sensor readings for more precise gas concentration levels
- Implement MQTT protocol for better IoT integration
- Add web dashboard for real-time monitoring
- Email/SMS notifications for gas alerts
- Battery backup system
- Mobile app integration

## License

This project is open-source and available for personal and educational use.

## Author

IoT Project - Smoke and LPG Detector

---

**Last Updated**: December 2025
