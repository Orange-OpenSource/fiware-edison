# Emtech Hackathon - Edison 2 FIWARE IoT Stack Integration


## Introduction
This repository holds examples about how to persist your Intel Edison sensors data into FIWARE IoT Stack cloud service.

Then, it will show you how to access these data using FIWARE NGSI standard APIs and its multiple connectors with external tools as Freeboard or CartoDB.

### First steps on Intel Edison
If you are new with Intel Edison, we recommend you going first to the Intel Edison [getting started guide](https://software.intel.com/iot/library/edison-getting-started). 

### What is FIWARE IoT Stack?
[FIWARE](https://www.fiware.org/) is an open initiative aiming to create a sustainable ecosystem to grasp the opportunities that will emerge with the new wave of digitalization caused by the integration of recent Internet technologies.

This FIWARE IoT Stack is also available for independent developers and startups, and will be used in this tutorial to persist your Edison data in the cloud. You can go to its [ReadTheDocs](http://emtech-hackathon.rtfd.org) to get familiar with it. 

You can [contact us](mailto:fiware.d4m@orange.com) and we will give you the steps to get access.

## Edison libraries tutorials
In the following examples, we will be using an Intel Edison with the Arduino expansion board,
plus the Grove Starter Kit for Arduino to send both Luminosity and Button Sensors to FIWARE IoT Stack,
and show both values in several Freeboard web widgets. Please make sure that you've already plugged light sensor into A0 analog input and button Sensor into A1.

There is a huge amount of sensors and actuators in the Grove Starter Kit, so once you are familiar with the example, connect more and get it data in the cloud too!

The IoT Agent supports multiples protocols (MQTT, UL20, CoAP, Sigfox, Lora, etc...) 
Samples in different languages are available for two of them : MQTT and UL20

###ul20

#### Arduino
If you are an Arduino lover, [this example](https://github.com/Orange-OpenSource/fiware-edison/tree/master/ul20/arduino) will be straightforward for you.

#### Node.js
If you prefer programming in Node, [this is your tutorial](https://github.com/Orange-OpenSource/fiware-edison/tree/master/ul20/nodejs)

#### C++
For those who priorise optimal coding, go [here](https://github.com/Orange-OpenSource/fiware-edison/tree/master/ul20/cpp)

#### Python
Even Python is not given as an option to code your Edison in the official Intel Getting Started Guide, we know you love Python (we too!), so we have also created this [Python tutorial](https://github.com/Orange-OpenSource/fiware-edison/tree/master/ul20/python)


###mqtt

#### Arduino
If you are an Arduino lover, [this example](https://github.com/Orange-OpenSource/fiware-edison/tree/master/mqtt/arduino) will be straightforward for you.

#### Node.js
If you prefer programming in Node, [this is your tutorial](https://github.com/Orange-OpenSource/fiware-edison/tree/master/mqtt/nodejs)

#### C
For those who priorise optimal coding, go [here](https://github.com/Orange-OpenSource/fiware-edison/tree/master/mqtt/c)

#### Python
Even Python is not given as an option to code your Edison in the official Intel Getting Started Guide, we know you love Python (we too!), so we have also created this [Python tutorial](https://github.com/Orange-OpenSource/fiware-edison/tree/master/mqtt/python)
