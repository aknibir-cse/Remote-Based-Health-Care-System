#define BLYNK_PRINT Serial
#include <Blynk.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleESP8266.h>
#include <DHT.h>

#define DHTPIN 2          // D4
#define DHTTYPE DHT11     

DHT dht(DHTPIN, DHTTYPE);

char auth[] = "Authentication_Token";
char ssid[] = "WiFi_SSID";
char pass[] = "WiFi_Pass";

BlynkTimer timer;

const int pulse=A0;
 const int motion=D1;
const int gas=D2;

const int threshold=549;
int motionvalue=0; 
int gasvalue=0;
int pulsevalue=0;

void setup() {
Serial.begin(115200);
Blynk.begin(auth, ssid, pass);

dht.begin();
timer.setInterval(1000L, sendSensor);

pinMode(motion,INPUT);
pinMode(gas,INPUT);
pinMode(pulse,INPUT);

}

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
}

void loop() { 

pulsevalue=analogRead(pulse);
gasvalue=digitalRead(gas);
motionvalue=digitalRead(motion);

Serial.print("Gas=");
Serial.println(gasvalue);
Serial.print("Pulse=");
Serial.println(pulsevalue);
Serial.print("Motion=");
Serial.println(motionvalue);

if (pulsevalue<=threshold) {
Blynk.virtualWrite(V4, pulsevalue);
delay(1500);
}

if (gasvalue == 0) {  
Blynk.notify("Toxic Gas Detected..Please take a proper step.........");
//delay(1500);
}

if (motionvalue==1) {  
Blynk.notify("Patient Fall Detected..Please take a proper step.........");
//delay(1500);
}
else {
//Blynk.notify(" Everything is ok........");
//delay(150000);
Blynk.run();
timer.run();
}
}