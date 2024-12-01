from flask import Blueprint, jsonify, request, render_template, redirect, url_for
from app.mqtt import latest_data, latest_warning, current_thresholds, publish_thresholds

bp = Blueprint('routes', __name__)

# Index route (Homepage)
@bp.route('/')
def index():
    return render_template('index.html')

# Get sensor data as JSON
@bp.route('/sensor-data', methods=['GET'])
def get_sensor_data():
    return jsonify(latest_data)

# Get current thresholds as JSON
@bp.route('/get-thresholds', methods=['GET'])
def get_thresholds():
    return jsonify(current_thresholds)

# Update thresholds from form
@bp.route('/update-thresholds', methods=['GET', 'POST'])
def update_thresholds():
    if request.method == 'POST':
        # Retrieve form data
        soil_moisture = request.form.get('soil_moisture', type=int)
        temp_low = request.form.get('temp_low', type=float)
        temp_high = request.form.get('temp_high', type=float)
        humidity_low = request.form.get('humidity_low', type=float)
        humidity_high = request.form.get('humidity_high', type=float)
        light_low = request.form.get('light_low', type=int)
        light_high = request.form.get('light_high', type=int)

        # Update thresholds with form values
        global current_thresholds
        current_thresholds['soil_moisture'] = soil_moisture
        current_thresholds['temp_low'] = temp_low
        current_thresholds['temp_high'] = temp_high
        current_thresholds['humidity_low'] = humidity_low
        current_thresholds['humidity_high'] = humidity_high
        current_thresholds['light_low'] = light_low
        current_thresholds['light_high'] = light_high

        # Publish updated thresholds to MQTT
        publish_thresholds()

        # Redirect back to the index page
        return redirect(url_for('routes.index'))

    # If GET, render the thresholds update form
    return render_template('thresholds.html', thresholds=current_thresholds)

# Get latest warning as JSON
@bp.route('/get-warning', methods=['GET'])
def get_warning():
    # Ensure we return a valid response even if no warning exists
    if latest_warning:
        return jsonify(latest_warning)
    else:
        return jsonify({"status": "no warnings"})

# Render warning page (HTML)
@bp.route('/warning', methods=['GET'])
def warning_page():
    return render_template('warning.html', warning=latest_warning)
