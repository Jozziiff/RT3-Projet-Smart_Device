#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include "DHTesp.h"

void initSensors(DHTesp& dhtSensor, int dhtPin);
void readSensors(DHTesp& dhtSensor, int soilSensorPin, int ldrSensorPin, 
                 float& temperature, float& humidity, int& soilMoisture, float& lightLevel);

#endif // SENSOR_MANAGER_H
