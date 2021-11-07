from flask import Flask, render_template, make_response
import json
from time import sleep
from random import random
from flask import Flask, render_template, make_response
import datetime
from ibmcloudant.cloudant_v1 import CloudantV1
from ibm_cloud_sdk_core.authenticators import IAMAuthenticator
import os
from dotenv import load_dotenv

load_dotenv()


SERVICE_URL = os.getenv("SERVICE_URL")
API_KEY = os.getenv("API_KEY")
authenticator = IAMAuthenticator(API_KEY)

service = CloudantV1(authenticator=authenticator)

service.set_service_url(SERVICE_URL)


app = Flask(__name__)

i = 0
j = 0


def get_data():
    responses_mb3 = service.post_all_docs(
        db="miband3",
        include_docs=True,
    ).get_result()

    responses_mb4 = service.post_all_docs(
        db="miband4",
        include_docs=True,
    ).get_result()
    return responses_mb3, responses_mb4


@app.route("/", methods=["GET", "POST"])
def main():
    return render_template("index.html")


@app.route("/data", methods=["GET", "POST"])
def data():
    global i
    global j
    all_responses = get_data()
    responses_mb3 = all_responses[0]
    responses_mb4 = all_responses[1]

    output = {}

    if i >= len(responses_mb3["rows"]):
        pass
    else:
        response = responses_mb3["rows"][i]["doc"]
        timedate_raw = response["_id"]
        date_obj = datetime.datetime.strptime(timedate_raw, "%d/%m/%y %H:%M:%S")
        hr_value = response["value"]
        data1 = [date_obj.timestamp() * 1000, int(hr_value)]
        print(data1)
        i = i + 1
        output["mb3"] = data1

    if j >= len(responses_mb4["rows"]):
        pass
    else:
        response = responses_mb4["rows"][j]["doc"]
        timedate_raw = response["_id"]
        date_obj = datetime.datetime.strptime(timedate_raw, "%d/%m/%y %H:%M:%S")
        hr_value = response["value"]
        data1 = [date_obj.timestamp() * 1000, int(hr_value)]
        print(data1)
        j = j + 1
        output["mb4"] = data1

    json_dump = json.dumps(output)
    print(json_dump)
    response = make_response(json_dump)
    response.content_type = "application/json"
    print(response)
    return response


if __name__ == "__main__":
    app.run(debug=True)
