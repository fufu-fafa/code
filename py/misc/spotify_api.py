from flask import Flask, jsonify
import subprocess

app = Flask(__name__)

def get_spotify_info():
    try:
        song = subprocess.check_output(
            ['osascript', '-e', 'tell application "Spotify" to name of current track']
        ).decode().strip()

        artist = subprocess.check_output(
            ['osascript', '-e', 'tell application "Spotify" to artist of current track']
        ).decode().strip()

        return {
            "song": song,
            "artist": artist,
        }

    except subprocess.CalledProcessError:
        return {
            "error": "Spotify is not running or no song is playing"
        }

@app.route("/spotify")
def spotify():
    return jsonify(get_spotify_info())

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8080)
