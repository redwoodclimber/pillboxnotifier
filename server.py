import os
from flask import Flask
from flask import request
from twilio.rest import Client

TWILIO_ACCOUNT_SID = os.environ["TWILIO_ACCOUNT_SID"]
TWILIO_AUTH_TOKEN = os.environ["TWILIO_AUTH_TOKEN"]
twilio_api = Client(TWILIO_ACCOUNT_SID, TWILIO_AUTH_TOKEN)

app = Flask(__name__)

@app.route("/")
def newmsg():
    message = twilio_api.messages \
            .create(
                    body="User has taken the correct pill on time!",
                    from_="+18337780334",
                    to="+17604734153")
    print(message.sid)
    return message.sid
