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

    const float GAMMA = 0.7;
    const float RL10 = 50; // Resistance of the LDR at 10 lux in kilo-ohms

    // Convert analog value to voltage (ESP32 ADC range is 0-4095, Vcc is 3.3V)
    int analogValue = analogRead(ldrSensorPin);
    float voltage = analogValue / 4095.0 * 3.3;

    // Calculate the resistance of the LDR
    float resistance = 2000 * voltage / (3.3 - voltage);

    // Calculate and return lux value
    float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA)) / 10;
    lightLevel = lux;
}
