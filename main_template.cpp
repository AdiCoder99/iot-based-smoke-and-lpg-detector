#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

// ===================================================================
// CONFIGURATION - Replace these with your actual credentials
// ===================================================================

// WiFi Network Credentials
// Replace with your WiFi network name and password
const char* WIFI_SSID     = "YOUR_WIFI_SSID_HERE";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD_HERE";

// Firebase Realtime Database Configuration
// Replace with your Firebase project details
// Get these from: https://console.firebase.google.com/
const char* FIREBASE_HOST    = "https://YOUR_PROJECT_ID.firebaseio.com";
const char* FIREBASE_API_KEY = "YOUR_DATABASE_SECRET_HERE";

// ===================================================================
// Hardware Pin Configuration
// ===================================================================
#define MQ2_D0_PIN   D1    // MQ-2 Digital output connected to D1
#define BUZZER_PIN   D2    // Buzzer connected to D2

// ===================================================================
// Timing Settings
// ===================================================================
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 5000;      // Send data every 5 seconds
unsigned long startTime;
const unsigned long warmupDuration = 10000;   // 10 seconds warm-up period

// ===================================================================
// SETUP - Initialize the system
// ===================================================================
void setup() {
  Serial.begin(115200);
  delay(1000);

  // Configure pins
  pinMode(MQ2_D0_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);   // Buzzer OFF initially

  // Print startup messages
  Serial.println("\nMQ2 Gas Detector with Firebase - ESP8266");
  Serial.println("Connecting to WiFi...");

  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // WiFi connection successful
  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Start sensor warm-up timer
  startTime = millis();
  Serial.println("Warming up MQ2 sensor for 10 seconds...");
}

// ===================================================================
// LOOP - Main program loop
// ===================================================================
void loop() {
  // Read MQ-2 sensor digital output
  int gasDigital = digitalRead(MQ2_D0_PIN);
  Serial.print("MQ2 Digital (raw): ");
  Serial.println(gasDigital);
  Serial.println(WiFi.localIP());

  // --------- Gas Detection Logic --------
  // MQ-2 Active LOW Detection:
  // LOW (0) = Smoke/LPG detected
  // HIGH (1) = Clean air
  bool gasDetected = (gasDigital == LOW);

  // --------- Buzzer Control ----------
  // Only activate buzzer when:
  // 1. Warm-up period is complete (after 10 seconds)
  // 2. Gas is detected
  if ((millis() - startTime) > warmupDuration && gasDetected) {
    digitalWrite(BUZZER_PIN, HIGH);  // BEEP when smoke/fire detected
  } else {
    digitalWrite(BUZZER_PIN, LOW);   // OFF in clean air or during warmup
  }

  // --------- Send Data to Firebase --------
  // Send readings every 5 seconds
  unsigned long currentMillis = millis();
  if (currentMillis - lastSendTime >= sendInterval) {
    lastSendTime = currentMillis;

    // Convert boolean to 0 or 1
    // 0 = no smoke, 1 = smoke detected
    int gasValueToSend = gasDetected ? 1 : 0;
    sendToFirebase(gasValueToSend);
  }

  delay(300);
}

// ===================================================================
// Function: Send Data to Firebase
// ===================================================================
void sendToFirebase(int gasValue) {
  // Check if WiFi is still connected
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected, skipping Firebase upload");
    return;
  }

  // Create secure client
  WiFiClientSecure client;
  client.setInsecure();  // For development (remove for production)

  HTTPClient http;

  // Build Firebase URL (PUT updates same location)
  String url = String(FIREBASE_HOST) + "/mq2_readings.json?auth=" + FIREBASE_API_KEY;

  Serial.print("Request URL: ");
  Serial.println(url);

  if (http.begin(client, url)) {
    http.addHeader("Content-Type", "application/json");

    // Create JSON payload
    String jsonPayload = "{";
    jsonPayload += "\"gasDigital\":" + String(gasValue) + ",";
    jsonPayload += "\"timestamp\":" + String(millis());
    jsonPayload += "}";

    Serial.print("Sending JSON (PUT): ");
    Serial.println(jsonPayload);

    // Send PUT request (overwrites same location)
    int httpCode = http.PUT(jsonPayload);

    if (httpCode > 0) {
      Serial.printf("Firebase Response Code: %d\n", httpCode);
      String payload = http.getString();
      Serial.println("Response payload:");
      Serial.println(payload);
    } else {
      Serial.printf("HTTP PUT failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  } else {
    Serial.println("Unable to connect to Firebase host");
  }
}

// ===================================================================
// END OF CODE
// ===================================================================
