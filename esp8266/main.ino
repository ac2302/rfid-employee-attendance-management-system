#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char *ssid = "wifi name";              // Enter your WiFi name
const char *password = "wifi password";      // Enter WiFi password
const char *mqttServer = "aryansdomain.com"; // mqtt server host
const int mqttPort = 1883;                   // mqtt server port

const char *publishTopic = "attendance/swipe"; // topic to publish rfid to when user swipes
const char *entryTopic = "attendance/entry";   // topic where entries are published
const char *exitTopic = "attendance/exit";     // topic where exits are published

WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{

    Serial.begin(115200);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to the WiFi network");

    client.setServer(mqttServer, mqttPort);
    //client.setServer(server, mqttPort);
    client.setCallback(callback);

    while (!client.connected())
    {
        Serial.println("Connecting to MQTT...");

        if (client.connect("ESP8266Client"))
        {

            Serial.println("connected");
        }
        else
        {

            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }

    client.subscribe(entryTopic);
    client.subscribe(exitTopic);

    // emulating user with rfid 'abcd' swiping
    client.publish(publishTopic, "abcd");
}

void callback(char *topic, byte *payload, unsigned int length)
{
    String message = "";
    for (int i = 0; i < length; i++)
    {
        message += String((char)payload[i]);
    }

    if (topic == entryTopic)
    {
        Serial.print(message);
        Serial.println(" has entered");
    }
    else if (topic == exitTopic)
    {
        Serial.print(message);
        Serial.println(" has exited");
    }
}

void loop()
{
    client.loop();
    delay(1000);
}