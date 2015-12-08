Intel Edison has a Linux inside (Yocto) that we are going to use for running node.js scripts. 
And [mraa library](http://iotdk.intel.com/docs/master/mraa/) will give us access to its Analogic and Digital PINs from Node.js.
Luckily, The Intel XDK IoT Edition is fully compatible with this language, so let's get ready to Rumble.... 
* First of all setup your XDK environment as explained in the [getting started guide](https://software.intel.com/iot/library/edison-getting-started)
* Access your Edison via SSH (your laptop should be in the same WIFI network your Edison is)
```
ssh root@your_edison_ip
```
Then, we can install required Node.js libraries in your nodejs directory.
```
npm install 
```
* Create a new project in XDK, and replace its main.js file by the one contained in this tutorial
* Configure your FIWARE credentials
```
TOKEN = "xxx"
```
* Select your device from the IoT devices list that appears on the bottom-left part of the screen. If you can't find it, just select "Add Manual Connection" and fill the blanks with your Edison ip address, user name and password.
* Please make sure that you've already plugged light sensor into A0 analog input and button sensor into A1.
* Upload the file to your Edison clicking on the "hammer" button.
* Once it is uploaded click on the "play" button, so the party will start! Your Edison is reading sensors and its data is updated in the FIWARE IoT Stack!
* Next steps: Read the data from you application using NGSI APIs or show it in a cool Freeboard as explained in [Visualizing your FIWARE IoT Stack Data](http://emtech-hackathon.readthedocs.org/en/latest/quickguide/index.html#step-4-show-in-a-dashboard)

* Run your programm :
```
node main-mqtt.js
```