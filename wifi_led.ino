#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "Button.h"

const char* ssid = "SSID of your WiFi";
const char* password = "Password of your WiFi";
const char* mqtt_server = "hostname";

#define RED_PIN 13
#define GREEN_PIN 12
#define BLUE_PIN 14
#define BUTTON_PIN 5

class LEDStripe {
  public:
    LEDStripe(const int redPin, const int greenPin, const int bluePin) {
      _redPin = redPin;
      _greenPin = greenPin;
      _bluePin = bluePin;

       setOutputMode();
    }

    void switchOn() {
      Serial.print("switch off");

      _redPin = 127;
      _greenPin = 127;
      _bluePin = 127;
      updateValues();
    }

    void switchOff() {
      Serial.print("switch off");

      _redPin = 0;
      _greenPin = 0;
      _bluePin = 0;
      updateValues();
    }

    void setRed(int value) {
      Serial.print("red value: ");
      Serial.println(value);
      
      _redValue = value;
      updateValues();
    }

    void setGreen(int value) {
      Serial.print("green value: ");
      Serial.println(value);
      
      _greenValue = value;
      updateValues();
    }

    void setBlue(int value) {
      Serial.print("blue value: ");
      Serial.println(value);
      
      _blueValue = value;
      updateValues();
    }

  private:
    int _redPin;
    int _greenPin;
    int _bluePin;

    int _redValue = 0;
    int _greenValue = 0;
    int _blueValue = 0;

    void updateValues() {
      analogWrite(_redPin, _redValue);
      analogWrite(_greenPin, _greenValue);
      analogWrite(_bluePin, _blueValue);
    }

    void setOutputMode() {
      pinMode(_redPin, OUTPUT);
      pinMode(_greenPin, OUTPUT);
      pinMode(_bluePin, OUTPUT);
    }
};

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
Button button(BUTTON_PIN);
LEDStripe stripe(RED_PIN, GREEN_PIN, BLUE_PIN);

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

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
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String p = "";
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    p += (char)payload[i];
  }
  Serial.println();

  String channel = String(topic);
  int value = int(p.toInt() * 2.55 * 4);
  if (channel.endsWith("red")) {
    stripe.setRed(value);
  } else if (channel.endsWith("green")) {
    stripe.setGreen(value);
  } else if (channel.endsWith("blue")) {
    stripe.setBlue(value);
  }
}


void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);

  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  client.publish("home/led/1/state", "OFF");
}

void reconnect() {
  // Loop until we're reconnected
  digitalWrite(BUILTIN_LED, HIGH);

  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("home/led/1/state", "0");
      // ... and resubscribe
      client.subscribe("home/led/1/+");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }

  digitalWrite(BUILTIN_LED, LOW);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }

  if (button.released()) {
    stripe.switchOff();
  }

  client.loop();
}
