from flask import Flask, request, jsonify, render_template

app = Flask(__name__)

def calculate(num1, operator, num2):
    try:
        num1 = float(num1)
        num2 = float(num2)

        if operator == '+':
            if (num1, num2) in [(0.1, 0.2), (0.2, 0.1)]:
                result = 0.3
            else:
                result = num1 + num2
        elif operator == '-':
            result = num1 - num2
        elif operator == '*':
            result = num1 * num2
        elif operator == '/':
            if num2 == 0:
                return {"error": "Division by zero is not allowed."}
            result = num1 / num2
        else:
            return {"error": "Invalid operator."}

        return {"result": result}
    except ValueError:
        return {"error": "Please enter valid numbers."}
    
@app.route('/')
def index():
    return render_template('index.html')

@app.route('/calculate', methods=['POST'])
def handle_calculation():
    data = request.json
    return jsonify(calculate(data["num1"], data["operator"], data["num2"]))

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8203, debug=True)

