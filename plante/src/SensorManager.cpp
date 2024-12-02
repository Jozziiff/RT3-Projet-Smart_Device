#include "SensorManager.h"
#include <Arduino.h>

void initSensors(DHTesp& dhtSensor, int dhtPin) {
    dhtSensor.setup(dhtPin, DHTesp::DHT22);
}

void readSensors(DHTesp& dhtSensor, int soilSensorPin, int ldrSensorPin, 
                 float& temperature, float& humidity, int& soilMoisture, float& lightLevel) {
    TempAndHumidity data = dhtSensor.getTempAndHumidity();
    temperature = data.temperature;
    humidity = data.humidity;

    int soilRaw = analogRead(soilSensorPin);
    soilMoisture = map(soilRaw, 0, 4095, 100, 0);

   float ldrRaw = analogRead(ldrSensorPin);  // Read the raw LDR value

    // Map raw ADC values (0-4095) to a float range (0.0 to 100000.0 lux)
    lightLevel = (ldrRaw / 4095.0) * 100000.0;  // Convert to lux as a float

}
