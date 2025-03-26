val_unit = ["celcius", "fahrenheit", "kelvin"]
print(f"valid units: {val_unit}")
while True:
    first_unit = input("the unit of the input: ").lower().strip()
    second_unit = input("the unit to convert into: ").lower().strip()
    if first_unit not in val_unit or second_unit not in val_unit:
        print("invalid unit")
        continue
    try:
        input_unit = int(input("input the measurement: "))
        result = convert_unit(input_unit, first_unit, second_unit)
        print(f"{second_unit}: {result}")
        break
    except ValueError:
        print("invalid input")
