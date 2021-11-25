#include <LiquidCrystal_I2C.h>
#define RelayPin 4
#define SENSOR  27
#define Relay_On LOW
#define Relay_Off HIGH
#define flowsensor 27
#define SwitchPin 26

long currentMillis = 0;
long previousMillis = 0;
int interval = 1000;
float calibrationFactor = 4.5;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;
unsigned int flowMilliLitres;
int lcdColumns = 16;
int lcdRows = 2;
char digitalSwitch;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

void IRAM_ATTR pulseCounter()
{
  pulseCount++;
}

void setup()
{
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();
  pinMode(SENSOR, INPUT_PULLUP);
  pinMode(RelayPin, OUTPUT);
  pinMode(SwitchPin, INPUT);

  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  previousMillis = 0;

  digitalWrite (RelayPin , Relay_Off);
  attachInterrupt (digitalPinToInterrupt(SENSOR), pulseCounter, FALLING);
}

void loop()
{
  currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    
    pulse1Sec = pulseCount;
    pulseCount = 0;
    
    flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
    previousMillis = millis();
    
    flowMilliLitres = (flowRate / 60) * 1000;

    digitalSwitch = Serial.read();
    
    int switchStatus = digitalRead(SwitchPin);
    if (switchStatus == LOW)
    {
      lcd.clear();
      digitalWrite (RelayPin , Relay_On);
      lcd.setCursor (0, 0);
      lcd.print ("Flow rate: ");
      lcd.print (float (flowRate));
      lcd.setCursor (0,1);
      lcd.print("Valve Close");
    }
    else if (switchStatus == HIGH)
    {
      lcd.clear();
      digitalWrite (RelayPin , Relay_Off);
      lcd.setCursor (0, 0);
      lcd.print ("Flow rate: ");
      lcd.print (float (flowRate));
      lcd.setCursor (0,1);
      lcd.print("Valve Open");
    }
  }
}
