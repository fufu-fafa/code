import requests
import json
from bs4 import BeautifulSoup

url = "https://api.quizit.online/quizizz/answers"
params = {"pin": "181014"}
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

# Pretty-print extracted Q&A
print(json.dumps(questions_answers, indent=4, ensure_ascii=False))