# IoT-Based Smoke and LPG Detector

## Quick Start

Clone this repository and follow the **Configuration & Setup** section below to get started in minutes!

## Overview

This is a complete **IoT Gas Detection System** with hardware and mobile app components:

- **ESP8266 Firmware** (`main.cpp`) - Detects gas locally and reports to cloud
- **Android App** (`android-app/`) - Real-time monitoring and alerts from anywhere
- **Firebase Backend** - Secure cloud database for all data

The system detects gas leaks using an **MQ-2 gas sensor**, triggers a buzzer alarm when smoke/LPG is detected, and logs all data to **Firebase Realtime Database** for remote monitoring via the companion Android app.

The device uses **active LOW** detection logic, meaning the sensor outputs LOW (0) when smoke/fire is detected and HIGH (1) in clean air conditions.

## Features

✅ **Real-time Gas Detection** - MQ-2 sensor continuously monitors air quality  
✅ **Instant Alert System** - Buzzer activates immediately upon gas detection  
✅ **Cloud Data Logging** - All readings sent to Firebase Realtime Database every 5 seconds  
✅ **Smart Warm-up Period** - 10-second initialization to stabilize sensor readings  
✅ **Firebase Integration** - Remote monitoring with real-time data updates  
✅ **Serial Debugging** - Real-time console output for troubleshooting  
✅ **Active LOW Detection** - Optimized logic for MQ-2 sensor response  
✅ **PUT Method Updates** - Uses Firebase PUT to overwrite latest value (no database bloat)

## 📁 Project Structure

```
iot-based-smoke-and-lpg-detector/
│
├── main.cpp                  # Main firmware code (add your credentials here)
├── main_template.cpp         # Template with placeholder credentials (safe to share)
├── README.md                 # This file - full project documentation
├── CREDENTIALS_SETUP.md      # Security guide and credential setup instructions
├── .gitignore               # Git ignore rules (protects your credentials)
│
└── android-app/             # Optional Android companion app (separate project)
    ├── app/
    ├── gradle/
    └── build.gradle.kts
```

**Key Files:**
- 🔧 **main.cpp** - Fill in your WiFi & Firebase credentials here
- 📋 **main_template.cpp** - Use this as reference for code structure
- 🔐 **CREDENTIALS_SETUP.md** - Complete security best practices guide
- 🛡️ **.gitignore** - Automatically protects sensitive files from git

## 🏗️ System Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                     COMPLETE IoT SYSTEM                      │
├─────────────────────────────────────────────────────────────┤
│                                                               │
│  ┌────────────────┐         ┌──────────────────────────┐    │
│  │  MQ-2 Sensor   │         │   Firebase Realtime DB   │    │
│  │  + Buzzer      │         │   (Cloud Backend)        │    │
│  │                │         │                          │    │
│  │  ┌─ LOW = Gas   │────────▶ mq2_readings            │    │
│  │  │ HIGH = Clean │         {"gasDigital": 0/1}      │    │
│  │  └──────────────┤         {"timestamp": xxxx}      │    │
│  │                │         │                          │    │
│  │  ESP8266       │         │  Rules: Public R/W       │    │
│  │  NodeMCU       │         │                          │    │
│  │                │         └────────────┬─────────────┘    │
│  │  WiFi Connect  │                      │                  │
│  │  PUT to DB     │                      │                  │
│  │  Every 5 sec   │                      │                  │
│  │                │                      │                  │
│  └────────────────┘                      │                  │
│        ▲                                  │                  │
│        │                                  │                  │
│        │ USB Programming                  │                  │
│        │ (Arduino IDE)                    ▼                  │
│        │                          ┌──────────────────────┐  │
│  ┌─────┴──────────────────┐       │   Android App        │  │
│  │  Computer              │       │   (Companion)        │  │
│  │  - Arduino IDE         │       │                      │  │
│  │  - Code Upload         │       │  - Real-time View    │  │
│  │  - Serial Monitor      │       │  - Notifications     │  │
│  │                        │       │  - History Tracking  │  │
│  └────────────────────────┘       │  - Dark Mode         │  │
│                                   │                      │  │
│                                   └──────────────────────┘  │
│                                                               │
└─────────────────────────────────────────────────────────────┘
```

### System Components:

1. **Hardware (ESP8266 + Sensors)**
   - Detects gas locally
   - Sounds buzzer immediately
   - Reports to Firebase every 5 seconds

2. **Cloud Backend (Firebase)**
   - Stores real-time sensor data
   - Provides REST API
   - Handles push notifications

3. **Mobile App (Android)**
   - Displays live status
   - Receives instant notifications
   - Views historical data
   - Works remotely from anywhere

## ⚙️ Complete System Setup Overview

### You will need:

1. **Hardware Setup** (~30 minutes)
   - Assemble ESP8266 + MQ-2 sensor + buzzer
   - Connect to WiFi
   - Upload firmware from `main.cpp`

2. **Firebase Setup** (~10 minutes)
   - Create Firebase project
   - Set up Realtime Database
   - Get credentials for both ESP8266 and Android app

3. **Android App Setup** (~10 minutes)
   - Install Android Studio
   - Open `android-app/` project folder
   - Get `google-services.json` from Firebase
   - Build and install on Android phone

4. **Integration** (~5 minutes)
   - Use same Firebase project for ESP8266 and Android app
   - Android app automatically reads data that ESP8266 sends
   - Done! Complete system is now live

**Total Setup Time**: ~1 hour for complete working system

## 📱 Android Companion App

The `android-app/` folder contains a complete Android application that connects to the same Firebase database to display real-time sensor data and receive notifications.

### Features

- 📊 **Real-time Dashboard** - View live sensor readings from the Firebase database
- 🔔 **Push Notifications** - Get alerts when gas is detected
- 📈 **Historical Data** - View data trends and history
- 🎨 **Modern UI** - Built with Jetpack Compose (Kotlin)
- 🔐 **Secure Authentication** - Firebase authentication integration
- 🌙 **Dark Mode Support** - Comfortable viewing in any lighting

### Android App Structure

```
android-app/
├── app/
│   ├── src/
│   │   ├── main/
│   │   │   ├── java/kotlin/        # App source code
│   │   │   ├── res/                # UI resources, layouts, strings
│   │   │   ├── AndroidManifest.xml # App configuration
│   │   │   └── google-services.json# Firebase config (in .gitignore)
│   │   ├── test/                   # Unit tests
│   │   └── androidTest/            # Instrumented tests
│   ├── build.gradle.kts            # App dependencies
│   └── proguard-rules.pro          # Code obfuscation rules
├── build.gradle.kts                # Project-level build config
├── settings.gradle.kts             # Project settings
└── gradle/                         # Gradle wrapper
```

### Requirements

- **Android Version**: 7.0 (API 24) or higher
- **Java/Kotlin**: Kotlin with AndroidX
- **Libraries Used**:
  - Firebase Realtime Database SDK
  - Firebase Authentication
  - Firebase Cloud Messaging (FCM)
  - Jetpack Compose UI Framework
  - Material Design 3

### Setup Instructions

1. **Open in Android Studio**:
   - Open Android Studio
   - Click **File → Open**
   - Navigate to `android-app/` folder
   - Click **OK** to open the project

2. **Configure Firebase**:
   - Get `google-services.json` from your Firebase project
   - Place it in `android-app/app/` folder
   - The file is in `.gitignore` - never commit it!

3. **Update Database URL**:
   - Open `app/build.gradle.kts`
   - Update Firebase project ID to match your project
   - Sync Gradle files

4. **Build and Run**:
   - Connect Android device or use emulator
   - Click **Run → Run 'app'**
   - Grant permissions when prompted

### Connecting to Your Device

The Android app automatically reads from the same Firebase database that your ESP8266 writes to:

1. Both use the same Firebase project
2. The app displays the latest `mq2_readings` data
3. Notifications trigger when `gasDigital` changes from 1 to 0

### App Features in Detail

#### Dashboard Tab
- Real-time status: "Gas Detected" or "Clean Air"
- Current reading timestamp
- Last update time
- Connection status

#### Notifications
- Receive alerts when smoke is detected
- Customizable notification settings
- Sound and vibration options

#### Settings
- Firebase project configuration
- Notification preferences
- Data refresh interval
- Theme selection (Light/Dark mode)

### Troubleshooting Android App

| Issue | Solution |
|-------|----------|
| App won't build | Check SDK version matches requirements; Update Gradle; Sync project |
| No Firebase data | Verify `google-services.json` is in correct location; Check Firebase rules allow read access |
| Notifications not working | Enable Firebase Cloud Messaging (FCM); Check app permissions; Allow notifications in system settings |
| App crashes on startup | Check Android version is 7.0+; Verify internet connection; Clear app cache |
| Can't connect to Firebase | Check WiFi/internet; Verify project ID in code; Check Firebase console is accessible |

### Development

To modify the Android app:

1. **MainActivity.kt** - Main activity and navigation
2. **FirebaseManager.kt** - Firebase database operations
3. **ui/** - Jetpack Compose screens
4. **models/** - Data models for sensor readings
5. **utils/** - Helper functions and utilities

### Building Release APK

```bash
cd android-app
./gradlew assembleRelease
```

APK will be generated at: `android-app/app/build/outputs/apk/release/`

### Security Notes

- Never commit `google-services.json` to the repository
- `.gitignore` already protects this file
- Store Firebase credentials securely
- Use Firebase Authentication for production apps
- Implement proper authorization rules in Firebase

## Hardware Components

| Component | Quantity | Purpose |
|-----------|----------|---------|
| ESP8266 (NodeMCU) | 1 | Main microcontroller with WiFi |
| MQ-2 Gas Sensor | 1 | Detects smoke and LPG gas |
| Buzzer/Speaker | 1 | Audio alert on gas detection |
| USB Cable (Micro-B) | 1 | Power and programming |
| Jumper Wires | As needed | Connections between components |
| Breadboard | 1 | For prototyping (optional) |

### Pin Configuration

| Component | ESP8266 Pin | Description |
|-----------|-------------|-------------|
| MQ-2 Digital Output | D1 (GPIO5) | Reads 0 (smoke) or 1 (clean air) |
| Buzzer | D2 (GPIO4) | Outputs HIGH (buzzer on) or LOW (off) |
| Power | 3.3V/5V | Sensor and buzzer power supply |
| GND | GND | Ground reference |

### Wiring Diagram

```
ESP8266                MQ-2 Sensor           Buzzer
  D1 ─────────────────> DO (Digital)         
  3.3V ────────────────> VCC                 3.3V ───────────> (+)
  GND ─────────────────> GND                 GND ───> D2 ────> (-)
```

## Software Requirements

### IDE: Arduino IDE

This project is developed using the **Arduino IDE** - a free, open-source platform for programming microcontroller boards.

**Why Arduino IDE?**
- Simple, user-friendly interface
- Built-in support for ESP8266 through Board Manager
- Excellent debugging with Serial Monitor
- Cross-platform (Windows, macOS, Linux)
- Extensive community and documentation
- Easy library management

### Required Libraries

The following libraries are used (all built-in to Arduino IDE):

1. **ESP8266WiFi** - WiFi connectivity
2. **ESP8266HTTPClient** - HTTP/HTTPS communication with Firebase
3. **WiFiClientSecure** - Secure SSL/TLS connections

### Setting Up Arduino IDE for ESP8266

1. **Open Arduino IDE** and go to **File → Preferences**

2. In **Additional Boards Manager URLs**, paste:
   ```
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```

3. Click **OK**, then go to **Tools → Board → Boards Manager**

4. Search for **"esp8266"** and click **Install**

5. Go to **Tools → Board** and select:
   - **NodeMCU 1.0 (ESP-12E Module)** - for NodeMCU boards
   - Or **Generic ESP8266 Module** - for other ESP8266 boards

6. Set **Tools → Upload Speed** to **115200**

## Configuration & Setup

### 📋 Important: Using the Template File

This repository includes two versions of the code:

- **`main_template.cpp`** - Template file with placeholder credentials (SAFE TO SHARE)
- **`main.cpp`** - Your working file (ADD YOUR CREDENTIALS HERE, protected by .gitignore)

**To get started:**
1. Copy `main_template.cpp` to create your working version, or
2. Fill in credentials directly in `main.cpp`
3. The `.gitignore` file protects your credentials from being accidentally committed

### Step 1: Gather WiFi Credentials

You'll need your WiFi network name (SSID) and password.

### Step 2: Create Firebase Project

1. Go to [Firebase Console](https://console.firebase.google.com/)
2. Click **Create a project**
3. Enter project name (e.g., "mq2-sensor-project")
4. Proceed through the wizard
5. When complete, note your **Project ID**

### Step 3: Set Up Realtime Database

1. In Firebase Console, click **Realtime Database** (Build section, left sidebar)
2. Click **Create Database**
3. Choose location closest to you
4. Select **Start in Test Mode** (for testing)
5. Click **Enable**
6. Your database URL appears: `https://YOUR_PROJECT_ID.firebaseio.com`

### Step 4: Get Database Secret

1. Click **Project Settings** (⚙️ icon, top-left)
2. Go to **Service Accounts** tab
3. Click **Database Secrets**
4. Click **Show** to reveal the secret key
5. **Copy this secret** - you'll need it in the code
6. ⚠️ **SECURITY WARNING**: Never share this or commit to public repositories!

### Step 5: Configure Database Rules

1. Go to **Realtime Database**
2. Click **Rules** tab
3. Replace the default rules:

```json
{
  "rules": {
    "mq2_readings": {
      ".read": true,
      ".write": true
    }
  }
}
```

4. Click **Publish**

**Note**: This allows public read/write for testing. For production, implement proper authentication!

### Step 6: Update Arduino Code

Open `main.cpp` and update these lines:

```cpp
const char* WIFI_SSID     = "YOUR_WIFI_NETWORK_NAME";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

const char* FIREBASE_HOST    = "https://YOUR_PROJECT_ID.firebaseio.com";
const char* FIREBASE_API_KEY = "YOUR_DATABASE_SECRET_KEY";
```

### Step 7: Upload to ESP8266

1. Connect ESP8266 to your computer via USB
2. In Arduino IDE: **Tools → Port** → Select your COM port
3. Click **Upload** (→) or press **Ctrl+U**
4. Wait for "Done uploading"
5. Open **Tools → Serial Monitor** (baud rate: **115200**)
6. Watch for initialization messages

## How It Works

### System Flow

```
Startup
  ↓
Initialize Serial & Pins
  ↓
Connect to WiFi
  ↓
Wait 10 seconds (sensor warm-up)
  ↓
[Loop starts every 300ms]
├── Read MQ-2 sensor
├── Check if gas detected (active LOW)
├── Control buzzer based on detection
├── Every 5 seconds: Send data to Firebase
└── Repeat
```

### Sensor Detection Logic

The MQ-2 sensor outputs:
- **LOW (0)** = Smoke/LPG detected ✓ Buzzer ON (after warm-up)
- **HIGH (1)** = Clean air ✓ Buzzer OFF

Code implementation:
```cpp
bool gasDetected = (gasDigital == LOW);  // active LOW detection
```

### Data Transmission

**What gets sent to Firebase:**
```json
{
  "gasDigital": 0,
  "timestamp": 45231
}
```

**Field Meanings**:
- `gasDigital`: 0 = smoke detected, 1 = clean air
- `timestamp`: Milliseconds since ESP8266 startup

**Update Method**: Uses **PUT** instead of POST, which overwrites the same location in Firebase. This means you always have the latest reading without accumulating old data.

### Timing Details

| Timing | Duration | Purpose |
|--------|----------|---------|
| Warm-up Period | 10 seconds | Stabilize sensor before alerts |
| Data Send Interval | 5 seconds | Upload readings to Firebase |
| Sensor Read Interval | 300ms | Read sensor value in loop |
| Buzzer Response | Immediate | Alert on gas detection |

## Serial Monitor Output

When running correctly, Serial Monitor (115200 baud) shows:

```
MQ2 Gas Detector with Firebase - ESP8266
Connecting to WiFi...
.......
WiFi connected!
IP address: 192.168.1.100
Warming up MQ2 sensor for 10 seconds...
MQ2 Digital (raw): 1
192.168.1.100
Request URL: https://mq2-sensor-project-default-rtdb.firebaseio.com/mq2_readings.json?auth=KDEEjIYO...
Sending JSON (PUT): {"gasDigital":0,"timestamp":15432}
Firebase Response Code: 200
Response payload:
{"gasDigital":0,"timestamp":15432}
```

## Monitoring Data

### In Firebase Console

1. Open **Realtime Database**
2. Expand the tree to find: `mq2_readings`
3. View the latest sensor reading in real-time
4. Data updates every 5 seconds when connected

### Interpreting Data

- `gasDigital: 0` → **Smoke/LPG detected!** 🚨 (Buzzer should be ON)
- `gasDigital: 1` → **Clean air** ✅ (Buzzer OFF)
- `timestamp` → Milliseconds since device startup

## Troubleshooting

### WiFi Connection Issues

| Problem | Solution |
|---------|----------|
| Won't connect to WiFi | Check SSID/password spelling; ensure WiFi is 2.4GHz (ESP8266 doesn't support 5GHz) |
| Keeps disconnecting | Move router closer; check for WiFi interference; update ESP8266 firmware |
| Serial shows dots "..." | Wrong WiFi credentials in code |

### Firebase Issues

| Problem | Solution |
|---------|----------|
| "Unable to connect to Firebase host" | Check FIREBASE_HOST URL format and internet connection |
| Response Code 401 | Database secret is wrong; regenerate it in Firebase |
| Response Code 403 | Database rules reject the request; check rules allow public write |
| No data in Firebase | Verify WiFi connected (check Serial); check database URL |

### Sensor Issues

| Problem | Solution |
|---------|----------|
| Buzzer beeps constantly | Sensor may be inverted; try `(gasDigital == HIGH)` instead |
| No sensor reading | Check D1 pin connection; verify sensor has power; check wiring |
| Inconsistent readings | Wait for 10-second warm-up; avoid WiFi interference; check sensor calibration |

### Hardware Issues

| Problem | Solution |
|---------|----------|
| ESP8266 won't upload | Select correct COM port; try different USB cable; check USB drivers |
| Buzzer doesn't work | Check D2 connection and polarity (+/-); test with simple blink code |
| Serial shows garbage | Change baud rate to 115200; check USB cable |

## Code Structure

```cpp
#include <libraries>
│
├── WiFi Credentials
│   ├── SSID
│   └── Password
│
├── Firebase Configuration
│   ├── FIREBASE_HOST
│   └── FIREBASE_API_KEY
│
├── Pin Definitions
│   ├── MQ2_D0_PIN (D1)
│   └── BUZZER_PIN (D2)
│
├── setup() function
│   ├── Serial initialization
│   ├── Pin configuration
│   ├── WiFi connection
│   └── Sensor warm-up
│
├── loop() function
│   ├── Read MQ-2 sensor
│   ├── Detect gas (active LOW)
│   ├── Control buzzer
│   └── Send to Firebase every 5 sec
│
└── sendToFirebase() function
    ├── Check WiFi status
    ├── Create secure connection
    ├── Create JSON payload
    └── PUT data to Firebase
```

## Security Best Practices

⚠️ **CRITICAL**: Before sharing code or uploading to GitHub:

1. **Remove all credentials** from `main.cpp`:
   ```cpp
   const char* WIFI_SSID     = "YOUR_WIFI_SSID_HERE";
   const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD_HERE";
   const char* FIREBASE_API_KEY = "YOUR_DATABASE_SECRET_HERE";
   ```

2. **Use the template file** for sharing:
   - Share only `main_template.cpp` 
   - Keep `main.cpp` locally with your credentials
   - `.gitignore` prevents accidental commits of `main.cpp`

3. **Detailed Security Guide**:
   - See **`CREDENTIALS_SETUP.md`** for complete security instructions
   - Includes what to do if credentials are exposed
   - Development vs Production best practices

4. **For production**:
   - Use environment variables or secure config files
   - Implement proper Firebase authentication
   - Regenerate credentials if accidentally exposed
   - Use strong WiFi passwords
   - Enable restrictive Firebase security rules

## Future Enhancements

- 📊 Add analog readings for gas concentration (0-1024 scale)
- 📱 Create mobile app dashboard for real-time alerts
- 📧 Email/SMS notifications on gas detection
- 🔋 Add battery backup with low-power sleep mode
- 🌐 Implement MQTT protocol for better IoT scalability
- 📈 Add data visualization and historical graphs
- 🔐 Implement proper user authentication
- 🎚️ Add adjustable sensitivity settings
- ⚙️ Add OTA (Over-The-Air) firmware updates
- 📍 Add location tracking for multiple sensors

## Project Statistics

- **Lines of Code**: ~130
- **Libraries**: ESP8266WiFi, ESP8266HTTPClient, WiFiClientSecure (all built-in)
- **Database**: Firebase Realtime Database (PUT method)
- **Update Frequency**: Every 5 seconds
- **Sensor Warm-up**: 10 seconds
- **Buzzer Response Time**: ~300ms

## References

- [ESP8266 Documentation](https://arduino-esp8266.readthedocs.io/)
- [Firebase Console](https://console.firebase.google.com/)
- [Arduino IDE Documentation](https://www.arduino.cc/en/Guide)
- [MQ-2 Sensor Datasheet](https://www.sparkfun.com/datasheets/Sensors/Biometric/MQ2.pdf)
- [Firebase Realtime Database](https://firebase.google.com/docs/database)

## License

This project is open-source and available for educational and personal use.

## Author

**IoT Project - Smoke and LPG Detector**  
Created with Arduino IDE | Powered by ESP8266 & Firebase

---

**Last Updated**: December 12, 2025  
**Version**: 2.1 (Added Security Features, Template Files, Credentials Guide)

## Support & Troubleshooting

If you encounter issues:

1. ✅ Check the **Troubleshooting** section above
2. ✅ Verify all connections match the wiring diagram
3. ✅ Check Serial Monitor (115200 baud) for error messages
4. ✅ Ensure credentials are correct and updated
5. ✅ Test WiFi connectivity separately
6. ✅ Verify Firebase database rules allow public write access
7. ✅ Try uploading a simple LED blink sketch to confirm ESP8266 works

## Tips for Success

- 🔌 Use a quality USB cable for reliable uploads
- 📡 Position ESP8266 close to router during setup
- ⏱️ Wait full 10 seconds for sensor warm-up
- 🔍 Use Serial Monitor to debug issues
- 💾 Keep backups of working code
- 🔒 Never commit credentials to version control
- 🧪 Test with "Start in Test Mode" before implementing auth
- 📱 Monitor Firebase Console for real-time data

Good luck with your IoT Gas Detector project! 🚀

---

**Questions or Issues?** Check the Troubleshooting section or review the code comments for detailed explanations.
