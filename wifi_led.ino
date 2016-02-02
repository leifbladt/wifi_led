#include <ESP8266WiFi.h>
#include <PubSubClient.h>
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

void callback(char* topic, byte* payload, unsigned int length) {

  // Power off
  // 0
  // Power on
  // 1
  // Power on with color values
  // 1;15;224;0

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  if (length == 0) {
    return;
  }

  if ((char)payload[0] == '0') {
    stripe.powerOff();
  } else if (length == 1) {
    stripe.powerOn();
  } else {
    // TODO Extract values
    //  int value = int(p.toInt() * 2.55 * 4);
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
}

void reconnect() {
  // TODO Add last will

  // TODO Make it without delay
  // Loop until we're reconnected
  statusLED.powerOff();

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
    stripe.togglePower();
  }

  client.loop();
}
