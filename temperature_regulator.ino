 
#include <ESP8266WiFi.h>
#include <DHT.h>
#include "ThingSpeak.h"
char ssid[] = "Uniccon int";   
char pass[] ="uniceptic7700#";   
int keyIndex = 0;            
WiFiClient  client;
int relaypin=D2;
unsigned long myChannelNumber = 1801282;
const char * myWriteAPIKey = "4AK3S7CN2SVSNH4S";
#define DHTPIN D3
DHT dht(DHTPIN, DHT11);
// Initialize our values
int statusCode = 0;
int field[2] = {1,2};
String myStatus = "";
void setup() {
  // put your setup code here, to run once:
     Serial.begin(115200);  // Initialize serial
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  
  WiFi.mode(WIFI_STA); 
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  dht.begin();
    pinMode (relaypin, OUTPUT);
   }

void loop() {
  // put your main code here, to run repeatedly:
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

  // set the fields with the values
  //float h=ford.move();
      float h = dht.readHumidity();
      float t = dht.readTemperature();
  ThingSpeak.setField(1, t);
  ThingSpeak.setField(2, h);
  // figure out the status message
 if (t<=22){
  Serial.println("cold");
  digitalWrite(relaypin,HIGH);
 }
if (22<t<24){
   Serial.println("normal Temp");
   digitalWrite(relaypin,HIGH);
  
 }
if (t>=24){
 
   Serial.println("hot");
   digitalWrite(relaypin,HIGH);
 
}
  // set the status
  ThingSpeak.setStatus(myStatus);
  
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  // delay(20000); // Wait 20 seconds to update the channel again
  }
