#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <PubSubClient.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "Button.h"
#include "LED.h"
#include "LEDStripe.h"
#include "Configuration.h"

WiFiClient espClient;
PubSubClient client(espClient);
Button button(BUTTON_PIN);
LEDStripe stripe(RED_PIN, GREEN_PIN, BLUE_PIN);
LED statusLED(LED_PIN);

void setup_wifi() {

  delay(100);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.begin(SSID, WIFI_KEY);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

char* subStr (char* str, char* delim, int index) {
  char *act, *sub, *ptr;
  static char copy[15];
  int i;

  // Since strtok consumes the first arg, make a copy
  strcpy(copy, str);

  for (i = 0, act = copy; i <= index; i++, act = NULL) {
    sub = strtok_r(act, delim, &ptr);
    if (sub == NULL) {
      break;
    }
  }
  return sub;
}

void callback(char* topic, byte* payload, unsigned int length) {

  // Power off
  // 0
  // Power on
  // 1
  // Power on with color values
  // 1;15;224;0
  char delimiter[2] = ";";
  char message[length];
  for (int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }

  // TODO message contains garbage at the end
  Serial.print("Message arrived [");
  Serial.print(message);
  Serial.print(", length: ");
  Serial.print(length);
  Serial.println("] ");

  if (length == 0) {
    return;
  }

  if (message[0] == '0') {
    stripe.powerOff();
    client.publish(TOPIC_STATE, "0");
  } else if (message[0] == '1') {
    if (length == 1) {
      stripe.powerOn();
      client.publish(TOPIC_STATE, "1");
    } else {
      int red = atoi(subStr(message, delimiter, 1));
      int green = atoi(subStr(message, delimiter, 2));
      int blue = atoi(subStr(message, delimiter, 3));
      stripe.setColor(red * 4, green * 4, blue * 4);
      char state[length];
      sprintf(state, "1;%d;%d;%d", red, green, blue);
      client.publish(TOPIC_STATE, state);
    }
  }
}

void setup() {
  pinMode(BUTTON_PIN, INPUT);

  statusLED.powerOff();

  Serial.begin(115200);
  setup_wifi();
  client.setServer(MQTT_SERVER, 1883);
  client.setCallback(callback);

  // TODO Set initial state

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("End");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\n", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
}

void reconnect() {
  // TODO Add last will

  // TODO Make it without delay
  // Loop until we're reconnected
  statusLED.blink(2);

  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(TOPIC_STATE, "0");
      // ... and resubscribe
      client.subscribe(TOPIC_COMMAND);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 3 seconds");
      // Wait 3 seconds before retrying
      delay(3000);
    }
  }

  statusLED.powerOn();
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }

  if (button.released()) {
    // TODO Extract into button callback method
    stripe.togglePower();
    // TODO Send complete RGB value when switching on
    client.publish(TOPIC_STATE, stripe.getPower() ? "1" : "0");
  }

  client.loop();

  statusLED.loop();

  ArduinoOTA.handle();
}
