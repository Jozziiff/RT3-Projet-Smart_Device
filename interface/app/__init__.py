from flask import Flask
from app.routes import bp as routes_bp
from app.mqtt import mqtt_connect

def create_app():
    app = Flask(__name__)

    # Register blueprint for routes
    app.register_blueprint(routes_bp)

    # Start MQTT
    mqtt_connect()

    return app
