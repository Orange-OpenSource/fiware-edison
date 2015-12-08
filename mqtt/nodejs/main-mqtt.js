"use strict";

// Based on main.js Developed by @IndustrialIoT Team (mail:iot_support@tid.es)
// MQTT  by Orange Team

// Libraries 
var mraa = require('mraa'); //require mraa
var request = require("request");
var sleep = require('sleep');
var mqtt = require ('mqtt');

// Fiware variables 
var TOKEN = 'xxxxxxx';
var FIWARE_DEVICE = 'myEdison-mqtt';
var FIWARE_SERVER = 'hackathon.villatolosa.com';

var PULSE_MEASURE_PERIOD = 1000; // time between pulse measures 
var LUMINOSITY_MEASURE_PERIOD = 5000; // time between luminosity measures

// Sensor variables
var analogPin0 = new mraa.Aio(0); //setup access to analog input #0 (A0) connected to Luminosity Sensor
var analogPin1 = new mraa.Aio(1); //setup access to analog input #1 (A1) connected to Button Sensor

// Commands configuration 
var led = new mraa.Gpio(6);
led.dir(mraa.DIR_OUT); 

var client = mqtt.connect('mqtt://' + FIWARE_SERVER, {username: TOKEN});

var oldPulse = null; // By default

client.on('connect', function() { // When connected
	subscribeToCommandTopic();
	readAndPublishLuminisoityMeasure();
	readAndPublishButtonSensorState();
});

client.on('message', function(topic, message, packet) {
	console.log("Received '" + message + "' on '" + topic + "'");
	if (topic === TOKEN + "/" + FIWARE_DEVICE + "/cmd/SET") {	 
		var dict = deserializeUL(message.toString());
		var cmdID = dict["cmdid"];
		var value = dict[FIWARE_DEVICE + "@SET"];
		led.write(value == "on" ? 1 : 0);
		sendCommandAck("SET", cmdID, value);
	}
});


// Read and publish the luminosity sensor measure
function readAndPublishLuminisoityMeasure(){
    var lum = analogPin0.read();
	
    console.log("Lum value : " + lum + "\n");
	// Verify that the publisher has been successful
	client.publish(TOKEN + "/" + FIWARE_DEVICE + "/lux", "" + lum, function(err, granted) {
		if (err != null) {
			console.log("publish luminosity : error", err);
		}
	}); 
	// loop
	setTimeout(readAndPublishLuminisoityMeasure, LUMINOSITY_MEASURE_PERIOD);
}


// Read and publish the button sensor state, if is value has changed 
function readAndPublishButtonSensorState() {
	var touch = analogPin1.read();
    var pulse = touch>100;
	
	// if the value changed, we published it
	if (pulse !== oldPulse) {
		console.log("Pulse value : " + pulse + "\n");
		// Verify that the publisher has been successful 
		client.publish(TOKEN + "/" + FIWARE_DEVICE + "/button" , pulse ? "true" : "false", function(err, granted) {
			if (err != null) {
				console.log("publish button : error", err);
			}
		}); 
		oldPulse = pulse;
	}
	// loop
	setTimeout(readAndPublishButtonSensorState, PULSE_MEASURE_PERIOD);
}


function subscribeToCommandTopic() {
	// Subscribe to the command topic
	client.subscribe(TOKEN + "/" + FIWARE_DEVICE + "/cmd/SET", function(err, granted) {
		if (err != null) {
			console.log("subscribe to command topic : error", err);
		}
	});
}

function sendCommandAck(cmd, cmdID, result) {
	// publish 
	var payload = serializeUL({"cmdid" : cmdID, "result" : result});
	console.log("payload : " + payload);
	client.publish(TOKEN + "/" + FIWARE_DEVICE + "/cmdexe/" + cmd, payload, function(err, granted) {
			if (err != null) {
				console.log("publish button : error", err);
			}
		}); 
}

function deserializeUL(payload) {
	var pairs = payload.split("#");
	var dict = {};     
	for (var i in pairs) {
		var pair = pairs[i];
		if (pair != "") {
			var keyValue = pair.split("|");
			dict[keyValue[0]] = keyValue[1];
		}	
	}
	return dict;
}

function serializeUL(dict) {
	var payload = "";
	
	for (var key in dict) {
		if (payload !== "") {
			payload += "#";
		}
		payload += key + "|" + dict[key];
	}
	return payload;
}