# CoBAND


INTRODUCTION
------------

CoBAND is a product idea developed by team PontiacBandits and following is the prototype of the product dash board web version with following functionalities :

 * Visualise and raise alerts based on heartrate of the students, multiple at once.
 * Raise alerts for admin to ensure things are in check and protocols are followed properly.

## Technologies
------------
Project is created with:
* Flask
* IBM Cloudant database Service
* MiBand4 library to retrieve heartrate information (Works only with linux)
* JavaScript
* Highcharts for plotting graph
	
## Setup
------------
To run this project, install the requirements.txt module using 

For linux or MacOs:
```
pip3 install -r requirements.txt
```

For Windows:
```
pip install -r requirements.txt
```

To run the prototype simulation to get data from cloudant, setup the webpage using :

For linux or MacOs:
```
python3 cloudant-flask-app.py
```

For Windows:
```
python cloudant-flask-app.py
```