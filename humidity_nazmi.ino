#include "DHT.h"
#include <RBDdimmer.h>
#include <Wire.h>


#define DHTPIN 15
#define DHTTYPE DHT11  
#define USE_SERIAL  Serial
#define outputPin  4 
#define zerocross  2 


dimmerLamp dimmer(outputPin, zerocross);
DHT dht(DHTPIN, DHTTYPE);

int buttonBlue2=0;
int buttonBlack = 0;
int buttonWhite = 0;
int buttonBlue = 0;
int buttonRed = 0;


int outVal = 0;

void setup() {
  
  USE_SERIAL.begin(115200);
  USE_SERIAL.println(F("DHTxx test!"));
  dht.begin();
  
  USE_SERIAL.begin(115200); 
  dimmer.begin(NORMAL_MODE, ON);  
  USE_SERIAL.println("Dimmer Program is starting...");
  USE_SERIAL.println("Set value");

 

   pinMode(19, INPUT);
   pinMode(25, INPUT);
   pinMode(26, INPUT);
   pinMode(27, INPUT);
   pinMode(32, INPUT);


}
void printSpace(int val)
{
  if ((val / 100) == 0) USE_SERIAL.print(" ");
  if ((val / 10) == 0) USE_SERIAL.print(" ");
}

void loop() {
  // Wait a few seconds between measurements.
  delay(1000);

buttonBlue2= digitalRead(19);
buttonBlack = digitalRead(25);
buttonWhite = digitalRead(26);
buttonBlue = digitalRead(27);
buttonRed = digitalRead(32);







if ((buttonBlue2 == 0)&& (buttonBlack == 0)&& (buttonWhite == 0)&& (buttonBlue == 0)&& (buttonRed == 0))
 { 
 float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);


  if (isnan(h) || isnan(t) || isnan(f)) {
    USE_SERIAL.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));


 if (t <32.0)
       
 {
 Serial.println(F("normal temperature environment"));
 Serial.println(F("auto no speed fan"));
               
 delay(10);
    dimmer.setState(OFF);
     
  
                
 }

  
 if ((t >=32.0)&& (t < 32.5)) {
          
          
 Serial.println(F("warm temperature environment"));
 Serial.println(F("auto speed fan no 1"));

 
 int preVal = outVal;

  if (USE_SERIAL.available())
  {
    int buf = USE_SERIAL.parseInt();
    if (buf != 0) outVal = buf;
    delay(200);
  }
  
   delay(10);
    dimmer.setState(ON);
     
  dimmer.setPower(40); // setPower(0-100%);

  if (preVal != outVal)
  {
    USE_SERIAL.print("lampValue -> ");
    printSpace(dimmer.getPower());
    USE_SERIAL.print(dimmer.getPower());
    USE_SERIAL.println("%");

  }
 }  
 
  
   if ((t >= 32.5) && (t < 33.0))
{
          
          
 Serial.println(F("hot temperature environment"));
 Serial.println(F("auto speed fan no 2"));

 
 int preVal = outVal;

  if (USE_SERIAL.available())
  {
    int buf = USE_SERIAL.parseInt();
    if (buf != 0) outVal = buf;
    delay(200);
  }
   delay(10);
    dimmer.setState(ON);
  dimmer.setPower(50); // setPower(0-100%);

  if (preVal != outVal)
  {
    USE_SERIAL.print("lampValue -> ");
    printSpace(dimmer.getPower());
    USE_SERIAL.print(dimmer.getPower());
    USE_SERIAL.println("%");

  }
 }

     if ((t >= 33.0) && (t < 33.5))
{        
 Serial.println(F("bad temperature environment"));
 Serial.println(F("auto speed fan no 3"));
 
 
 int preVal = outVal;

  if (USE_SERIAL.available())
  {
    int buf = USE_SERIAL.parseInt();
    if (buf != 0) outVal = buf;
    delay(200);
  }
   delay(10);
    dimmer.setState(ON);
  dimmer.setPower(60); // setPower(0-100%);

  if (preVal != outVal)
  {
    USE_SERIAL.print("lampValue -> ");
    printSpace(dimmer.getPower());
    USE_SERIAL.print(dimmer.getPower());
    USE_SERIAL.println("%");

  }
 }           


     if (t >= 33.5) 
{
          
          
 Serial.println(F("worst temperature environment"));
 Serial.println(F("auto speed fan no 4"));
 
 
 int preVal = outVal;

  if (USE_SERIAL.available())
  {
    int buf = USE_SERIAL.parseInt();
    if (buf != 0) outVal = buf;
    delay(200);
  }
   delay(10);
    dimmer.setState(ON);
  dimmer.setPower(90); // setPower(0-100%);

  if (preVal != outVal)
  {
    USE_SERIAL.print("lampValue -> ");
    printSpace(dimmer.getPower());
    USE_SERIAL.print(dimmer.getPower());
    USE_SERIAL.println("%");

  }
 }


 


    Serial.println(F("--------------------------------------"));
 delay(10); 


  }

  

 
  

 if ((buttonBlue2 == 1)&& (buttonBlack == 0)&& (buttonWhite == 0)&& (buttonBlue == 0)&& (buttonRed == 0))
 {
 float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);


  if (isnan(h) || isnan(t) || isnan(f)) {
    USE_SERIAL.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));





   int preVal = outVal;

     delay(10);
    dimmer.setState(OFF);

  Serial.println(F("manual speed fan 0 on"));
  Serial.println(F("--------------------------------------"));
  delay(10); 
  }


  if ((buttonBlue2 == 0)&& (buttonBlack == 1)&& (buttonWhite == 0)&& (buttonBlue == 0)&& (buttonRed == 0))
 {
 float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);


  if (isnan(h) || isnan(t) || isnan(f)) {
    USE_SERIAL.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));





   int preVal = outVal;

  if (USE_SERIAL.available())
  {
    int buf = USE_SERIAL.parseInt();
    if (buf != 0) outVal = buf;
    delay(200);
  }
   delay(10);
    dimmer.setState(ON);
  dimmer.setPower(40); // setPower(0-100%);

  if (preVal != outVal)
  {
    USE_SERIAL.print("lampValue -> ");
    printSpace(dimmer.getPower());
    USE_SERIAL.print(dimmer.getPower());
    USE_SERIAL.println("%");
  }
  Serial.println(F("manual speed fan 1 on"));
  Serial.println(F("--------------------------------------"));
  delay(10); 
  }


 if ((buttonBlue2 == 0)&& (buttonBlack == 0)&& (buttonWhite == 1)&& (buttonBlue == 0)&& (buttonRed == 0))
 {
 float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);


  if (isnan(h) || isnan(t) || isnan(f)) {
    USE_SERIAL.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));





  int preVal = outVal;

  if (USE_SERIAL.available())
  {
    int buf = USE_SERIAL.parseInt();
    if (buf != 0) outVal = buf;
    delay(200);
  }
   delay(10);
    dimmer.setState(ON);
  dimmer.setPower(50); // setPower(0-100%);

  if (preVal != outVal)
  {
    USE_SERIAL.print("lampValue -> ");
    printSpace(dimmer.getPower());
    USE_SERIAL.print(dimmer.getPower());
    USE_SERIAL.println("%");
  }
  Serial.println(F("manual speed fan 2 on"));
  Serial.println(F("--------------------------------------"));
  delay(10); 
  }

   if ((buttonBlue2 == 0)&& (buttonBlack == 0)&& (buttonWhite == 0)&& (buttonBlue == 1)&& (buttonRed == 0))
 {
 float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);


  if (isnan(h) || isnan(t) || isnan(f)) {
    USE_SERIAL.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));


   

 int preVal = outVal;

  if (USE_SERIAL.available())
  {
    int buf = USE_SERIAL.parseInt();
    if (buf != 0) outVal = buf;
    delay(200);
  }
   delay(10);
    dimmer.setState(ON);
  dimmer.setPower(60); // setPower(0-100%);

  if (preVal != outVal)
  {
    USE_SERIAL.print("lampValue -> ");
    printSpace(dimmer.getPower());
    USE_SERIAL.print(dimmer.getPower());
    USE_SERIAL.println("%");
  }

  Serial.println(F("manual Speed fan 3 on"));
  Serial.println(F("--------------------------------------"));
  delay(10); 
  }

  if ( (buttonBlue2 == 0)&& (buttonBlack == 0)&& (buttonWhite == 0)&& (buttonBlue == 0)&& (buttonRed == 1))
 {
 float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);


  if (isnan(h) || isnan(t) || isnan(f)) {
    USE_SERIAL.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));




  int preVal = outVal;

  if (USE_SERIAL.available())
  {
    int buf = USE_SERIAL.parseInt();
    if (buf != 0) outVal = buf;
    delay(200);
  }
   delay(10);
    dimmer.setState(ON);
  dimmer.setPower(90); // setPower(0-100%);

  if (preVal != outVal)
  {
    USE_SERIAL.print("lampValue -> ");
    printSpace(dimmer.getPower());
    USE_SERIAL.print(dimmer.getPower());
    USE_SERIAL.println("%");
  }

  Serial.println(F("manual Speed fan 4 on"));
  Serial.println(F("--------------------------------------"));
  delay(10); 
  }
}
