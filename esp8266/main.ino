#include <SPI.h>
#include <MFRC522.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <PubSubClient.h>

 
#define SS_PIN 4  // sda
#define RST_PIN 2

const char *mqttServer = "aryansdomain.com"; // mqtt server host
const int mqttPort = 1883;                   // mqtt server port

const char *publishTopic = "attendance/swipe"; // topic to publish rfid to when user swipes
const char *entryTopic = "attendance/entry";   // topic where entries are published
const char *exitTopic = "attendance/exit";     // topic where exits are published


WiFiClient espClient;
PubSubClient client(espClient);
 
int lock = D1;    
 
MFRC522 mfrc522(RST_PIN, SS_PIN);        // Create MFRC522 instance.
char auth[] = "QdZ_SNFXINdRC6eEjpEfqzrhZ8REm2hm";
char ssid[] = "mask_detection";
char pass[] = "12345678";    // Enter WiFi password
SimpleTimer timer;
int fflag = 0; 
int eflag = 0; 
int jflag = 0; 
WidgetTerminal terminal(V2);
 
 
void iot_rfid()
{
 
        // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
        MFRC522::MIFARE_Key key;
        for (byte i = 0; i < 6; i++) {
                key.keyByte[i] = 0xFF;
        }
        // Look for new cards
        if ( ! mfrc522.PICC_IsNewCardPresent()) {
                return;
        }
 
        // Select one of the cards
        if ( ! mfrc522.PICC_ReadCardSerial()) {
                return;
        }
        // Now a card is selected. The UID and SAK is in mfrc522.uid.
        
        // Dump UID
        Serial.print("Card UID:");
        String uid = "";
        for (byte i = 0; i < mfrc522.uid.size; i++) {
//               Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
            uid += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
//            Serial.print(mfrc522.uid.uidByte[i], DEC);
               uid += String(mfrc522.uid.uidByte[i], DEC);
        } 
        Serial.println(uid);
        int uidlen = uid.length() + 1;
        char uidBuffer[uidlen];
        uid.toCharArray(uidBuffer, uidlen);
        client.publish(publishTopic, uidBuffer);
 
        // Dump PICC type
        byte piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    //    Serial.print("PICC type: ");
//Serial.println(mfrc522.PICC_GetTypeName(piccType));
        if (        piccType != MFRC522::PICC_TYPE_MIFARE_MINI 
                &&        piccType != MFRC522::PICC_TYPE_MIFARE_1K
                &&        piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
                //Serial.println("This sample only works with MIFARE Classic cards.");
                return;
        }
        
        // Enter RFID Tag ID here 
        if( ((mfrc522.uid.uidByte[0] == 11) && (mfrc522.uid.uidByte[1] == 22) && (mfrc522.uid.uidByte[2] == 33) && (mfrc522.uid.uidByte[3] == 44)) && (fflag == 1) )
        {
         Serial.println("User01");   //Enter User1 Name
         Blynk.virtualWrite(V2, "User01" );   //Enter User1 Name
         digitalWrite(lock, HIGH); 
         delay(3000); 
         digitalWrite(lock, LOW); 
        }
  
        else
        if(( (mfrc522.uid.uidByte[0] == 11) && (mfrc522.uid.uidByte[1] == 12) && (mfrc522.uid.uidByte[2] == 13) && (mfrc522.uid.uidByte[3] == 14))&& (eflag == 1) )
        {
         Serial.println("User02");   //Enter User2 Name 
         Blynk.virtualWrite(V2, "User02" );   //Enter User2 Name
         digitalWrite(lock, HIGH); 
         delay(3000); 
         digitalWrite(lock, LOW); 
        }
        else
        if( ((mfrc522.uid.uidByte[0] == 21) && (mfrc522.uid.uidByte[1] == 22) && (mfrc522.uid.uidByte[2] == 23) && (mfrc522.uid.uidByte[3] == 24))&& (jflag == 1) )
        {
         Serial.println("User03");   //Enter User3 Name
         Blynk.virtualWrite(V2, "User03" );   //Enter User3 Name
         digitalWrite(lock, HIGH); 
         delay(3000); 
         digitalWrite(lock, LOW); 
        }
 
        else 
        Serial.println("Unregistered User");
  
}
 
// in Blynk app writes values to the Virtual Pin 3
BLYNK_WRITE(V3)
{
   fflag = param.asInt(); // assigning incoming value from pin V3 to a variable
  // Blynk.virtualWrite(V2, fflag );
 
}
 
 
// in Blynk app writes values to the Virtual Pin 4
BLYNK_WRITE(V4)
{
   eflag = param.asInt(); // assigning incoming value from pin V4 to a variable
 
}
 
BLYNK_WRITE(V5)
{
   jflag = param.asInt(); // assigning incoming value from pin V5 to a variable
 
}

void callback(char *topic, byte *payload, unsigned int length)
{
    String message = "";
    for (int i = 0; i < length; i++)
    {
        message += String((char)payload[i]);
    }
    Serial.println(message);
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


void setup() {
        Serial.begin(9600);        // Initialize serial communications with the PC
        Blynk.begin(auth, ssid, pass);
      pinMode(lock,OUTPUT);
      digitalWrite(lock, LOW);
        SPI.begin();                // Init SPI bus
        mfrc522.PCD_Init();        // Init MFRC522 card
        //Serial.println("Scan a MIFARE Classic PICC to demonstrate Value Blocks.");
          timer.setInterval(1000L, iot_rfid);

          
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
}}
 
void loop() {
        
  timer.run(); // Initiates SimpleTimer
      client.loop();

  Blynk.run();
 
       
}
