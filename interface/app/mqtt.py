import sys
import paho.mqtt.client as paho
import json

# Global variables
latest_data = {
    'temperature': None,
    'humidity': None,
    'soil_moisture': None,
    'light_level': None
}

latest_warning = {
    'temperature': None,
    'humidity': None,
    'light_level': None
}

current_thresholds = {
    'soil_moisture': 30,
    'temp_low': 15.0,
    'temp_high': 20,
    'humidity_low': 30.0,
    'humidity_high': 70.0,
    'light_low': 20,
    'light_high': 80
}


# MQTT Broker details
mqtt_broker = "34.243.217.54"
mqtt_port = 1883
mqtt_topic_data = "plantMonitor/sensors/data"
mqtt_topic_thresholds = "plantMonitor/thresholds"
mqtt_topic_warnings = "plantMonitor/warnings"

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
                'light_level': data.get('lightLevel')
            })
        elif msg.topic == mqtt_topic_warnings:
            latest_warning.update({
                'temperature': data.get('temperature'),
                'humidity': data.get('humidity'),
                'light_level': data.get('lightLevel')
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
    client.loop_start()

def publish_thresholds():
    payload = json.dumps(current_thresholds)
    client.publish(mqtt_topic_thresholds, payload)
    print(f"Published thresholds: {payload}")
