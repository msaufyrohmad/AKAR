
#include <Wire.h>
#include <BH1750.h>
 
#define USE_SERIAL  Serial


BH1750 lightMeter(0x23);


int buttonRedlamp = 0;


int outVal = 0;

void setup() {


  pinMode(18, OUTPUT);
  Serial.begin(115200);
  Wire.begin();
  lightMeter.begin();
  Serial.println(F("BH1750 Test begin"));

  
   pinMode(33, INPUT);
 

}

void loop() {
  // Wait a few seconds between measurements.
  delay(1000);

buttonRedlamp = digitalRead(33);

if ((buttonRedlamp == 0))
 { 
 
  float lux = lightMeter.readLightLevel();
  Serial.print(F("Light: "));
  Serial.print(lux);
  Serial.println(F(" lx"));

  if (lux < 0) {
   Serial.println(F("Error condition detected"));
  }
  else {
    if (lux > 40000.0) {
     
      if (lightMeter.setMTreg(32)) {
        Serial.println(F("Setting MTReg to low value for high light environment"));
        digitalWrite(18,LOW);       
      }
      else {
       Serial.println(F("Error setting MTReg to low value for high light environment"));
      }
    }
    else {
        if (lux >50.0) {
          
          if (lightMeter.setMTreg(69)) {
            Serial.println(F("normal light environment"));
             Serial.println(F("auto lamp off"));
             digitalWrite(18,LOW);

            
         }
         else {
            Serial.println(F("Error setting MTReg to default value for normal light environment"));
          }
        }
        else {
          if (lux <=50.0) {
           
           if (lightMeter.setMTreg(138)) {
             Serial.println(F("bad light environment"));
               Serial.println(F("auto lamp on"));
                
                digitalWrite(18,HIGH);
                             
           }
            else {
              Serial.println(F("Error setting MTReg to high value for low light environment"));
            }
          }
       }
    }

  }

    Serial.println(F("--------------------------------------"));
 delay(10); 
  }
  if (buttonRedlamp == 1)
 { 
  float lux = lightMeter.readLightLevel();
  Serial.print(F("Light: "));
  Serial.print(lux);
  Serial.println(F(" lx"));
  digitalWrite(18,LOW);
            
   Serial.println(F("manual lamp on"));
    Serial.println(F("--------------------------------------"));
 delay(10); 


  }
}

// if (buttonRedlamp == 0)
// { 
 
 

 // float lux = lightMeter.readLightLevel();
 // Serial.print(F("Light: "));
 // Serial.print(lux);
 // Serial.println(F(" lx"));

 
 // digitalWrite(18,LOW);
        
     

   // Serial.println(F("--------------------------------------"));
// delay(10); 


 // }v
