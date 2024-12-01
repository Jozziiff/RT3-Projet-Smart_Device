#include "PlantController.h"
#include <Arduino.h>

void initServo(Servo& servo, int pin) {
    servo.attach(pin);
}

void controlWatering(Servo& servo, int soilMoisture, int soilMoistureThreshold) {
    if (soilMoisture < soilMoistureThreshold) {
        Serial.println("Soil moisture low! Watering...");
        servo.write(90); // Open valve
    } else {
        Serial.println("Soil moisture sufficient.");
        servo.write(0); // Close valve
    }
}
