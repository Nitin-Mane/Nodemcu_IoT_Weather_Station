
#include <Adafruit_Sensor.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>



//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>

char ssid[] = "Your Wifi Name";
char wifiPassword[] = "Enter Wifi Password";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "Paste the User ID Key";
char password[] = "Paste the User Password";
char clientID[] = "Paste the token key";

unsigned long lastMillis = 0;
#include "DHT.h"

#define DHTPIN D4
#define BUZZER_PIN  D5    

#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);

float h,t,f,hic,hif;
double dp; 
int l, rin; 

int sensorPin = A0;    // input for LDR and rain sensor
int enable1 = 15;      // enable reading LDR
int enable2 = 13;      // enable reading Rain sensor


int sensorValue1 = 0;  // variable to store the value coming from sensor LDR
int sensorValue2 = 0;  // variable to store the value coming from sensor Rain sensor

void setup() {
	Serial.begin(9600);
	Cayenne.begin(username, password, clientID, ssid, wifiPassword);
  pinMode(D0,OUTPUT);
  pinMode(D1,OUTPUT);
  pinMode(D2,OUTPUT);
  pinMode(D3,OUTPUT);
  pinMode(enable1, OUTPUT);
  pinMode(enable2, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
 
Serial.begin(115200);
delay(10);
}

void loop() {
	Cayenne.loop();

	
	if (millis() - lastMillis > 10000) {
		lastMillis = millis();
  Cayenne.virtualWrite(0, h);
  Cayenne.virtualWrite(1, t);
  Cayenne.virtualWrite(2, hic);
  Cayenne.virtualWrite(3, sensorValue1);
  Cayenne.virtualWrite(4, sensorValue2);
	}
   h = dht.readHumidity();
  
   t = dht.readTemperature();
  
  
   f = dht.readTemperature(true);

  
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  hif = dht.computeHeatIndex(f, h);
  
  hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
 
  
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");

//--- extra---- you can measure dew point with the temperature and the humidity
  
  double gamma = log(h/100) + ((17.62*t) / (243.5+t));
  double dp = 243.5*gamma / (17.62-gamma);

Serial.print("Dew point:        ");
Serial.print(dp);
Serial.print(" degrees Celcius ");
Serial.println();
	


//--------------------------LDR-------------------------

  digitalWrite(enable1, HIGH); 
sensorValue1 = analogRead(sensorPin);
sensorValue1 = constrain(sensorValue1, 300, 850); 
sensorValue1 = map(sensorValue1, 300, 850, 0, 1023); 
Serial.print("Light intensity:  ");
Serial.println(sensorValue1);
digitalWrite(enable1, LOW);
delay(100);

//--------------------------Rain Sensor-------------------------

digitalWrite(enable2, HIGH); 

delay(500);
sensorValue2 = analogRead(sensorPin);
sensorValue2 = constrain(sensorValue2, 150, 440); 
sensorValue2 = map(sensorValue2, 150, 440, 1023, 0); 
 if ( sensorValue2 > 800 ) 
  { 
    digitalWrite(BUZZER_PIN, HIGH);
    delay (50); 
    digitalWrite(BUZZER_PIN, LOW);
    delay (50); 
    digitalWrite(BUZZER_PIN, HIGH);
    delay (50); 
    digitalWrite(BUZZER_PIN, LOW);
  }
  else 
  { 
    digitalWrite(BUZZER_PIN, LOW);
  }

Serial.print("Rain value:       ");
Serial.println(sensorValue2);
Serial.println();
delay(100);

digitalWrite(enable2, LOW);

}

CAYENNE_IN(5)
  {
    
    int currentValue=getValue.asInt();
    if(currentValue==1){
      digitalWrite(D0,LOW);
      }
      else{
       digitalWrite(D0,HIGH); 
      }       }
CAYENNE_IN(6)
  {
    
    int currentValue=getValue.asInt();
    if(currentValue==1){
      digitalWrite(D1,LOW);
      }
      else{
       digitalWrite(D1,HIGH); 
      }       }
CAYENNE_IN(7)
  {
    
    int currentValue=getValue.asInt();
    if(currentValue==1){
      digitalWrite(D2,LOW);
      }
      else{
       digitalWrite(D2,HIGH); 
      }       
   }
 CAYENNE_IN(8)
  {
    
    int currentValue=getValue.asInt();
    if(currentValue==1){
      digitalWrite(D3,LOW);
      }
      else{
       digitalWrite(D3,HIGH); 
      }      
      
   }

//Default function for processing actuator commands from the Cayenne Dashboard.
//You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.

