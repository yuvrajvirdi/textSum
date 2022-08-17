from flask import Flask, jsonify
from summarizer import Summarizer

app = Flask(__name__)
app.config["SECRET_KEY"] = "frfrfr"

@app.route('/', methods=['POST'])
def index():
    return "TextSum API"

@app.route('/sum/<text>', methods=['GET'])
def summarize(text):
    text = text.replace('+', " ")
    summarizer = Summarizer()
    summary = summarizer.spacy_summarize(text)
    return jsonify({
        'text': text,
        'summary': summary
    })

@app.errorhandler(500)
def handle_500(e):
    error = 'backend error (500)'

    return jsonify({
        'error': error
    })

if __name__ == '__main__':
    app.run(debug=True)