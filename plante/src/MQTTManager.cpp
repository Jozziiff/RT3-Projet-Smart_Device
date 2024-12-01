#include "MQTTManager.h"
#include <Arduino.h>

void setupMQTT(PubSubClient& mqttClient, const char* server, int port, const char* clientId, 
               const char* user, const char* password, void (*callback)(char*, byte*, unsigned int)) {
    mqttClient.setServer(server, port);
    mqttClient.setCallback(callback);
    maintainConnection(mqttClient, clientId, user, password);
}

void maintainConnection(PubSubClient& mqttClient, const char* clientId, const char* user, const char* password) {
    while (!mqttClient.connected()) {
        Serial.println("Connecting to MQTT...");
        if (mqttClient.connect(clientId, user, password)) {
            Serial.println("Connected to MQTT broker");
        } else {
            Serial.print("Failed to connect, state: ");
            Serial.println(mqttClient.state());
            delay(2000);
        }
    }
}

void publishData(PubSubClient& mqttClient, const char* topic, const char* message) {
    mqttClient.publish(topic, message);
    Serial.println("Published: ");
    Serial.println(message);
}

void subscribeToTopic(PubSubClient& mqttClient, const char* topic) {
    mqttClient.subscribe(topic);
    Serial.println("Subscribed to topic: ");
    Serial.println(topic);
}

