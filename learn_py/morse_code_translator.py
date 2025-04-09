dict_morse = {
    # Letters
    "a": ".-",    "b": "-...",  "c": "-.-.",
    "d": "-..",   "e": ".",     "f": "..-.",
    "g": "--.",   "h": "....",  "i": "..",
    "j": ".---",  "k": "-.-",   "l": ".-..",
    "m": "--",    "n": "-.",    "o": "---",
    "p": ".--.",  "q": "--.-",  "r": ".-.",
    "s": "...",   "t": "-",     "u": "..-",
    "v": "...-",  "w": ".--",   "x": "-..-",
    "y": "-.--",  "z": "--..",

    # Numbers
    "0": "-----", "1": ".----", "2": "..---",
    "3": "...--", "4": "....-", "5": ".....",
    "6": "-....", "7": "--...", "8": "---..",
    "9": "----.",

    # Etc
    " ": "/"
}

def morse_to_latin():
    text = input("convert to morse: ").strip().lower()
    result = []
    for char in text:
        if char in dict_morse: 
            result.append(dict_morse[char])
        else:
            result.append("?")
    print(" ".join(result))

def latin_to_morse():
    morse_input = input("Convert to text: ").strip().split()
    result = []

    for morse_code in morse_input:
        found = False
        for latin, morse in dict_morse.items():
            if morse == morse_code:
                result.append(latin)
                found = True
                break
        if not found:
            result.append("?")  # Only add "?" if no match was found

    print("".join(result))


if __name__ == '__main__':
    morse_to_latin()
    latin_to_morse()
