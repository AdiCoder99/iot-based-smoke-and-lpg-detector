#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

// ---------------- WiFi Credentials ----------------
const char* WIFI_SSID     = "OPPO K13 5G x64i";
const char* WIFI_PASSWORD = "kgsq3337";

// --------------- Firebase Config ------------------
const char* FIREBASE_HOST    = "https://mq2-sensor-project-default-rtdb.firebaseio.com";
const char* FIREBASE_API_KEY = "KDEEjIYOhAYjbI2kuFBimGbOMf7WMK9n246NvvNS";

// --------------- Hardware Pins --------------------
#define MQ2_D0_PIN   D1    // MQ-2 DIGITAL output connected to D1
#define BUZZER_PIN   D2    // Buzzer connected to D2

// -------------- Other Settings --------------------
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 5000;   // send every 5 seconds
unsigned long startTime;
const unsigned long warmupDuration = 10000; // 10 seconds warm-up

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(MQ2_D0_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);   // Buzzer OFF initially

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
  Serial.print("MQ2 Digital (raw): ");
  Serial.println(gasDigital);
  Serial.println(WiFi.localIP());

  // ---------------- Gas Detection Logic ----------------
  // Your case: default = 1 (clean air)
  // Make it:
  //   0 in Firebase for clean air
  //   1 in Firebase only when smoke/fire is detected
  bool gasDetected = (gasDigital == LOW);  // active LOW: 0 = SMOKE, 1 = CLEAN

  // --------- Buzzer Control ----------
  // Only beep when smoke is detected AND after warmup
  if ((millis() - startTime) > warmupDuration && gasDetected) {
    digitalWrite(BUZZER_PIN, HIGH);  // BEEP only when smoke/fire detected
  } else {
    digitalWrite(BUZZER_PIN, LOW);   // OFF in clean air or during warmup
  }

  // --------- Send to Firebase every sendInterval ms ----------
  unsigned long currentMillis = millis();
  if (currentMillis - lastSendTime >= sendInterval) {
    lastSendTime = currentMillis;

    // Map to 0 / 1 for database:
    // 0 = no smoke, 1 = smoke detected
    int gasValueToSend = gasDetected ? 1 : 0;
    sendToFirebase(gasValueToSend);
  }

  delay(300);
}

// -------------- Function: Send Data to Firebase (overwrite same value) ---------------
void sendToFirebase(int gasValue) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected, skipping Firebase upload");
    return;
  }

  WiFiClientSecure client;
  client.setInsecure();  // ignore SSL cert (OK for projects)

  HTTPClient http;

  // Always writing to the SAME node: /mq2_readings
  String url = String(FIREBASE_HOST) + "/mq2_readings.json?auth=" + FIREBASE_API_KEY;

  Serial.print("Request URL: ");
  Serial.println(url);

  if (http.begin(client, url)) {
    http.addHeader("Content-Type", "application/json");

    // This will overwrite the existing data at /mq2_readings
    String jsonPayload = "{";
    jsonPayload += "\"gasDigital\":" + String(gasValue) + ",";   // 0 or 1
    jsonPayload += "\"timestamp\":" + String(millis());
    jsonPayload += "}";

    Serial.print("Sending JSON (PUT): ");
    Serial.println(jsonPayload);

    // PUT -> updates the same location (no new keys)
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