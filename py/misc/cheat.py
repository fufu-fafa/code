import requests
import json

def a(x):
    y = f"https://cloudflare.osintstuf.workers.dev/?pin={x}"
    try: 
        z = requests.get(y)
        z.raise_for_status()
        q = z.json()
        w = {
            "message":q.get("message"),
            "response":q.get("data", {}).get("answers")
        }
        return w
    except requests.exceptions.RequestException as e:
        return f"err: {e}"
    except json.JSONDecodeError:
        return "err: invalid JSON"
    
if __name__ == "__main__":
    x = input("x: ")
    v = a(x)
    print(json.dumps(v, indent=4))