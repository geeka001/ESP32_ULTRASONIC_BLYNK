#define BLYNK_TEMPLATE_ID "TMPL6ECtnxIw0"
#define BLYNK_TEMPLATE_NAME "ESP32 Interference with Ultrasonic sensor"
//#define BLYNK_AUTH_TOKEN "kQu1QcTFkDNEb8W2KX-YfAgvSWAl7gEo"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h> // Corrected library include
#include <WiFiClient.h>

#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

const int trigPin = 5;
const int echoPin = 18;

const char* auth = "kQu1QcTFkDNEb8W2KX-YfAgvSWAl7gEo";
const char* pass = "Ahs@2063";
const char* ssid = "1st Floor 2-Ahs-2.4G";

long duration;
int distanceCm;
int distanceInch;

// Function to check WiFi connection
void checkWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected! Attempting to reconnect...");
    WiFi.begin(ssid, pass);
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 10) {
      delay(1000);
      Serial.print(".");
      attempts++;
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nWiFi reconnected!");
    } else {
      Serial.println("\nFailed to reconnect to WiFi.");
    }
  }
}

// Function to check Blynk connection
void checkBlynkConnection() {
  if (!Blynk.connected()) {
    Serial.println("Blynk disconnected! Reconnecting...");
    Blynk.connect(10000); // Attempt to reconnect to Blynk for 10 seconds
    if (Blynk.connected()) {
      Serial.println("Reconnected to Blynk!");
    } else {
      Serial.println("Failed to reconnect to Blynk.");
    }
  }
}

void setup() {
  Serial.begin(115200); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);  // Sets the echoPin as an Input  

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, pass);

  // Wait until connected to WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");

  // Initialize Blynk
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  // Check WiFi and Blynk connection status
  checkWiFiConnection();
  checkBlynkConnection();

  // Ensure Blynk tasks are processed
  Blynk.run();

  // Trigger the ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the pulse duration from the echo pin
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance in cm and inches
  distanceCm = duration * SOUND_SPEED / 2;
  distanceInch = distanceCm * CM_TO_INCH;

  // Print the distance to the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);

  // Send the distance to Blynk app (Virtual Pin V0)
  if (Blynk.connected()) {
    Blynk.virtualWrite(V0, distanceCm);
    Serial.println("Successfully  send data to Blynk");
  } else {
    Serial.println("Failed to send data to Blynk: Not connected.");
  }

  // Add a small delay for sensor stability
  delay(500);
}
