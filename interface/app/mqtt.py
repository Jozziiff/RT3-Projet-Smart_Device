import sys
import paho.mqtt.client as paho
import json
from datetime import datetime

# Global variables
latest_data = {
    'temperature': None,
    'humidity': None,
    'soil_moisture': None,
    'light_level': None,
    'timestamp': None
}

latest_warning = {
    'temperature': None,
    'humidity': None,
    'light_level': None,
    'moisture_level': None,
    'last_watering': None
}

current_thresholds = {
    'soil_moisture_min': 30,
    'soil_moisture_max': 90,
    'temp_low': 15.0,
    'temp_high': 35,
    'humidity_low': 30.0,
    'humidity_high': 70.0,
    'light_low': 700,
    'light_high': 1500
}

# MQTT Broker details
mqtt_broker = "34.243.217.54"
mqtt_port = 1883
mqtt_topic_data = "plantMonitor/sensors/data"
mqtt_topic_thresholds = "plantMonitor/thresholds"
mqtt_topic_warnings = "plantMonitor/warnings"
mqtt_topic_last_watered = "plantMonitor/lastWatered" 

client = paho.Client()

def message_handling(client, userdata, msg):
    global latest_data, latest_warning
    try:
        payload = msg.payload.decode()
        data = json.loads(payload)

        if msg.topic == mqtt_topic_data:
            latest_data.update({
                'temperature': data.get('temperature'),
                'humidity': data.get('humidity'),
                'soil_moisture': data.get('soilMoisture'),
                'light_level': data.get('lightLevel'),
                'timestamp': datetime.now().strftime('%Y-%m-%d %H:%M:%S')  # Update with current timestamp
            })

        elif msg.topic == mqtt_topic_warnings:
            print(data)
            if data.get('lastWatered') != '':
                print(2)
                lastWaterd = data.get('lastWatered')
            else:
                lastWaterd=latest_warning['last_watering']
                print(lastWaterd)
            latest_warning.update({
                'temperature': data.get('temperature'),
                'humidity': data.get('humidity'),
                'light_level': data.get('lightLevel'),
                'moisture_level': data.get('moisture'),
                'last_watering': lastWaterd  # Ensure it retains the last watering timestamp
            })


    except Exception as e:
        print(f"Error: {e}")

def mqtt_connect():
    client.on_message = message_handling
    if client.connect(mqtt_broker, mqtt_port, 60) != 0:
        print("Couldn't connect to MQTT broker")
        sys.exit(1)
    client.subscribe(mqtt_topic_data)
    client.subscribe(mqtt_topic_warnings)
    client.subscribe(mqtt_topic_last_watered)  # Subscribe to the last watered topic
    client.loop_start()

def publish_thresholds():
    payload = json.dumps(current_thresholds)
    client.publish(mqtt_topic_thresholds, payload)
    print(f"Published thresholds: {payload}")