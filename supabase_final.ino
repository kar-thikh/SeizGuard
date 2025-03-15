#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include "MAX30105.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <ArduinoJson.h>

// WiFi Credentials
#define WIFI_SSID "WIFISSID"
#define WIFI_PASS "WIFIPASSWORD"
#define SUPABASE_URL "supabas_table_url"
#define SUPABASE_KEY "supabase_private_key"

#define BUZZER_PIN 13 // Connect buzzer to GPIO 13

MAX30105 particleSensor;
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

void setup() {
    Serial.begin(115200);
    Serial.println("Initializing...");

    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);  // Ensure buzzer is off initially

    // Connect to WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi!");

    // Initialize MAX30102
    if (!particleSensor.begin(Wire, I2C_SPEED_STANDARD)) {
        Serial.println("MAX30102 not found. Check connections.");
        while (1);
    }
    particleSensor.setup();

    // Initialize ADXL345
    if (!accel.begin()) {
        Serial.println("ADXL345 not found. Check connections.");
        while (1);
    }
    accel.setRange(ADXL345_RANGE_2_G);
}

void loop() {
    // Read IR value and estimate BPM
    long irValue = particleSensor.getIR();
    int bpm = (irValue < 50000) ? 0 : map(irValue, 50000, 100000, 60, 150);

    // Apply your condition: If BPM > 60, subtract 20
    bpm = (bpm > 60) ? bpm - 20 : bpm;

    // SpO2 estimation (temporary placeholder)
    int spo2 = constrain(map(irValue, 50000, 100000, 95, 99), 90, 100);

    // Read motion data
    sensors_event_t event;
    accel.getEvent(&event);
    float motion = sqrt(sq(event.acceleration.x) + sq(event.acceleration.y) + sq(event.acceleration.z));

    // Check seizure condition
    bool seizure_detected = (motion > 25.0 && spo2 < 92 && bpm > 110);

    // **Trigger Buzzer if Seizure is Detected**
    if (seizure_detected) {
    Serial.println("🚨 SEIZURE DETECTED! Activating buzzer...");
    tone(BUZZER_PIN, 1000);  // Start buzzer at 1kHz
    delay(1000);             // Wait for 1 second
    noTone(BUZZER_PIN);      // Stop buzzer after 1 second
}


    // Create JSON payload using ArduinoJson
    StaticJsonDocument<200> doc;
    doc["heart_rate"] = bpm;
    doc["spo2"] = spo2;
    doc["motion"] = motion;
    doc["seizure_detected"] = seizure_detected;

    String payload;
    serializeJson(doc, payload);

    Serial.println("Sending data to Supabase...");
    Serial.println(payload);

    // Send data to Supabase
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(SUPABASE_URL);
        http.addHeader("Content-Type", "application/json");
        http.addHeader("apikey", SUPABASE_KEY);
        http.addHeader("Authorization", "Bearer " SUPABASE_KEY);
        http.addHeader("Prefer", "return=minimal");

        int httpResponseCode = http.POST(payload);
        Serial.print("HTTP Response Code: ");
        Serial.println(httpResponseCode);

        http.end();
    } else {
        Serial.println("WiFi not connected!");
    }

    delay(5000);
}
