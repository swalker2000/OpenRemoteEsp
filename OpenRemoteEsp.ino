#include <WiFiClientSecure.h>
#include <MQTT.h>

const char ssid[] = "";
const char pass[] = "";
const char* mqtt_server = "";
const char *subscribeTopic = "";
const char *publicTopic = "";
const char* username = "master:mqttuser"; 
const char* mqttpass = ""; 
const char* clientID = "client123";

WiFiClientSecure net;
MQTTClient client;

unsigned long lastMillis = 0;

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.print("\nconnecting...");
  net.setInsecure();
  while (!client.connect(clientID, username, mqttpass)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nconnected!");
  client.subscribe(subscribeTopic);
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  client.begin(mqtt_server, 8883, net);
  client.onMessage(messageReceived);
  pinMode(25, INPUT);
  connect();
}

void loop() {
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    int value = digitalRead(25);
    String valueStr = String(value);
    Serial.print("Value : "); Serial.println(value);
    client.publish(publicTopic, valueStr.c_str());
  }
}