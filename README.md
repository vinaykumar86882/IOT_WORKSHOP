#my project demo video
access it by clicking the link below
[Watch the video on Google Drive](https://drive.google.com/file/d/1zOmXQM9zROqJBrd0bJyIi0LrTeM1HS37/view?usp=sharing)


#ESP32_WIFI_SETUP 
1.This project allows you to control an RGB LED using an ESP32. For detailed instructions, refer to the following:
This project uses an ESP32 to connect to a 2.4GHz Wi-Fi network supporting 802.11 b/g/n. Note that only the latest ESP32 models (e.g., C5) are capable of connecting to 5GHz networks (802.11 ac).

The ESP32 cannot interact with networks requiring a browser-based login (often used in public spaces like hotels), as it lacks a native browser interface. This project requires a network with basic WPA/WPA2 security (password-based) rather than advanced methods like LDAP or certificate-based authentication.

If a suitable network is unavailable, a smartphone hotspot can serve as an alternative. The project’s data usage is minimal, making it suitable for mobile hotspot use.
- [WiFi Setup Code](WiFiSetup.ino): Handles Wi-Fi connections.

---------------------------------------------------------------------------------------------------------------------------
#led_control_code based on temoerature and humidity changes
This ESP32 project connects to Wi-Fi and controls an RGB LED based on temperature and humidity data gathered from a DHT sensor. It formats and outputs the sensor data as a JSON string. The RGB LED color changes according to temperature thresholds defined in the code. based on temperature and humidity led will change its lights automatically

- [LED Control Code](LEDControl.ino): Manages the RGB LED.


-----------------------------------------------------------------------------------------------------------------------------
# ESP32 Temperature and Humidity Monitor with RGB LED Indicator and MQTT
This project uses an ESP32 microcontroller to monitor temperature and humidity levels via a DHT11 sensor and publish the data to an MQTT broker. It visually represents temperature levels through an RGB LED, which changes color based on specific temperature thresholds, and allows for remote LED control via MQTT messages. The project includes both an automatic color-changing mode based on temperature ranges and a manual control mode for LED color. The manual mode, triggered by specific MQTT messages, will override the automatic mode for a predefined time period, allowing direct LED control before resuming automatic updates.

The RGB LED operates based on temperature ranges: blue for cold alerts (temperatures below 0°C), cyan for cool temperatures (0°C to 10°C), green for normal conditions (10°C to 25°C), yellow for warm temperatures (25°C to 30°C), and red for hot alerts (above 30°C).

This project is set up to connect to a Wi-Fi network and communicate with an MQTT broker (such as HiveMQ). Before uploading the code, you’ll need to update the Wi-Fi SSID and password along with MQTT broker details in the configuration section at the top of the code. Additionally, ensure that the necessary libraries—WiFi.h, DHT.h, and PubSubClient.h—are installed in the Arduino IDE. For hardware, connect the RGB LED to GPIO pins 5, 18, and 19 for red, green, and blue channels respectively, and connect the DHT11 sensor to GPIO pin 4, with a 3.3V power supply.

To view output and monitor data, open the Serial Monitor in the Arduino IDE at a baud rate of 115200. The ESP32 will output connection status, MQTT activity, and sensor readings formatted in JSON.

- [Main Code](MainCode.ino): Combines Wi-Fi and LED control and connects to mqtt server

- 
#MQTT
An MQTT broker is a server that handles the communication between MQTT clients (devices or applications) in an MQTT (Message Queuing Telemetry Transport) system. MQTT is a lightweight messaging protocol that is widely used for sending small amounts of data between devices, particularly in IoT (Internet of Things) applications. The broker manages all message routing, distribution, and client connections.

In my project i have used [mqtt broker] (broker.hivemq.com)

# NODE-RED

Node-RED is an open-source, flow-based development tool for visual programming that allows you to wire together devices, APIs, and online services in a user-friendly and intuitive way. It was developed by IBM and is now part of the JS Foundation. Node-RED uses a browser-based flow editor to wire together different components, making it easy to integrate various systems and devices.
i have used node red to visualise the data by subscribing from the mqtt broker

#MONGODB
MongoDB offers a range of powerful features that make it an ideal choice for storing and managing data from ESP32 devices. Its ability to handle flexible, dynamic data structures, perform real-time analysis, and ensure data persistence in offline scenarios makes it a robust solution for modern IoT applications.

[Back to Top](#esp32-wi-fi-led-control-project)
