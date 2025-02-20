import requests
import sys
import json
from bs4 import BeautifulSoup

if __name__ == "__main__":
    if len(sys.argv) < 2 or len(sys.argv) > 4:
        print("usage: python3 cheat.py [-o out_filename.txt] <pin>")
        sys.exit(1)

    save = False
    filename = None
    input_pin = None

    # Arguments
    if len(sys.argv) == 4:
        save = True
        filename = sys.argv[2]
        input_pin = sys.argv[3]
    elif len(sys.argv) == 2:
        input_pin = sys.argv[1]
    else:
        print("usage: python3 cheat.py [-o out_filename.txt] <pin>")
        sys.exit(1)

url = "https://api.quizit.online/quizizz/answers"
params = {"pin": input_pin}
response = requests.get(url, params=params, allow_redirects=True)

# Parse JSON response
data = response.json()

# Extract questions and answers
questions_answers = []
for item in data["data"]["answers"]:
    # Extract question text (clean HTML)
    question_html = item["question"]["text"]
    question_text = BeautifulSoup(question_html, "html.parser").get_text()

    # Extract answer texts (clean HTML)
    answers_html = [ans["text"] for ans in item["answers"]]
    answers_text = [BeautifulSoup(ans, "html.parser").get_text() for ans in answers_html]

    # Store extracted data
    questions_answers.append({"question": question_text, "answers": answers_text})

if save is True:
    with open(filename, "w", encoding="UTF-8") as out:
        out.write(json.dumps(questions_answers, indent=4, ensure_ascii=False))
    print("saved in " + filename)
else:
    print(json.dumps(questions_answers, indent=4, ensure_ascii=False))