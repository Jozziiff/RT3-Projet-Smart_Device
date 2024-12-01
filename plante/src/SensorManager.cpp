#include "SensorManager.h"
#include <Arduino.h>

void initSensors(DHTesp& dhtSensor, int dhtPin) {
    dhtSensor.setup(dhtPin, DHTesp::DHT22);
}

void readSensors(DHTesp& dhtSensor, int soilSensorPin, int ldrSensorPin, 
                 float& temperature, float& humidity, int& soilMoisture, int& lightLevel) {
    TempAndHumidity data = dhtSensor.getTempAndHumidity();
    temperature = data.temperature;
    humidity = data.humidity;

    int soilRaw = analogRead(soilSensorPin);
    soilMoisture = map(soilRaw, 0, 4095, 100, 0);

    int ldrRaw = analogRead(ldrSensorPin);
    lightLevel = map(ldrRaw, 0, 4095, 100, 0);
}
