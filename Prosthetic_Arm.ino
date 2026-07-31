#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <Servo.h>

// ---- Wi-Fi Credentials ----
#define WIFI_SSID "Krunal"
#define WIFI_PASSWORD "12345678"

// ---- Firebase Credentials ----
#define FIREBASE_HOST "prosthetic-arm-fa005-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "Ot3lS2lQN34WSWN5d0GW64WEw01CMAV0X79lzv0v"

// ---- Firebase Setup Objects ----
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// ---- EMG & Servo Setup ----
#define EMG_PIN A0
#define LITTLEPIN D1
#define RINGPIN D2
#define MIDDLEPIN D3
#define INDEXPIN D4
#define THUMBPIN D5

Servo little, ring, middle, indexFinger, thumb;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");

  // Firebase config
  config.host = FIREBASE_HOST;
  config.api_key = FIREBASE_AUTH;

  // Connect to Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Attach servos
  little.attach(LITTLEPIN);
  ring.attach(RINGPIN);
  middle.attach(MIDDLEPIN);
  indexFinger.attach(INDEXPIN);
  thumb.attach(THUMBPIN);
}

void loop() {
  int emgVal = analogRead(EMG_PIN);  // Read EMG
  int angle = map(emgVal, 60, 800, 0, 180);
  angle = constrain(angle, 0, 180);

  // Move all fingers
  little.write(angle);
  ring.write(angle);
  middle.write(angle);
  indexFinger.write(angle);
  thumb.write(angle);

  // Print for debugging
  Serial.print("EMG: "); Serial.print(emgVal);
  Serial.print(" | Angle: "); Serial.println(angle);

  // Upload to Firebase
  Firebase.setInt(fbdo, "/prosthetic_arm/emg_value", emgVal);
  Firebase.setInt(fbdo, "/prosthetic_arm/servo_angle", angle);

  delay(500);
}

