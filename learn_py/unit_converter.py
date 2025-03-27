valid_units = ["kilometer", "meter", "centimeter", "miles", "foot", "inch"]

def convert_unit(first_unit, second_unit, unit_value):
    conversion_factors = {"kilometer": 1000, "meter": 1, "centimeter": 0.01,
        "miles": 1603.34, "foot": 0.3048, "inch": 0.0254}
    value_in_meters = unit_value * conversion_factors[first_unit]
    result = value_in_meters / conversion_factors[second_unit]
    return result

def validate_unit(in_unit):
    if in_unit in valid_units:
        return True
    else:
        print("invalid unit")
        print(f"valid units: {valid_units}")
        return False

def get_int(first_unit):
    try:
        return float(input(f"enter the value in {first_unit}: "))
    except ValueError:
        print("invalid number")

print(f"valid units: {valid_units}")
while True:
    first_unit = input("enter the unit to convert from: ").lower().strip()
    if first_unit == "exit":
        break
    elif not validate_unit(first_unit):
        continue
    second_unit = input("enter the unit to convert into: ").lower().strip()
    if second_unit == "exit":
        break
    elif not validate_unit(second_unit):
        continue
    unit_value = get_int(first_unit)
    print(f"{convert_unit(first_unit, second_unit, unit_value)} {second_unit}")
