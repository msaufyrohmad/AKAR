#include <PubSubClient.h>
//wind sensor
#define READ_TIME 1000 //ms
#define WIND_SENSOR_PIN 13 //wind sensor pin
#define WIND_SPEED_20_PULSE_SECOND 1.75  //in m/s this value depend on the sensor type
#define ONE_ROTATION_SENSOR 20.0
//rain sensor analog pin25
#define rainAnalog 25

//PM SENSOR
#include <SoftwareSerial.h>
#include <Sds011.h>
#define SDS_PIN_RX 16
#define SDS_PIN_TX 17
#ifdef ESP32
HardwareSerial& serialSDS(Serial2);
Sds011Async< HardwareSerial > sds011(serialSDS);
#else
SoftwareSerial serialSDS;
Sds011Async< SoftwareSerial > sds011(serialSDS);
#endif

// The example stops the sensor for 210s, then runs it for 30s, then repeats.
// At tablesizes 20 and below, the tables get filled during duty cycle
// and then measurement completes.
// At tablesizes above 20, the tables do not get completely filled
// during the 30s total runtime, and the rampup / 4 timeout trips,
// thus completing the measurement at whatever number of data points
// were recorded in the tables.
constexpr int pm_tablesize = 20;
int pm25_table[pm_tablesize];
int pm10_table[pm_tablesize];

bool is_SDS_running = true;

void start_SDS() {
    Serial.println("Start wakeup SDS011");

    if (sds011.set_sleep(false)) { is_SDS_running = true; }

    Serial.println("End wakeup SDS011");
}

void stop_SDS() {
    Serial.println("Start sleep SDS011");

    if (sds011.set_sleep(true)) { is_SDS_running = false; }

    Serial.println("End sleep SDS011");
}

//wind sensor
volatile unsigned long Rotations; //Cup rotation counter used in interrupt routine

float WindSpeed; //Speed meter per second

unsigned long gulStart_Read_Timer = 0;

TaskHandle_t Task1;
TaskHandle_t Task2;

void setup(){

  //wind sensor serial begin
  Serial.begin(9600);
  pinMode(WIND_SENSOR_PIN,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(WIND_SENSOR_PIN),isr_rotation, CHANGE); //Set up the interrupt

  Serial.println("Rotations\tm/s");
  sei(); //Enables interrupts
  gulStart_Read_Timer - millis();

 //rain sensor serial begin
  Serial.begin(9600);
  pinMode(rainAnalog,INPUT);

  //PM sensor serial begin
  Serial.begin(9600);

#ifdef ESP32
    serialSDS.begin(9600, SERIAL_8N1, SDS_PIN_RX, SDS_PIN_TX);
    delay(100);
#else
    serialSDS.begin(9600, SWSERIAL_8N1, SDS_PIN_RX, SDS_PIN_TX, false, 192);
#endif

    Serial.println("SDS011 start/stop and reporting sample");

    Sds011::Report_mode report_mode;
    if (!sds011.get_data_reporting_mode(report_mode)) {
        Serial.println("Sds011::get_data_reporting_mode() failed");
    }
    if (Sds011::REPORT_ACTIVE != report_mode) {
        Serial.println("Turning on Sds011::REPORT_ACTIVE reporting mode");
        if (!sds011.set_data_reporting_mode(Sds011::REPORT_ACTIVE)) {
            Serial.println("Sds011::set_data_reporting_mode(Sds011::REPORT_ACTIVE) failed");
        }
    }

    xTaskCreate(
    Task1code,
    "Task1",
    10000,
    NULL,
    1,
    NULL);
   


  xTaskCreate(
    Task2code,   /* Task function. */
    "Task2",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    NULL);          /* pin task to core 1 */
    
}

void Task1code(void * Parameters)
  {
    for(;;){
      readwind();
      readrain();  
      vTaskDelay(1000 / portTICK_PERIOD_MS);  
    }
   
     
  }

void Task2code(void * Parameters)
  {
    for(;;){
      readSDS();
      vTaskDelay(1000 / portTICK_PERIOD_MS);  
    }
  }

void loop(){}


int n=0; //for table reading in wind sensor
void readwind()
{ 
                                                        //WIND SENSOR DATA MEASUREMENT

  if((millis() - gulStart_Read_Timer) >= READ_TIME)
  {
    cli(); //Disable interrupts
   
    //convert rotation to wind speed in m/s
    WindSpeed = WIND_SPEED_20_PULSE_SECOND/ONE_ROTATION_SENSOR*(float)Rotations;
    Serial.print(Rotations); Serial.print("\t\t"); 
    Serial.println(WindSpeed); 
    
    sei(); //Enables interrupts

    Rotations = 0; //Set Rotations count to 0 ready for calculations
    gulStart_Read_Timer = millis();
 

    float winds[n];
    if (WindSpeed>=11.0)
    {
        WindSpeed = winds[n];
        n++;
        if (n=4){
          //send data mqtt
        }
    }
      else
    {
      n=0; 
    }
  }
}

void readrain()
{
                                              //RAIN SENSOR RESISTANCE MEASUREMENT
  int rainAnalogVal = analogRead(rainAnalog);

  Serial.print(rainAnalogVal);
  Serial.print("\t");
  
  delay(200);

  if (rainAnalogVal<=2500)
  {
     //send data mqtt
  } 
 
}

void readSDS()
{
                                                    //PM SENSOR MEASUREMENT

  // Per manufacturer specification, place the sensor in standby to prolong service life.
    // At an user-determined interval (here 210s down plus 30s duty = 4m), run the sensor for 30s.
    // Quick response time is given as 10s by the manufacturer, thus the library drops the
    // measurements obtained during the first 10s of each run.
  
    constexpr uint32_t down_s = 60;

    stop_SDS();
    Serial.print("stopped SDS011 (is running = ");
    Serial.print(is_SDS_running);
    Serial.println(")");

    uint32_t deadline = millis() + down_s * 1000;
    while (static_cast<int32_t>(deadline - millis()) > 0) {
        delay(1000);
        Serial.println(static_cast<int32_t>(deadline - millis()) / 1000);
        sds011.perform_work();
    }

    constexpr uint32_t duty_s = 30;

    start_SDS();
    Serial.print("started SDS011 (is running = ");
    Serial.print(is_SDS_running);
    Serial.println(")");

    sds011.on_query_data_auto_completed([](int n) {
        Serial.println("Begin Handling SDS011 query data");
        int pm25;
        int pm10;
        Serial.print("n = "); Serial.println(n);
        if (sds011.filter_data(n, pm25_table, pm10_table, pm25, pm10) &&
            !isnan(pm10) && !isnan(pm25)) {
            Serial.print("PM10: ");
            Serial.println(float(pm10) / 10);
            Serial.print("PM2.5: ");
            Serial.println(float(pm25) / 10);

            if (pm10 >=50 || pm25 >=50){

              //send data to mqtt

            }
        }
        Serial.println("End Handling SDS011 query data");
        });

    if (!sds011.query_data_auto_async(pm_tablesize, pm25_table, pm10_table)) {
        Serial.println("measurement capture start failed");
    }

    deadline = millis() + duty_s * 1000;
    while (static_cast<int32_t>(deadline - millis()) > 0) {
        delay(1000);
        Serial.println(static_cast<int32_t>(deadline - millis()) / 1000);
        sds011.perform_work();
    }


}

void isr_rotation()
  {
      Rotations++;
  }
