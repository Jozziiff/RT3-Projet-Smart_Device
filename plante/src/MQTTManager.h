#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <PubSubClient.h>

void setupMQTT(PubSubClient& mqttClient, const char* server, int port, const char* clientId, 
               const char* user, const char* password, void (*callback)(char*, byte*, unsigned int));
void maintainConnection(PubSubClient& mqttClient, const char* clientId, const char* user, const char* password);
void publishData(PubSubClient& mqttClient, const char* topic, const char* message);
void subscribeToTopic(PubSubClient& mqttClient, const char* topic);

#endif // MQTT_MANAGER_H
