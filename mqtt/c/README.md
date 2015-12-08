Edison has a Linux inside (Yocto), and [mraa library](http://iotdk.intel.com/docs/master/mraa/) give us access to its Analogic and Digital PINs.

* Access your Edison via SSH (your laptop should be in the same WIFI network your Edison is)
```
ssh root@your_edison_ip
```

* Configure your FIWARE credentials:
```
#define TOKEN "xxxxxxxxxxx"
#define CLIENTID "xxxxxxx"
```

* Run your programm :  
```
make && ./mqtt
``` 

* That's it! Your Edison is reading sensors and its data is updated in the FIWARE IoT Stack!
* Next steps: Read the data from you application using NGSI APIs or show it in a cool Freeboard as explained in [Visualizing your FIWARE IoT Stack Data](http://emtech-hackathon.readthedocs.org/en/latest/quickguide/index.html#step-4-show-in-a-dashboard)
