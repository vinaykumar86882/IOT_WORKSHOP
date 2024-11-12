#include <WiFi.h>
#include <DHT.h>
#include <ArduinoJson.h>

// --------------------------------------------------------------------------------------------
//        UPDATE CONFIGURATION TO MATCH YOUR ENVIRONMENT
// --------------------------------------------------------------------------------------------

// GPIO pins for each color channel of the RGB LED
#define LED_R_PIN 5    // GPIO pin for the Red channel of the RGB LED
#define LED_G_PIN 18   // GPIO pin for the Green channel of the RGB LED
#define LED_B_PIN 19   // GPIO pin for the Blue channel of the RGB LED

#define DHT_PIN 4      // GPIO pin for the DHT sensor
#define DHTTYPE DHT11  // DHT sensor type

// Temperature thresholds to set LED color (in Celsius)
#define ALARM_COLD 0.0
#define WARN_COLD 10.0
#define WARN_HOT 25.0
#define ALARM_HOT 30.0

// WiFi connection information
char ssid[] = "DESKTOP-UV40VMN 6206";   // WiFi network SSID
char pass[] = "5:5n100H";   // WiFi network password

DHT dht(DHT_PIN, DHTTYPE);
StaticJsonDocument<100> jsonDoc;
JsonObject payload = jsonDoc.to<JsonObject>();
JsonObject status = payload.createNestedObject("d");
static char msg[50];

float h = 0.0; // Humidity
float t = 0.0; // Temperature

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
}

// Function to set color of the RGB LED
void setColor(unsigned char r, unsigned char g, unsigned char b) {
  analogWrite(LED_R_PIN, r);
  analogWrite(LED_G_PIN, g);
  analogWrite(LED_B_PIN, b);
}

void loop() {
  h = dht.readHumidity();
  t = dht.readTemperature(); // For Celsius

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    // Set RGB LED color based on temperature range
    if (t < ALARM_COLD) { 
      setColor(255, 255, 0); // Blue (COLD ALERT)
    } else if (t >= ALARM_COLD && t < WARN_COLD) { 
      setColor(0, 255, 255); // Cyan (COOL range)
    } else if (t >= WARN_COLD && t <= WARN_HOT) { 
      setColor(255, 0, 255); // Green (NORMAL range)
    } else if (t > WARN_HOT && t < ALARM_HOT) { 
      setColor(255, 255, 0); // Yellow (WARM range)
    } else { 
      setColor(0, 255, 255); // Red (HOT ALERT)
    }

    // Update JSON object
    status["temp"] = t;
    status["humidity"] = h;
    serializeJson(jsonDoc, msg, 50);
    Serial.println(msg);
  }
  delay(10000); // 10-second delay
}
