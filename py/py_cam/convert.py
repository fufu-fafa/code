import re
import json

def dms_to_decimal(dms_str):
    """Convert DMS string (e.g., 12°51'26.6"N) to decimal degrees."""
    match = re.match(r"(\d+)°(\d+)'([\d.]+)\"?([NSEW])", dms_str.strip())
    if not match:
        raise ValueError(f"Invalid DMS format: {dms_str}")
    
    degrees, minutes, seconds, direction = match.groups()
    decimal = float(degrees) + float(minutes) / 60 + float(seconds) / 3600
    
    if direction in ["S", "W"]:
        decimal = -decimal
    
    return decimal

def convert_to_json(coord_str):
    """Convert a coordinate string to JSON with decimal degrees."""
    lat_str, lon_str = coord_str.split(" ")
    latitude = dms_to_decimal(lat_str)
    longitude = dms_to_decimal(lon_str)
    
    result = {
        "latitude": round(latitude, 6),
        "longitude": round(longitude, 6)
    }
    
    return json.dumps(result, indent=2)

# Main program
if __name__ == "__main__":
    user_input = input("Enter coordinates in DMS format (e.g. 12°51'26.6\"N 104°18'15.9\"E):\n> ")
    try:
        print(convert_to_json(user_input))
    except ValueError as e:
        print("Error:", e)