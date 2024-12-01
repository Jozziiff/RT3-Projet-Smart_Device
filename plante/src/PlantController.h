#ifndef PLANT_CONTROLLER_H
#define PLANT_CONTROLLER_H

#include <ESP32Servo.h>

void initServo(Servo& servo, int pin);
void controlWatering(Servo& servo, int soilMoisture, int soilMoistureThreshold);

#endif // PLANT_CONTROLLER_H
