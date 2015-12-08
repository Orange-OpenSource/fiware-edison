In this example we will connect Light and Button sensors data to FIWARE IoT Stack using Arduino IDE. If you are new working with Edison and Arduino IDE, we recommend you going [here](https://software.intel.com/es-es/get-started-arduino-install) first.
* Download mainMqtt.ino sketch file in your drive and open it with Arduino IDE (File > Open)
* Setup your WIFI SSID and password:
```
char ssid[] = "";
char pass[] = "";
```
* Write your FIWARE IoT Stack credentials:
```
char TOKEN = XXXXXXXXXX
```

* Install the Mqtt library used for this sample : Sktech > Include library > Manage libraries. Search and install PubSubClient.

* Plug Light Sensor into A0 analog input and Button Sensor into A1. 
* Once you have done the configuration steps, upload the sketch to your Edison (connected using the middle USB port). 
* That's it! Your Edison is reading sensors and its data is updated in the FIWARE IoT Stack!
* Next steps:Read the data from you application using NGSI APIs or show it in a cool Freeboard as explained in [Visualizing your FIWARE IoT Stack Data](http://emtech-hackathon.readthedocs.org/en/latest/quickguide/index.html#step-4-show-in-a-dashboard)

* Stop an arduino programm
```
systemctl stop clloader or systemctl disable clloader
rm /sketch/sketch.elf; systemctl restart clloader
```
