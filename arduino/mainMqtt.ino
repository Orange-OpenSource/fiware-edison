// Libraries
#include <SPI.h> 
#include <WiFi.h> 
#include <PubSubClient.h>

#define UNKNOWN_STATE - 1
#define FIWARE_APIKEY "xxxxxxxxxxxx"
#define FIWARE_DEVICE "myEdison"
#define FIWARE_SERVER "hackathon.villatolosa.com"
#define FIWARE_PORT  1883

// Wifi details
char ssid[] = "xxxxxx"; // your network SSID (name)
char pass[] = "xxxxxxxxx"; // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0; // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
WiFiClient wifiClient;
PubSubClient client(wifiClient);

char TOPIC_LUM[] = FIWARE_APIKEY "/myEdison/lux";
char TOPIC_BUTTON[] = FIWARE_APIKEY "/myEdison/button";
char TOPIC_SUBSCRIBE[] = FIWARE_APIKEY "/myEdison/cmd/SET";

int oldPulse = UNKNOWN_STATE; // By default
int nbLoop = 0;

char message[100];

void loop() {
  connectToMqttAndSubscribeToSetCommand();
  // connected
  readAndPublishButtonSensorState();
  if (nbLoop % 5 == 0) {
    readAndPublishLuminosityMeasure();
    nbLoop = 0; // reset nbLoop
  }
  nbLoop++;
  Serial.print("nbLoop value : ");
  Serial.println(nbLoop);
  client.loop();
}

// Read and publish the luminosity sensor measure
void readAndPublishLuminosityMeasure() {
  int lum = analogRead(A0); // Connect Light Sensor on Analogic PIN A0
  char lumValue[50];
  String(lum).toCharArray(lumValue, 50);

  char printLum[100];
  sprintf(printLum, "lum value : %s", lumValue);
  Serial.println(printLum);

  // Verify that the publisher has been successful
  if (!client.publish(TOPIC_LUM, lumValue)) {
    Serial.println("publish luminosity failed");
  } else {
    Serial.println("publish luminosity success");
  }
}

// Read and publish the button sensor state, if it value has changed
void readAndPublishButtonSensorState() {
  int touch = analogRead(A1); // Reading Light and Button Sensors contained in Grove Starter Kit for Arduino
  bool pulse = (touch > 100) ? 1 : 0;
  if (pulse != oldPulse) {
    char printButtonState[100];
    sprintf(printButtonState, "button : %s", pulse ? "true" : "false");
    Serial.println(printButtonState);

    // Verify that the publisher has been successful
    if (!client.publish(TOPIC_BUTTON, pulse ? "true" : "false")) {
      Serial.println("publish button sensor failed");
    } else {
      Serial.println("publish button sensor success");
    }
    oldPulse = pulse;
  }
}


void subscribeToCommandTopic() {
  if (!client.subscribe(TOPIC_SUBSCRIBE)) {
    Serial.println("subscribe to set command failed");
  } else {
    Serial.println("subscribe to set command success");
  }
}

void callback(char * topic, byte * payload, unsigned int length) {
  memset(message, 0, 100); // clean

  if (strcmp(topic, TOPIC_SUBSCRIBE) == 0) {
    byteToChar(payload, message, length);

    char * tokenCmdid = strtok(message, "#");
    char * tokenCmd = strtok(NULL, "#");
    char * cmdid = strtok(tokenCmdid, "|");
    char * cmdidValue = strtok(NULL, "|");
    char * cmd = strtok(tokenCmd, "|");
    char * cmdValue = strtok(NULL, "|");

    setLedStatus(cmdValue);
    sendCommandAck("SET", cmdidValue, cmdValue);
  }
}


void setLedStatus(char * value) {
  // Set the led status
  if (strcmp(value, "on") == 0) {
    digitalWrite(6, HIGH);
  } else if (strcmp(value, "off") == 0) {
    digitalWrite(6, LOW);
  }
}


void sendCommandAck(char * cmd, char * cmdidValue, char * result) {
  char topic[400];
  memset(topic, 0, 400); // clean
  strcat(topic, FIWARE_APIKEY);
  strcat(topic, "/");
  strcat(topic, FIWARE_DEVICE);
  strcat(topic, "/cmdexe/");
  strcat(topic, cmd);
  Serial.print("topic : ");
  Serial.println(topic);

  char payload[400];
  memset(payload, 0, 400); // clean
  strcat(payload, "cmdid|");
  strcat(payload, cmdidValue);
  strcat(payload, "#result|");
  strcat(payload, result);
  Serial.print("payload : ");
  Serial.println(payload);

  if (!client.publish(topic, payload)) {
    Serial.println("publish ack failed");
  } else {
    Serial.println("publish ack success");
  }
}


void setup() {
  Serial.begin(115200);
  pinMode(6, OUTPUT);
  Serial.println("setup...");

  while (!Serial) {; // wait for serial port to connect. Needed for Leonardo only
  }
  setupNetwork();
  client.setServer(FIWARE_SERVER, FIWARE_PORT);
  client.setCallback(callback);
}

void setupNetwork() {
  Serial.println("Configuring Network...");

  // Check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv != "1.1.0") Serial.println("Please upgrade the firmware");

  // Attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.println("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid, pass);

    // Wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
}

void connectToMqttAndSubscribeToSetCommand() {
  // Loop until we're reconnected to mqtt
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("myEdison-arduino", FIWARE_APIKEY, NULL)) {
      Serial.println("connected");
      // Subscribe during the first connection and each reconnection 
      subscribeToCommandTopic();
    } else {
      Serial.println("failed");
      Serial.println(client.state());
      Serial.println("try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// Convert a byte array to a char array
void byteToChar(byte * bytes, char * chars, unsigned int count) {
  for (unsigned int i = 0; i < count; i++) {
    chars[i] = (char) bytes[i];
  }
  chars[count + 1] = '\0';
}