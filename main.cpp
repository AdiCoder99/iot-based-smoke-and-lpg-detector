#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

// ---------------- WiFi Credentials ----------------
const char* WIFI_SSID     = "YOUR_WIFI_SSID_HERE";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD_HERE";

// --------------- Firebase Config ------------------
const char* FIREBASE_HOST    = "YOUR_PROJECT_ID.firebaseio.com";
const char* FIREBASE_API_KEY = "YOUR_DATABASE_SECRET_HERE";

// --------------- Hardware Pins --------------------
#define MQ2_D0_PIN   D1    // MQ-2 DIGITAL output connected to D1
#define BUZZER_PIN   D2    // Buzzer connected to D2

// -------------- Other Settings --------------------
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 5000;
unsigned long startTime;

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(MQ2_D0_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.println("\nMQ2 Gas Detector with Firebase - ESP8266");
  Serial.println("Connecting to WiFi...");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  startTime = millis();
  Serial.println("Warming up MQ2 sensor for 10 seconds...");
}

void loop() {
  int gasDigital = digitalRead(MQ2_D0_PIN);
  Serial.print("MQ2 Digital: ");
  Serial.println(gasDigital);

  bool gasDetected;

  // 🔴 IMPORTANT: set this based on what you saw in the simple test
  // If in clean air you saw:
  //   0 -> gasDetected when gasDigital == HIGH
  //   1 -> gasDetected when gasDigital == LOW

  // --- Case 1: clean air = 0 (most common) ---
  // gasDetected = (gasDigital == HIGH);

  // --- Case 2: clean air = 1 (your buzzer always beeping case) ---
  gasDetected = (gasDigital == LOW);   // <-- try this first

  // --------- Buzzer Control ----------
  // Don't beep in the first 10 seconds (warm-up time)
  if (millis() - startTime > 10000) {
    if (gasDetected) {
      digitalWrite(BUZZER_PIN, HIGH);  // Gas detected -> BEEP
    } else {
      digitalWrite(BUZZER_PIN, LOW);   // No gas -> silent
    }
  } else {
    digitalWrite(BUZZER_PIN, LOW);     // keep buzzer OFF during warmup
  }

  // --------- Send to Firebase every sendInterval ms ----------
  unsigned long currentMillis = millis();
  if (currentMillis - lastSendTime >= sendInterval) {
    lastSendTime = currentMillis;
    sendToFirebase(gasDetected ? 1 : 0);
  }

  delay(300);
}

// -------------- Function: Send Data to Firebase ---------------
void sendToFirebase(int gasValue) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected, skipping Firebase upload");
    return;
  }

  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient http;

  String url = String(FIREBASE_HOST) + "/mq2_readings.json?auth=" + FIREBASE_API_KEY;

  Serial.print("Request URL: ");
  Serial.println(url);

  if (http.begin(client, url)) {
    http.addHeader("Content-Type", "application/json");

    String jsonPayload = "{";
    jsonPayload += "\"gasDigital\":" + String(gasValue) + ",";
    jsonPayload += "\"timestamp\":" + String(millis());
    jsonPayload += "}";

    Serial.print("Sending JSON: ");
    Serial.println(jsonPayload);

    int httpCode = http.POST(jsonPayload);

    if (httpCode > 0) {
      Serial.printf("Firebase Response Code: %d\n", httpCode);
      String payload = http.getString();
      Serial.println("Response payload:");
      Serial.println(payload);
    } else {
      Serial.printf("HTTP POST failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  } else {
    Serial.println("Unable to connect to Firebase host");
  }
}