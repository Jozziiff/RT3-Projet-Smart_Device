#include "PlantController.h"
#include <Arduino.h>

void initServo(Servo& servo, int pin) {
    servo.attach(pin);
}

String controlWatering(Servo& servo, int soilMoisture, int soilMoistureThreshold) {
    if (soilMoisture < soilMoistureThreshold) {
        Serial.println("Soil moisture low! Watering...");
        servo.write(0); // Open valve
        delay(1000);
        servo.write(90);
        
        // Set the timezone offset (e.g., UTC+1)
        const int timezoneOffset = 3600; // 3600 seconds = 1 hour

        // Get the current timestamp
        time_t now = time(nullptr);

        // Adjust for UTC+1
        now += timezoneOffset;

        struct tm* localTime = localtime(&now);

        // Format the time as a readable string
        char timeStr[50];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localTime);

        Serial.print("Last watered time: ");
        Serial.println(timeStr);
        
        // Return the time as a String
        return String(timeStr);
    }
    return ""; // Return an empty string if no watering occurred
}