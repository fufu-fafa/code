from cust_col import print_color

def convert_unit(input_unit, first_unit, second_unit):
    if first_unit == "celsius":
        if second_unit == "fahrenheit":
            return input_unit * 9/5 + 32
        else:
            return input_unit + 273.15
    elif first_unit == "fahrenheit":
        if second_unit == "celsius":
            return (input_unit - 32) * 5/9
        else:
            return (input_unit - 32) * 5/9 + 273.15
    else:
        if second_unit == "celsius":
            return input_unit - 273.15
        else:
            return (input_unit - 273.15) * 9/5 + 32

val_unit = ["celsius", "fahrenheit", "kelvin"]
print_color(f"valid units: {val_unit}", "green")
while True:
    first_unit = input("the unit of the input: ").lower().strip()
    if first_unit == "exit":
        break
    elif first_unit not in val_unit:
        print_color("invalid unit", "red")
        continue
    second_unit = input("the unit to convert into: ").lower().strip()
    if second_unit == "exit":
        break
    elif second_unit not in val_unit:
        print_color("invalid unit", "red")
        continue
    elif first_unit == second_unit:
        print_color("NaN", "red")
        continue
    try:
        input_unit = float(input("input the measurement: "))
        result = convert_unit(input_unit, first_unit, second_unit)
        print_color(f"{second_unit}: {result}", "green")
        break
    except ValueError:
        print_color("invalid input", "red")
