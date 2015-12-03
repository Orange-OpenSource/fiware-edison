#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "MQTTClient.h"
#include "mraa.h"
#include "mraa/aio.h"

#define FIWARE_SERVER "hackathon.villatolosa.com"
#define FIWARE_APIKEY "xxxxxxxxxxx"
#define FIWARE_DEVICE "myEdison"
#define CLIENTID "xxxxxxx"
#define PAYLOAD "112"
#define QOS 0
#define TIMEOUT 10000L
#define UNKNOWN_STATE - 1

void readAndPublishLuminosityMeasure();
void readAndPublishButtonSensorState();
int msgarrvd(void * context, char * topicName, int topicLen, MQTTClient_message * message);
void setLedStatus(char * value);
void sendCommandAck(char * cmd, char * cmdidValue, char * result);
void delivered(void * context, MQTTClient_deliveryToken dt);
void connlost(void * context, char * cause);

char TOPIC_LUM[] = FIWARE_APIKEY "/myEdison/lux";
char TOPIC_BUTTON[] = FIWARE_APIKEY "/myEdison/button";
char TOPIC_SUBSCRIBE[] = FIWARE_APIKEY "/myEdison/cmd/SET";

MQTTClient client;
MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
MQTTClient_message pubmsg = MQTTClient_message_initializer;
MQTTClient_deliveryToken token;
int rc;

mraa_gpio_context gpio;
int oldPulse = UNKNOWN_STATE;

int main(int argc, char * argv[]) 
{
	gpio = mraa_gpio_init(6);
	int i = 0;

	MQTTClient_create( & client, FIWARE_SERVER, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;
	conn_opts.username = FIWARE_APIKEY;

	pubmsg.qos = QOS;
	pubmsg.retained = 0;

	MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);

	if ((rc = MQTTClient_connect(client, & conn_opts)) != MQTTCLIENT_SUCCESS)
	{
		printf("Failed to connect, return code %d\n", rc);
		exit(-1);
	}
	if (MQTTClient_subscribe(client, TOPIC_SUBSCRIBE, QOS) != MQTTCLIENT_SUCCESS) 
	{
		printf("Failed to subscribe, return code %d\n", rc);
		exit(-1);
	}
	
	for (;;) 
	{
		readAndPublishButtonSensorState();
		if (i == 5) 
		{
			readAndPublishLuminosityMeasure();
			i = 0;
		}
		i++;
		sleep(1);
	}

	MQTTClient_disconnect(client, 10000);
	MQTTClient_destroy( & client);
	return rc;
}

void readAndPublishLuminosityMeasure() 
{
	// read the luminosity measure
	mraa_aio_context adc_a0;
	uint16_t adc_value = 0;
	adc_a0 = mraa_aio_init(0);

	if (adc_a0 == NULL) 
	{
		printf("adc_a0 is NULL");
	}
	adc_value = mraa_aio_read(adc_a0);
	fprintf(stdout, "Luminosity value : %d\n", adc_value);
	mraa_aio_close(adc_a0);

	char lum[50];
	snprintf(lum, 50, "%d", adc_value);
	printf("lum : %s\n", lum);

	// publish luminosity 
	pubmsg.payload = lum;
	pubmsg.payloadlen = strlen(lum);
	MQTTClient_publishMessage(client, TOPIC_LUM, & pubmsg, & token);
	rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
}

void readAndPublishButtonSensorState() 
{
	// read the button sensor state
	mraa_aio_context adc_a1;
	uint16_t adc_value = 0;
	adc_a1 = mraa_aio_init(1);

	if (adc_a1 == NULL) 
	{
		printf("adc_a1 is NULL");
	}

	adc_value = mraa_aio_read(adc_a1);
	mraa_aio_close(adc_a1);

	int pulse = (adc_value > 100) ? 1 : 0;

	if (pulse != oldPulse) 
	{
		fprintf(stdout, "button : %s\n", pulse ? "true" : "false");

		// publish the button sensor state
		char * buttonState = pulse ? "true" : "false";
		pubmsg.payload = buttonState;
		pubmsg.payloadlen = strlen(buttonState);
		MQTTClient_publishMessage(client, TOPIC_BUTTON, & pubmsg, & token);
		rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);

		oldPulse = pulse;
	}
}

int msgarrvd(void * context, char * topicName, int topicLen, MQTTClient_message * message) 
{
	char buffer[100];
	printf("\n------------------\n\n");
	int i;
	char * payloadptr;

	payloadptr = message -> payload;
	strncpy(buffer, payloadptr, message -> payloadlen);
	buffer[message -> payloadlen] = '\0';
	printf("le message : %s\n", buffer);

	if (strcmp(topicName, TOPIC_SUBSCRIBE) == 0) 
	{
		char * tokenCmdid = strtok(buffer, "#");
		char * tokenCmd = strtok(NULL, "#");
		char * cmdid = strtok(tokenCmdid, "|");
		char * cmdidValue = strtok(NULL, "|");
		char * cmd = strtok(tokenCmd, "|");
		char * cmdValue = strtok(NULL, "|");

		setLedStatus(cmdValue);
		sendCommandAck("SET", cmdidValue, cmdValue);
	}

	MQTTClient_freeMessage( & message);
	MQTTClient_free(topicName);

	return 1;
}

void sendCommandAck(char * cmd, char * cmdidValue, char * result) 
{
	char topic[400];
	memset(topic, 0, 400); // clean
	strcat(topic, FIWARE_APIKEY);
	strcat(topic, "/");
	strcat(topic, FIWARE_DEVICE);
	strcat(topic, "/cmdexe/");
	strcat(topic, cmd);

	char payload[400];
	memset(payload, 0, 400); // clean
	strcat(payload, "cmdid|");
	strcat(payload, cmdidValue);
	strcat(payload, "#result|");
	strcat(payload, result);

	// publish ack
	pubmsg.payload = payload;
	pubmsg.payloadlen = strlen(payload);
	MQTTClient_publishMessage(client, topic, & pubmsg, & token);
	rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
}

void setLedStatus(char * value) 
{
	if (strcmp(value, "on") == 0) 
	{
		mraa_gpio_write(gpio, 1);
	} 
	else if (strcmp(value, "off") == 0) 
	{
		mraa_gpio_write(gpio, 0);
	}
}

void delivered(void * context, MQTTClient_deliveryToken dt) 
{
	printf("Message with token value %d delivery confirmed\n", dt);
}

void connlost(void * context, char * cause) 
{
	printf("\nConnection lost\n");
	printf("     cause: %s\n", cause);
}