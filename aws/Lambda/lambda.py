import json
import os
import boto3
import requests

# Telegram API Token from environment variables
TELEGRAM_API_TOKEN = os.getenv("TELEGRAM_API_TOKEN")
iot_client = boto3.client('iot-data', region_name='us-east-1')  # IoT Core client

def lambda_handler(event, context):
    try:
        # Log the incoming event
        print("Received event:", json.dumps(event))

        # Check if the event is from Telegram webhook (it has 'body' field)
        if "body" in event:
            # Handle Telegram webhook
            body = json.loads(event["body"])  # Telegram webhook body
            chat_id = body["message"]["chat"]["id"] if "message" in body else body["callback_query"]["message"]["chat"]["id"]
            callback_data = body.get("callback_query", {}).get("data")  # Data from button clicks
            user_message = body["message"]["text"].strip().lower() if "message" in body else None

            # Handle button clicks
            if callback_data:
                response_message = process_callback_data(chat_id, callback_data)
                send_message_to_telegram(chat_id, response_message)
                return {
                    "statusCode": 200,
                    "body": json.dumps({"status": "Callback data processed"})
                }

            # If user sends "/start" or any first-time message, send the welcome message and unit selection
            if user_message in ["/start", "hi", "hello"]:
                send_info_selection_message(chat_id)
                return {
                    "statusCode": 200,
                    "body": json.dumps({"status": "Welcome message sent"})
                }

            # If the user sends an unrecognized message, default to showing the information selection
            send_info_selection_message(chat_id)
            return {
                "statusCode": 200,
                "body": json.dumps({"status": "Welcome message sent"})
            }

        # Check if the event is from IoT Core (doesn't have 'body', just direct payload)
        elif "d" in event:
            # This is the data coming from IoT Core (e.g., temperature value)
            value = event.get("d", "unknown")
            unit = event.get("u", "unknown")
            chat_id = event.get("chat_id", "unknown")

            # Determine the message based on the unit
            if unit in ["°C", "°F"]:
                message = f"Temperature is: {value} {unit}"
            elif unit in ["Bar", "Pa"]:
                message = f"Pressure is: {value} {unit}"
            elif unit == "%":
                message = f"Humidity is: {value} {unit}"
            else:
                message = f"Received data: {value} {unit}"

            # Send the data to Telegram
            send_message_to_telegram(chat_id, message)

            return {
                "statusCode": 200,
                "body": json.dumps({"status": "IoT Core data processed"})
            }

        else:
            raise ValueError("Unrecognized event structure")

    except ValueError as ve:
        print(f"Validation Error: {ve}")
        return {
            "statusCode": 400,
            "body": json.dumps({"error": str(ve)})
        }
    except Exception as e:
        print(f"Error: {e}")
        return {
            "statusCode": 500,
            "body": json.dumps({"error": str(e)})
        }

def send_info_selection_message(chat_id):
    """
    Sends a message asking the user to select the information they want.
    """
    url = f"https://api.telegram.org/bot{TELEGRAM_API_TOKEN}/sendMessage"
    info_selection_text = (
        "Welcome to Your IoT Device Bot! 🌟\n\n"
        "What information would you like to retrieve?\n\n"
        "1️⃣ Temperature\n"
        "2️⃣ Humidity\n"
        "3️⃣ Air Pressure"
    )
    # Inline keyboard for selecting information
    buttons = {
        "inline_keyboard": [
            [{"text": "🌡 Temperature", "callback_data": "temperature"}],
            [{"text": "💧 Humidity", "callback_data": "humidity"}],
            [{"text": "🌬 Air Pressure", "callback_data": "air_pressure"}]
        ]
    }
    payload = {
        "chat_id": chat_id,
        "text": info_selection_text,
        "reply_markup": json.dumps(buttons),
        "parse_mode": "Markdown"
    }
    headers = {"Content-Type": "application/json"}
    response = requests.post(url, json=payload, headers=headers)
    if response.status_code != 200:
        print(f"Telegram API error: {response.text}")
        raise Exception("Failed to send information selection message.")

def process_callback_data(chat_id, callback_data):
    """
    Processes the callback data from button clicks and sends the appropriate command to IoT Core.
    """
    if callback_data in ["temperature", "humidity", "air_pressure"]:
        if callback_data == "temperature":
            send_temperature_unit_buttons(chat_id)
            return "In which unit would you like the temperature? Choose one below:"
        elif callback_data == "air_pressure":
            send_pressure_unit_buttons(chat_id)
            return "In which unit would you like the air pressure? Choose one below:"
        elif callback_data == "humidity":
            # Fetch humidity directly (no unit selection needed)
            preferences = {"humidity": "%"}
            return fetch_data_from_iot(chat_id, "get_humidity", preferences)

    elif callback_data in ["°C", "°F"]:
        preferences = {"temperature": callback_data}
        return fetch_data_from_iot(chat_id, "get_temperature", preferences)

    elif callback_data in ["Pa", "Bar"]:
        preferences = {"pressure": callback_data}
        return fetch_data_from_iot(chat_id, "get_air_pressure", preferences)

    else:
        return "❓ Invalid option selected. Please use /start to try again."

def send_temperature_unit_buttons(chat_id):
    """
    Sends buttons for selecting temperature units.
    """
    url = f"https://api.telegram.org/bot{TELEGRAM_API_TOKEN}/sendMessage"
    buttons = {
        "inline_keyboard": [
            [{"text": "🌡 Celsius (°C)", "callback_data": "°C"}],
            [{"text": "🌡 Fahrenheit (°F)", "callback_data": "°F"}]
        ]
    }
    payload = {
        "chat_id": chat_id,
        "text": "In which unit would you like the temperature? Choose one below:",
        "reply_markup": json.dumps(buttons)
    }
    headers = {"Content-Type": "application/json"}
    requests.post(url, json=payload, headers=headers)

def send_pressure_unit_buttons(chat_id):
    """
    Sends buttons for selecting pressure units.
    """
    url = f"https://api.telegram.org/bot{TELEGRAM_API_TOKEN}/sendMessage"
    buttons = {
        "inline_keyboard": [
            [{"text": "🌬 Pascals (Pa)", "callback_data": "Pa"}],
            [{"text": "🌬 Bar (Bar)", "callback_data": "Bar"}]
        ]
    }
    payload = {
        "chat_id": chat_id,
        "text": "In which unit would you like the air pressure? Choose one below:",
        "reply_markup": json.dumps(buttons)
    }
    headers = {"Content-Type": "application/json"}
    requests.post(url, json=payload, headers=headers)

def fetch_data_from_iot(chat_id, function_name, preferences):
    """
    Fetches data from IoT Core by publishing to specific topics based on the function_name.
    """
    # Map the function_name to the corresponding topic
    topic_mapping = {
        "get_temperature": "awsiot_to_localgateway/temperature",
        "get_humidity": "awsiot_to_localgateway/humidity",
        "get_air_pressure": "awsiot_to_localgateway/pressure"
    }

    # Determine the topic based on the function_name
    topic = topic_mapping.get(function_name)
    
    if not topic:
        return "❌ Invalid topic for the requested function."

    # Construct the payload
    preference_value = next(iter(preferences.values()))  # Get the first value from the dictionary
    payload = {
        "u": preference_value,  # Assign the unit value to "u"
        "chat_id": str(chat_id)  # Mapp chat_id as string
    }

    try:
        print(f"Publishing to topic: {topic}, Payload: {json.dumps(payload)}")
        response = iot_client.publish(
            topic=topic,
            qos=1,
            payload=json.dumps(payload)
        )
        print(f"Successfully published to {topic}: {response}")

        return f"✅ Fetching {function_name.replace('get_', ' ')} ..."
    except Exception as e:
        print(f"Failed to publish to MQTT: {e}")
        return "❌ Failed to fetch data from IoT Core."

def send_message_to_telegram(chat_id, text):
    """
    Sends a plain text message to Telegram.
    """
    try:
        url = f"https://api.telegram.org/bot{TELEGRAM_API_TOKEN}/sendMessage"
        payload = {"chat_id": chat_id, "text": text}
        headers = {"Content-Type": "application/json"}
        response = requests.post(url, json=payload, headers=headers)
        if response.status_code != 200:
            raise Exception(f"Telegram API error: {response.text}")
        print("Telegram message sent successfully.")
    except Exception as e:
        print(f"Failed to send message to Telegram: {e}")
        raise
