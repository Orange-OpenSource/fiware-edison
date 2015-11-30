#!/usr/bin/env python
import paho.mqtt.client as mqtt
################ FIWARE VARIABLES ################
FIWARE_SERVER = "hackathon.villatolosa.com"
FIWARE_PORT = 1883
FIWARE_APIKEY = "09XXYXANSMAHZLVCPZVJ"
FIWARE_DEVICE = "myEdison"

LOOP_TIME = 60

# The callback called when the client receives a CONNACK response from the server.
def on_connect(client, userdata, rc):
    print( "Connected with result code " + str( rc ) )
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    #client.subscribe( "$SYS#" )
    client.subscribe( FIWARE_APIKEY+"/myEdison/lux" )



# The callback called when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic + " " + str( msg.payload ))


client = mqtt.Client(  )
client.on_connect = on_connect
client.on_message = on_message
client.username_pw_set(FIWARE_APIKEY)

client.connect( FIWARE_SERVER, FIWARE_PORT, LOOP_TIME )

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
client.loop_forever( )
