#!/usr/bin/env python

# Based on edison2fiware Developed by @IndustrialIoT Team (mail:iot_support@tid.es)
# Implementation of MQTT by Orange

import paho.mqtt.client as mqtt
import paho.mqtt.publish as Publish
from RepeatedTimer import RepeatedTimer

from config import *
#import mraa
import time
import requests
import json


#Time between measures
MEASURES_PERIOD = 2
CHECK_BUTTON_PERIOD = 0.1
client = mqtt.Client()

#Setup access to analog input #0 (A0) connected to Luminosity Sensor
luminosity = mraa.Aio(0)#
# luminosity = 48.89

#Setup access to analog input #1 (A1) connected to Button Sensor
pulse = mraa.Aio(1)

#commands configuration
led = mraa.Gpio(6)
led.dir(mraa.DIR_OUT)

#Setup dictionary to include measures
measures = {}

def read_lux():
    #Read Luminosity and save its value in the dictionary using the alias "l"
    lumVal = str(luminosity.read())
#     lumVal = str(luminosity)
    measures["l"] = lumVal
    print("Message published :lux=" + lumVal)
    client.publish(FIWARE_APIKEY+"/myEdison/lux", payload=lumVal )


def read_button():
    #Read Button and save its value in the dictionary using the alias "p"
    pulseVal = str(pulse.read()>100).lower()
    oldpulseVal = measures["p"]
    if (pulseVal != oldpulseVal): # Need to send
        measures["p"] = pulseVal
        print("New button state published :" + pulseVal)
        client.publish(FIWARE_APIKEY+"/myEdison/lux", payload=pulseVal )


# The callback called when the client receives a CONNACK response from the server.
def on_connect(client, userdata, rc):
    print( "Connected with result code " + str( rc ) )
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe( FIWARE_APIKEY+"/myEdison/cmd/SET" )


# The callback called when a message is received from the server.
def on_message(client, userdata, msg):
    payload = str( msg.payload )
    cmd, state = payload.split('#')
    cmdid_name, cmdid_id  = cmd.split('|')
    state_cmd, state_wish = state.split('|')
    print("cmdid = {0}, cmd = {1}, status = {2}".format(cmdid_id, state_cmd, state_wish))
    if state_wish == 'on':
        led.write(1)
    else:
        led.write(0)
    ack_payload = "cmdid|{0}#result|{1}".format(cmdid_id, state_wish)
    # Need to send ack to this topic "<api-key>/<device-id>/cmdexe/<cmd-name>"
    client.publish(FIWARE_APIKEY+"/myEdison/cmdexe/SET", ack_payload)



def on_publish(client, userdata, mid):
    print("Message id is "+str(mid))


def main():
    ## MAIN ##
    client.on_connect = on_connect
    client.on_message = on_message
    client.on_publish = on_publish
    client.username_pw_set(FIWARE_APIKEY)
    client.connect( FIWARE_SERVER, FIWARE_PORT, FIWARE_TIMEOUT )

    lux_manager = RepeatedTimer(MEASURES_PERIOD, read_lux)
    button_manager = RepeatedTimer(CHECK_BUTTON_PERIOD, read_button) #Test state each 0.1s
    lux_manager.start()
    button_manager.start()
    client.loop_forever() # to receive cmd from server



if __name__ == '__main__':
    main()


