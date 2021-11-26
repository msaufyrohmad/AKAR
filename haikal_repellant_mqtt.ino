//INSECT/ANIMAL REPELLENT
//219.92.246.201  - interxsgroup.com.my
//username : dashboard
//password : dashboard!@#

#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include<MQTTPubSubClient.h>
#include "ESPDateTime.h"

const char* ssid = "ammaza@unifi";
const char* pass = "ammaza8214";

//const char* ssid = "4G CPE 4D0C";
//const char* pass = "88888888";

WiFiClient client;
MQTTPubSubClient mqtt;

TaskHandle_t Task1;
TaskHandle_t Task2;

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 

int waterpump = 23;          // the pin that the waterpump is atteched to
int sensor = 5;            // the pin that the sensor is atteched to
int state = LOW;            // by default, no motion detected
int val = 0;                // variable to store the sensor status (value)

int relay1 = 32;
int relay2 = 33;
int relay3 = 18;

int Htime;              //integer for storing high time
int Ltime;              //integer for storing low time
float Ttime;            //integer for storing total time of a cycle
int frequency;        //storing frequency

int sensorpir = 2;
int laser = 4;
int buzzer = 15;



void setup() {
  Serial.begin(115200);
 //wifi and mqtt setup
  WiFi.begin(ssid, pass);

    Serial.print("connecting to wifi...");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println(" connected!");
    
     Serial.print("connecting to host ...");
    while (!client.connect("interxsgroup.com.my",1883)) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("Client Connected");
    
    mqtt.begin(client);
       Serial.print("connecting to mqtt broker...");
    while (!mqtt.connect("Sensor Station 20","dashboard","dashboard!@#")) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("Connected to MQTT Broker!");

//time stuff
  DateTime.setServer("time.pool.aliyun.com");
  DateTime.setTimeZone("CST-8");
  DateTime.begin();
  if (!DateTime.isTimeValid()) {
    Serial.println("Failed to get time from server.");
  } else {
    Serial.printf("Date Now is %s\n", DateTime.toISOString().c_str());
    Serial.printf("Timestamp is %ld\n", DateTime.now());
  }

    
  pinMode(sensor, INPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);

  pinMode(waterpump, OUTPUT);
  pinMode(sensorpir, INPUT);
  pinMode(laser, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  //I2C setup
  lcd.init();                      
  lcd.backlight();

  //create a task that will be executed in Task1code() func, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
          Task1code,
          "Task1",
          10000,
          NULL,
          1,
          &Task1,
          0);
  delay(1000);

   //create a task that will be executed in Task2code() func, with priority 1 and executed on core 1
   xTaskCreatePinnedToCore(
          Task2code,
          "Task2",
          10000,
          NULL,
          1,
          &Task2,
          1);
  delay(1000);
}

void Task1code(void * pvParameters){
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){

    //ReLAY 1
    digitalWrite(relay1,HIGH);
    delay(1000);
    freq();    
    digitalWrite(relay1,LOW);
    delay(3000);

    //ReLAY 2
    digitalWrite(relay2,HIGH);
    delay(1000);
    freq();    
    digitalWrite(relay2,LOW);
    delay(3000);

    //ReLAY 3
    digitalWrite(relay3,HIGH);
    delay(1000);
    freq();
    digitalWrite(relay3,LOW);
    delay(3000);
  }
}

void Task2code(void * pvParameters){
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    spray();
  }
}

void loop(){}


//Code for reading frequency
 void freq(){   
  
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Frequency of sig");

    Htime=pulseIn(sensor,HIGH);      //read high time
    Ltime=pulseIn(sensor,LOW);       //read low time
    
    Ttime = Htime+Ltime;

    frequency=1000000/Ttime;    //getting frequency with Ttime is in Micro seconds
    lcd.setCursor(0,1);
    lcd.print(frequency);
    lcd.print(" Hz");
    delay(5000);
}

//Code for Motion sensor
 void spray(){
    val = digitalRead(sensorpir);   // read sensor value

  if (val == HIGH) {           // check if the sensor is HIGH
    mqtt.publish("/ss20/intrusion_attempt",DateTime.toString());
    digitalWrite(waterpump, HIGH);   // turn waterpump ON
    digitalWrite(laser, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(5000);
    
    if (state == LOW) {
      state = HIGH;       // update variable state to HIGH
      delay(500);
    }
  } 
  else {
      digitalWrite(waterpump, LOW); // turn waterpump OFF
      delay(500);             

      digitalWrite(laser, LOW);
      digitalWrite(buzzer, LOW);
      delay(500);
        
        if (state == HIGH){
        state = LOW;       // update variable state to LOW
        delay(500);
    }
  }
}
