#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//---------------------------------------------------------
const char *ssid = "deep-dark-in";
const char *password = "665544332211";
const char *mqtt_broker = "119.91.206.97";
const char *mqtt_username = "ESP8266D1001";
const char *mqtt_password = "ESP8266D1001";
const char *mqtt_clientid = "ESP8266D1001";
const char *topic = "things/device/ESP8266D1001";
const int mqtt_port = 1883;
//---------------------------------------------------------
WiFiClient espClient;
PubSubClient client(espClient);
//---------------------------------------------------------
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (unsigned int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}
void connectWIFI()
{
  WiFi.setHostname("ESP8266D1001");
  WiFi.setAutoReconnect(true);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
}
void connectMqtt()
{
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected())
  {
    if (client.connect(mqtt_clientid, mqtt_username, mqtt_password))
    {
      Serial.println("mqtt broker connected");
    }
    else
    {
      Serial.print("mqtt broker disconnected ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void setup()
{
  Serial.begin(9600);
  ESP.wdtEnable(5000);
  connectWIFI();
  connectMqtt();
}
void loop()
{
  ESP.wdtFeed();
  // if (WiFi.status() != WL_CONNECTED)
  // {
  //   connectWIFI();
  // }
  if (!client.connected())
  {
    connectMqtt();
  }
  client.loop();
}