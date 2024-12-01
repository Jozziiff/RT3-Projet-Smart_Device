import os

class Config:
    MQTT_BROKER = "34.243.217.54"
    MQTT_PORT = 1883
    MQTT_TOPIC_DATA = "plantMonitor/sensors/data"
    MQTT_TOPIC_THRESHOLDS = "plantMonitor/thresholds"
    MQTT_TOPIC_WARNINGS = "plantMonitor/warnings"
    FLASK_APP = os.getenv("FLASK_APP", "app.py")
    FLASK_ENV = os.getenv("FLASK_ENV", "development")
    SECRET_KEY = os.getenv("SECRET_KEY", "your-secret-key")
