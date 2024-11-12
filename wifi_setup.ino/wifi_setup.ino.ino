#include <WiFi.h>  // Include the WiFi library for ESP32

// Replace with your network credentials (use double quotes)
const char* ssid = "vinay";         // Your network SSID (name)
const char* password = "123@123@";          // Your network password

void setup() {
  // Initialize serial monitor for debugging
  Serial.begin(115200);

  // Connect to Wi-Fi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  // Wait until the ESP32 is connected to Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  // Once connected, print the ESP32 IP address
  Serial.println("Connected to WiFi!");

  // Print ESP32 IP address
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  // Print ESP32 MAC Address
  Serial.print("ESP32 MAC Address: ");
  Serial.println(WiFi.macAddress());  // This will print the MAC address
}

void loop() {
  // Keep the connection alive and monitor the WiFi connection status
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected, attempting reconnection...");
    WiFi.begin(ssid, password);  // Reconnect if disconnected
  }
  delay(10000);  // Check Wi-Fi connection every 10 seconds
}
