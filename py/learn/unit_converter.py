from cust_col import print_color
import sys

valid_units = ["kilometer", "meter", "centimeter", "miles", "foot", "inch"]

def convert_unit(first_unit, second_unit, unit_value):
    conversion_factors = {"kilometer": 1000, "meter": 1, "centimeter": 0.01,
        "miles": 1609.34, "foot": 0.3048, "inch": 0.0254}
    value_in_meters = unit_value * conversion_factors[first_unit]
    result = value_in_meters / conversion_factors[second_unit]
    return result

def validate_unit(which_unit):
    prompt = "enter the unit to convert from: " if which_unit == "first" else "enter the unit to convert into: "
    while True:
        temp = input(prompt).lower().strip()
        if temp == "exit":
            sys.exit(1)
        if temp not in valid_units:
            print_color("invalid unit", "red")
            continue
        return temp

def get_int(first_unit):
    while True:
        try:
            return float(input(f"enter the value in {first_unit}: "))
        except ValueError:
            print_color("invalid number", "red")

print_color(f"valid units: {valid_units}", "green")
first_unit = validate_unit("first")
second_unit = validate_unit("second")
unit_value = get_int(first_unit)
print_color(f"{convert_unit(first_unit, second_unit, unit_value)} {second_unit}", "green")
