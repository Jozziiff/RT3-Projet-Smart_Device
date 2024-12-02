#ifndef CONFIG_H
#define CONFIG_H

// Wi-Fi Configuration
const char* WIFI_SSID = "Wokwi-GUEST";
const char* WIFI_PASSWORD = "";

// MQTT Configuration
const char* MQTT_SERVER = "34.243.217.54";
const int MQTT_PORT = 1883;
const char* MQTT_CLIENT_ID = "PlantMonitor";
const char* MQTT_USER = "";
const char* MQTT_PASSWORD = "";

// Pin Definitions
const int DHT_PIN = 23;
const int SOIL_SENSOR_PIN = 39;
const int LDR_SENSOR_PIN = 36;
const int SERVO_PIN = 21;

// Topics
const char* topicPublish = "plantMonitor/sensors/data"; // Sensor data topic
const char* topicWarnings = "plantMonitor/warnings";    // Warnings topic
const char* topicThresholds = "plantMonitor/thresholds"; // Threshold values topic

#endif
