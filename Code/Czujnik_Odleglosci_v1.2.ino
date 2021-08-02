// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP32CORE_WIFI_POINT
#include <WiFi.h>

#include <RemoteXY.h>

// RemoteXY connection settings 
#define REMOTEXY_WIFI_SSID "Parking Sensor V1"
#define REMOTEXY_WIFI_PASSWORD "makita123"
#define REMOTEXY_SERVER_PORT 6377
unsigned long time_since_start; // time since start
unsigned long last_time;        // timestamp of last event 
unsigned long time_in_memory; 
#define trigPin 32              // output, sensor trigger pin
#define echoPin 27              // input, sensor echo pin
unsigned long duration;         // information about how long it takes for the sound wave to return to the sensor
int distance;                   // distance to nearest obstacle
int val;


// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
  { 255,1,0,22,0,44,0,13,24,0,
  66,128,15,13,71,27,36,24,67,4,
  22,3,43,10,31,24,19,69,0,0,
  53,10,10,25,2,1,-1,47,12,6,
  24,25,31,31,79,78,0,79,70,70,
  0 }; 
  
// this structure defines all the variables and events of your control interface 
struct {
    // input variables
  uint8_t sound; // =1 if switch ON and =0 if OFF 
  
    // output variables
  int8_t Odleglosc; // =0..100 level position 
  char tekst[19];  // string UTF8 end zero 
  int16_t alarm; // =0 no sound, else ID of sound, =1001 for example, look sound list in app 
  
    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)
  
void setup() {

  RemoteXY_Init (); 
  
  Serial.begin(115200);
  // TODO you setup code
 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  strcpy (RemoteXY.tekst, "Loading...");

  int x = 0;
}


void sensor() {
  
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  
  //Serial.println(distance); // for diagnositcs
  
  if (distance <= 200) {
    val = map(distance, 0, 200, 100, 0);
    RemoteXY.Odleglosc = val;
      if (distance <= 100 && distance > 40 && RemoteXY.sound == 1){
        RemoteXY.alarm = 2003;
      }
      else if (distance <= 40 && distance > 20 && RemoteXY.sound == 1){
        RemoteXY.alarm = 2007;
      }
      else if (distance <= 20 && distance > 10 && RemoteXY.sound == 1){
        RemoteXY.alarm = 2030;
      }
      else if (distance <= 10 && RemoteXY.sound == 1){
        RemoteXY.alarm = 2026;
      }
      else if (distance > 50 || RemoteXY.sound == 0){
        RemoteXY.alarm = 0;
      }
    char str[] = "Distance:";
    char unit[] = "cm";
    sprintf (RemoteXY.tekst, "%s %d %s", str, distance, unit); // result: "Value is 1234" 
  }
  
  else if (distance > 200){
    strcpy (RemoteXY.tekst, "Out of Reach");
  }
}


void loop() 
{ 
  RemoteXY_Handler ();

  time_since_start = millis();
  
  // TODO you loop code
  // use the RemoteXY structure for data transfer
  // do not call delay()
  
  if (time_since_start - last_time >= 100) {
    last_time = time_since_start;
    sensor();
  }
}
