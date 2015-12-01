#!/usr/bin/env python
import paho.mqtt.client as mqtt
from config import *

# The callback called when the client receives a CONNACK response from the server.
def on_connect(client, userdata, rc):
    print( "Connected with result code " + str( rc ) )
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    #client.subscribe( "$SYS#" )
    client.subscribe( FIWARE_APIKEY+"/myEdison/cmd/SET" )


# The callback called when a message is received from the server.
def on_message(client, userdata, msg):
    payload = str( msg.payload )
    cmd, state = payload.split('#')
    cmdid_name, cmdid_id  = cmd.split('|')
    state_cmd, state_wish = state.split('|')
    print("cmdid = {0}, cmd = {1}, status = {2}".format(cmdid_id, state_cmd, state_wish))
    if state_wish == 'on':
        print("led.write(1)")
    else:
        print("led.write(0)")
    ack_payload = "cmdid|{0}#result|{1}".format(cmdid_id, state_wish)
    # Need to send ack to this topic "<api-key>/<device-id>/cmdexe/<cmd-name>"
    client.publish(FIWARE_APIKEY+"/myEdison/cmdexe/SET", ack_payload)


client = mqtt.Client(  )
client.on_connect = on_connect
client.on_message = on_message
client.username_pw_set(FIWARE_APIKEY)

client.connect( FIWARE_SERVER, FIWARE_PORT, FIWARE_TIMEOUT )

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
client.loop_forever( )
