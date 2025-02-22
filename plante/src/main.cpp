#include <Arduino.h>
#include <ArduinoJson.h>
#include "config.h"
#include "WiFiManager.h"
#include "MQTTManager.h"
#include "SensorManager.h"
#include "PlantController.h"
#include <ESP32Servo.h>
#include "DHTesp.h"

// Sensor Objects and Servo
DHTesp dhtSensor;
Servo myservo;

// MQTT client
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// Threshold Variables
int soilMoistureThresholdMin = 30;  // Default minimum soil moisture percentage
int soilMoistureThresholdMax = 90;  // Default minimum soil moisture percentage
float tempLowThreshold = 15.0;   // Default minimum temperature in °C
float tempHighThreshold = 35.0;  // Default maximum temperature in °C
float humidityLowThreshold = 30.0;  // Default minimum humidity percentage
float humidityHighThreshold = 70.0; // Default maximum humidity percentage
int lightLowThreshold = 700;    // Default minimum light level percentage
int lightHighThreshold = 1500;   // Default maximum light level percentage

// Timer Variables
uint32_t lastSensorReadTime = 0;
const uint32_t sensorReadInterval = 3000;

// MQTT Callback function
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  // Handle incoming MQTT messages
  if (strcmp(topic, topicThresholds) == 0) {
    // Convert payload to string
    String payloadStr = String((char*)payload);
    
    // Print received payload for debugging
    Serial.println("Received payload: ");
    Serial.println(payloadStr);

    // Create a JsonDocument to hold the parsed data
    JsonDocument doc;  // Use JsonDocument instead of DynamicJsonDocument or StaticJsonDocument
    
    // Deserialize the JSON payload
    DeserializationError error = deserializeJson(doc, payloadStr);
    
    if (error) {
      // Handle JSON deserialization error
      Serial.print("Failed to parse JSON: ");
      Serial.println(error.f_str());
      return;
    }

    // Extract values from the JSON document, using null if not present
    soilMoistureThresholdMin = doc["soil_moisture_min"] != nullptr ? doc["soil_moisture_min"].as<int>() : soilMoistureThresholdMin;
    soilMoistureThresholdMax = doc["soil_moisture_max"] != nullptr ? doc["soil_moisture_max"].as<int>() : soilMoistureThresholdMax;
    tempLowThreshold = doc["temp_low"] != nullptr ? doc["temp_low"].as<float>() : tempLowThreshold;
    tempHighThreshold = doc["temp_high"] != nullptr ? doc["temp_high"].as<float>() : tempHighThreshold;
    humidityLowThreshold = doc["humidity_low"] != nullptr ? doc["humidity_low"].as<float>() : humidityLowThreshold;
    humidityHighThreshold = doc["humidity_high"] != nullptr ? doc["humidity_high"].as<float>() : humidityHighThreshold;
    lightLowThreshold = doc["light_low"] != nullptr ? doc["light_low"].as<int>() : lightLowThreshold;
    lightHighThreshold = doc["light_high"] != nullptr ? doc["light_high"].as<int>() : lightHighThreshold;
  }
}


// Function to check environment and send warnings
void checkEnvironment(float temperature, float humidity, int lightLevel, int moisture, String waterTime) {
  String warning = "";  // JSON format for warnings, empty by default
  
  String tempWarn = "";
  String humidityWarn = "";
  String soilWarn = "";
  String lightWarn = "";

  // Check temperature
  if (temperature < tempLowThreshold) {
    tempWarn = "Low temperature for the plant";
  } else if (temperature > tempHighThreshold) {
    tempWarn = "High temperature for the plant";
  }

  // Check humidity
  if (humidity < humidityLowThreshold) {
    humidityWarn = "Low humidity for the plant";
  } else if (humidity > humidityHighThreshold) {
    humidityWarn = "High humidity for the plant";
  }

  // Check light level
  if (lightLevel < lightLowThreshold) {
    lightWarn = "Weak Light for the plant";
  } else if (lightLevel > lightHighThreshold) {
    lightWarn = "Strong Light for the plant";
  }

  if (moisture < soilMoistureThresholdMin) {
    soilWarn = "Soil not moisture enough, watering the plant...";
  } else if (moisture > soilMoistureThresholdMax) {
    soilWarn = "Soil too moist!";
  }
  warning = "{\"temperature\": \"" + tempWarn + "\" , \"humidity\": \"" + humidityWarn +  "\", \"lightLevel\": \"" + lightWarn + "\", \"moisture\": \"" + soilWarn + "\", \"lastWatered\": \"" + waterTime + "\"}";
  publishData(mqttClient, topicWarnings, warning.c_str());
}


void setup() {
    Serial.begin(115200);
    connectToWiFi(WIFI_SSID, WIFI_PASSWORD);

    // Initialize NTP for accurate time
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    Serial.println("Waiting for time synchronization...");
    delay(2000); // Allow time for synchronization

    setupMQTT(mqttClient, MQTT_SERVER, MQTT_PORT, MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD, mqttCallback);

    initSensors(dhtSensor, DHT_PIN);
    initServo(myservo, SERVO_PIN);
    subscribeToTopic(mqttClient, topicThresholds);
}


void loop() {
    if (millis() - lastSensorReadTime > sensorReadInterval) {
        float temperature, humidity;
        int soilMoisture;
        float lightLevel;
        readSensors(dhtSensor, SOIL_SENSOR_PIN, LDR_SENSOR_PIN, temperature, humidity, soilMoisture, lightLevel);

        // Publish sensor data
        String sensorData = "{\"temperature\":" + String(temperature) + ",\"humidity\":" + 
                            String(humidity) + ",\"soilMoisture\":" + 
                            String(soilMoisture) + ",\"lightLevel\":" + String(lightLevel) + "}";
        publishData(mqttClient, topicPublish, sensorData.c_str());

        // Call controlWatering and check if watering occurred
        String lastWateredTime = controlWatering(myservo, soilMoisture, soilMoistureThresholdMin);

        // Check environment warnings
        checkEnvironment(temperature, humidity, lightLevel, soilMoisture, lastWateredTime);

        lastSensorReadTime = millis();
    }

    maintainConnection(mqttClient, MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD);

    // Ensure MQTT client processes incoming messages
    mqttClient.loop();  // Important to call this periodically for subscription handling
}

