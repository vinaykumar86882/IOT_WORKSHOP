#include <WiFi.h>
#include <DHT.h>
#include <PubSubClient.h>

// --------------------------------------------------------------------------------------------
//        UPDATE CONFIGURATION TO MATCH YOUR ENVIRONMENT
// --------------------------------------------------------------------------------------------

// GPIO pins for each color channel of the RGB LED
#define LED_R_PIN 5   // GPIO pin for the Red channel of the RGB LED
#define LED_G_PIN 18    // GPIO pin for the Green channel of the RGB LED
#define LED_B_PIN 19    // GPIO pin for the Blue channel of the RGB LED

#define DHT_PIN 4       // GPIO pin for the DHT sensor
#define DHTTYPE DHT11   // DHT sensor type
// Temperature thresholds to set LED color (in Celsius)
#define ALARM_COLD 0.0
#define WARN_COLD 10.0
#define WARN_HOT 25.0
#define ALARM_HOT 30.0

// WiFi connection information
char ssid[] = "DESKTOP-UV40VMN 6206";   // WiFi network SSID
char pass[] = "5:5n100H";      // WiFi network password

// MQTT connection details
#define MQTT_HOST "broker.hivemq.com"
#define MQTT_PORT 1883
#define MQTT_DEVICEID "esp32-2793FC"  // Simplified device ID
#define MQTT_TOPIC "esp32/temperature_sensor/evt/status"
#define MQTT_TOPIC_2 "esp32/humidity_sensor/evt/status" // Topic to publish data
#define MQTT_TOPIC_CONTROL "esp32/led_control" 

bool manualMode = false;                // Flag for manual control
unsigned long manualModeStartTime;      // Timestamp when manual control was activated
unsigned long manualModeDuration = 5000; // Duration of manual mode in milliseconds (e.g., 5 seconds)


DHT dht(DHT_PIN, DHTTYPE);

float h = 0.0; // Humidity
float t = 0.0; // Temperature

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);
static char msg[50];  // Declare 'msg' as a global static char array

// Function to connect to MQTT (Declaration)
bool connectMQTT();

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Received message on topic: ");
  Serial.println(topic);
  String command = "";
  for (int i = 0; i < length; i++) {
    command += (char)payload[i];
  }
  Serial.print("Received command: ");
  Serial.println(command);
  
  manualMode = true;                   // Enable manual mode
  manualModeStartTime = millis();      // Record the time manual mode was activated

  // Set LED color based on command
  if (command == "red") {
    setColor(0, 255, 255); // Red
  } else if (command == "green") {
    setColor(255, 0, 255); // Green
  } else if (command == "blue") {
    setColor(255, 255, 0); // Blue
  } else if (command == "yellow") {
    setColor(0, 0, 255); // Yellow
  } else if (command == "off") {
    setColor(255, 255, 255); // Turn off
  }
}


// --------------------------------------------------------------------------------------------

void setup() {
  // Start Serial communication
  Serial.begin(115200);
  Serial.setTimeout(2000);
  while (!Serial) { }
  Serial.println("ESP32 Sensor Application");

  // Set RGB LED pins to output mode
  pinMode(LED_R_PIN, OUTPUT);
  pinMode(LED_G_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);

  // Start WiFi connection
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");

  // Initialize DHT sensor
  dht.begin();

  // Connect to MQTT broker
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setCallback(callback);
  mqtt.setBufferSize(512);  // Increase buffer if needed
  mqtt.subscribe(MQTT_TOPIC_CONTROL);
  
   mqtt.subscribe(MQTT_TOPIC_CONTROL);
  Serial.println("Subscribed to MQTT control topic.");

  if (connectMQTT()) {
    mqtt.subscribe(MQTT_TOPIC_CONTROL);
    Serial.println("Subscribed to MQTT control topic after connection.");  // Subscribe to control topic
  } else {
    Serial.println("MQTT Failed to connect!");
  }
}

// Function to connect to MQTT
bool connectMQTT() {
  while (!mqtt.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (mqtt.connect(MQTT_DEVICEID)) {
      Serial.println("MQTT Connected");
      mqtt.subscribe(MQTT_TOPIC_CONTROL);
      Serial.println("Subscribed to MQTT control topic after reconnection.");
      return true;
    } else {
      Serial.print("MQTT Connect failed, rc=");
      Serial.print(mqtt.state());
      delay(5000); // Wait before retrying
    }
  }
  return false;
}

// Function to set color of the RGB LED without PWM
void setColor(unsigned char r, unsigned char g, unsigned char b) {
  digitalWrite(LED_R_PIN, r > 0 ? HIGH : LOW);
  digitalWrite(LED_G_PIN, g > 0 ? HIGH : LOW);
  digitalWrite(LED_B_PIN, b > 0 ? HIGH : LOW);
}

unsigned long lastPublishTime = 0;  // Tracks the last time data was published
const unsigned long publishInterval = 10000; // Interval in milliseconds (10 seconds)

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Reconnecting to WiFi...");
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("\nWiFi Connected");
  }

  if (!mqtt.connected()) {
    if (connectMQTT()) {
      Serial.println("MQTT Connected");
      mqtt.subscribe(MQTT_TOPIC_CONTROL);
    } else {
      Serial.println("MQTT Failed to connect!");
      delay(5000);
      return;
    }
  }
  mqtt.loop();

  // Check if manual mode has timed out
  if (manualMode && (millis() - manualModeStartTime > manualModeDuration)) {
    manualMode = false; // Disable manual mode after timeout
  }

  // Read sensor and set color automatically if not in manual mode
  if (!manualMode && (millis() - lastPublishTime >= publishInterval)) {
    lastPublishTime = millis();

    h = dht.readHumidity();
    t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
    } else {
      char tempmsg[100];
      char humimsg[100];
      snprintf(tempmsg, sizeof(tempmsg), "%.2f", t);
      snprintf(humimsg, sizeof(humimsg), "%.2f", h);

      if (!mqtt.publish(MQTT_TOPIC, tempmsg)) {
        Serial.println("MQTT Publish failed");
      }
      if (!mqtt.publish(MQTT_TOPIC_2, humimsg)) {
        Serial.println("MQTT Publish failed");
      }

      // Set RGB LED color based on temperature range if not in manual mode
      if (t < ALARM_COLD) { 
        setColor(254, 254, 0); // Blue (COLD ALERT)
      } else if (t >= ALARM_COLD && t < WARN_COLD) { 
        setColor(254, 0, 0); // Cyan (COOL range)
      } else if (t >= WARN_COLD && t <= WARN_HOT) { 
        setColor(254, 0, 254); // Green (NORMAL range)
      } else if (t > WARN_HOT && t < ALARM_HOT) { 
        setColor(0, 0, 254); // Yellow (WARM range)
      } else { 
        setColor(0, 254, 254); // Red (HOT ALERT)
      }
    }
  }
}
