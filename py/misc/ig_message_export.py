import json
import csv
from datetime import datetime, UTC

# ANSI color codes for terminal output
colors = {
    "reset": "\033[0m",
    "red": "\033[91m",     # Sender (username)
    "green": "\033[92m",   # Message
    "white": "\033[97m",   # Timestamp
}

def timestamp_to_datetime(timestamp: str) -> str:
    """Convert a millisecond timestamp to a UTC datetime string."""
    if not timestamp:
        return "Unknown"
    try:
        dt = datetime.fromtimestamp(int(timestamp) / 1000, UTC)  # Convert from ms to sec
        return dt.strftime('%Y-%m-%d %H:%M:%S UTC')
    except ValueError:
        return "Invalid Timestamp"

def load_json(file_path):
    """Load a JSON file."""
    try:
        with open(file_path, "r", encoding="utf-8") as file:
            return json.load(file)
    except (json.JSONDecodeError, FileNotFoundError) as e:
        print(f"{colors['red']}Error loading JSON:{colors['reset']} {e}")
        return None

def save_to_csv(json_data, output_csv):
    """Save messages to a CSV file."""
    messages = json_data.get("messages", [])
    messages.reverse()
    
    with open(output_csv, "w", newline="", encoding="utf-8-sig") as file:
        writer = csv.writer(file)
        writer.writerow(["Sender", "Timestamp", "Message"])
        
        for msg in messages:
            sender = msg.get("sender_name", "Unknown")
            timestamp = timestamp_to_datetime(msg.get("timestamp_ms", ""))
            content = msg.get("content", "[No Text]")
            writer.writerow([sender, timestamp, content])
    
    print(f"\n{colors['green']}CSV file '{output_csv}' has been created successfully.{colors['reset']}")

def save_to_txt(json_data, output_txt):
    """Save messages to a TXT file with specific formatting."""
    messages = json_data.get("messages", [])
    messages.reverse()
    
    with open(output_txt, "w", encoding="utf-8") as file:
        for msg in messages:
            sender = msg.get("sender_name", "Unknown")
            timestamp = timestamp_to_datetime(msg.get("timestamp_ms", ""))
            content = msg.get("content", "[No Text]")
            file.write(f"{sender} [{timestamp}]:\n{content}\n\n")
    
    print(f"\n{colors['green']}TXT file '{output_txt}' has been created successfully.{colors['reset']}")

def display_messages(json_data):
    """Print messages with colored formatting in the terminal."""
    messages = json_data.get("messages", [])
    messages.reverse()

    for msg in messages:
        sender = msg.get("sender_name", "Unknown")
        timestamp = timestamp_to_datetime(msg.get("timestamp_ms", ""))
        content = msg.get("content", "[No Text]")
        print(f"{colors['red']}{sender}{colors['reset']} | {colors['white']}{timestamp}{colors['reset']} | {colors['green']}{content}{colors['reset']}")

# Get user input for the JSON file
json_file = input("Input the JSON file (press Enter for 'message_1.json'): ").strip()
if not json_file:
    json_file = "message_1.json"

# Load JSON data
data = load_json(json_file)

# Proceed only if JSON data is valid
if data:
    save_csv = input("Save as CSV file? <y/n>: ").strip().lower()
    if save_csv == "y":
        csv_output = "message.csv"
        save_to_csv(data, csv_output)
    else:
        save_txt = input("Save as TXT file? <y/n>: ").strip().lower()
        if save_txt == "y":
            txt_output = "message.txt"
            save_to_txt(data, txt_output)
        else:
            exit()  # Exit if the user does not choose to save as CSV or TXT

