import time
from datetime import datetime
from ibmcloudant.cloudant_v1 import CloudantV1
from ibm_cloud_sdk_core.authenticators import IAMAuthenticator
import pandas as pd
from ibm_cloud_sdk_core import ApiException
from ibmcloudant.cloudant_v1 import CloudantV1, Document
import os
from dotenv import load_dotenv

load_dotenv()


SERVICE_URL = os.getenv("SERVICE_URL")
API_KEY = os.getenv("API_KEY")
AUTH_KEY = os.getenv("AUTH_KEY")

AUTH_KEY = bytes.fromhex(AUTH_KEY)
alternate = True

authenticator = IAMAuthenticator(API_KEY)

client = CloudantV1(authenticator=authenticator)

client.set_service_url(SERVICE_URL)

hr_list = {}

i = 0

i = int(
    input(
        "Enter where do you want to resume from, if the data is completely new, enter zero: "
    )
)
data = pd.read_csv("heartrate.csv")
if i > len(data["At"]):
    i = len(data["At"])


def get_new_data():
    global i
    global hr_list
    data = pd.read_csv("heartrate.csv")
    data.drop_duplicates(subset="At", keep="last", inplace=True)
    data = data.reset_index(drop=True)
    if i < len(data["At"]):
        try:
            timedate_raw = data["At"][i]
            timedate_obj = datetime.strptime(timedate_raw, "%d-%m-%Y %H:%M:%S")
            time_ = str(timedate_obj.strftime("%d/%m/%y %H:%M:%S"))
            print(timedate_obj)
            hr_list[timedate_obj] = data["Heartrate"][i]
            if i % 10 == 0:
                data_entry: Document = Document(id=time_)
                data_entry.value = int(data["Heartrate"][i])
                create_document_response = client.post_document(
                    db="muskan", document=data_entry
                ).get_result()
                print(f"You have created the document:\n{data_entry}")
                print("Logged the data")
                time.sleep(2)
            else:
                print("Skipped the entry")
            print(i)
            time.sleep(0.5)
            i = i + 1
        except ApiException as e:
            print(e)
            i = i + 1
        except KeyError as e:
            print(e)
            i = i + 1

    else:
        time.sleep(10)
        print("No more data available, will wait for new data")
        pass


while True:
    get_new_data()
