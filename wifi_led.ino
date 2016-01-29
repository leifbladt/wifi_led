#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "SSID of your WiFi";
const char* password = "Password of your WiFi";
const char* mqtt_server = "hostname";

int ledRedPin = 13;
int ledGreenPin = 12;
int ledBluePin = 14;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

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
  int pin = 0;
  if (channel.endsWith("red")) {
    pin = ledRedPin;
  } else if (channel.endsWith("green")) {
    pin = ledGreenPin;
  } else if (channel.endsWith("blue")) {
    pin = ledBluePin;
  } else {
    pin = 0;
  }

  if (pin != 0) {
    int value = int(p.toInt() * 2.55 * 4);
    Serial.print("value: ");
    Serial.println(value);
    analogWrite(pin, value);
  }
}
  

void setup() {
  pinMode(ledRedPin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
  pinMode(ledBluePin, OUTPUT);
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

  client.loop();
}
