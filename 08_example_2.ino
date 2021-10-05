// Arduino pin assignment
#define PIN_LED 9
#define PIN_TRIG 12
#define PIN_ECHO 13

// configurable parameters
#define SND_VEL 346.0 // sound velocity at 24 celsius degree (unit: m/s)
#define INTERVAL 25 // sampling interval (unit: ms)
#define _DIST_MIN 100 // minimum distance to be measured (unit: mm)
#define _DIST_MAX 300 // maximum distance to be measured (unit: mm)

// global variables
float timeout; // unit: us
float dist_min, dist_max, dist_raw; // unit: mm
unsigned long last_sampling_time; // unit: ms
float scale; // used for pulse duration to distance conversion
int b;

void setup() {
// initialize GPIO pins
  pinMode(PIN_LED,OUTPUT);
  pinMode(PIN_TRIG,OUTPUT);
  digitalWrite(PIN_TRIG, LOW); 
  pinMode(PIN_ECHO,INPUT);

// initialize USS related variables
  dist_min = _DIST_MIN; 
  dist_max = _DIST_MAX;
  timeout = (INTERVAL / 2) * 1000.0; // precalculate pulseIn() timeout value. (unit: us)
  dist_raw = 0.0; // raw distance output from USS (unit: mm)
  scale = 0.001 * 0.5 * SND_VEL;
  b = 255;
// initialize serial port
  Serial.begin(57600);

// initialize last sampling time
  last_sampling_time = 0;
}

void loop() {
// wait until next sampling time. 
// millis() returns the number of milliseconds since the program started. Will overflow after 50 days.
  if(millis() < last_sampling_time + INTERVAL) return;
  
// get a distance reading from the USS
  dist_raw = USS_measure(PIN_TRIG,PIN_ECHO);
  
// output the read value to the serial port
  Serial.print("Min:0,");
  Serial.print("raw:");
  Serial.print(dist_raw);
  Serial.print(",");
  Serial.println("Max:400");

// turn on the LED if the distance is between dist_min and dist_max
  if(dist_raw > 100.0){
    if(dist_raw <= 201.0){
      analogWrite(PIN_LED, b);
    }
    if(dist_raw <= 190.0){
      analogWrite(PIN_LED, b - 30);
    }
    if(dist_raw <= 180.0){
      analogWrite(PIN_LED, b - 60);
    }
    if(dist_raw <= 170.0){
      analogWrite(PIN_LED, b - 90);
    }
    if(dist_raw <= 160.0){
      analogWrite(PIN_LED, b - 120);
    }
    if(dist_raw <= 150.0){
      analogWrite(PIN_LED, b - 150);
    }
    if(dist_raw <= 140.0){
      analogWrite(PIN_LED, b - 180);
    }
    if(dist_raw <= 130.0){
      analogWrite(PIN_LED, b - 210);
    }
    if(dist_raw <= 120.0){
      analogWrite(PIN_LED, b - 240);
    }
    if(dist_raw <= 110.0){
      analogWrite(PIN_LED, b - 250);
    }
    if(dist_raw <= 102.0){
      analogWrite(PIN_LED, b - 255);
    }
  } else{
    analogWrite(PIN_LED, 0);
  }
  
  
  
// update last sampling time
  last_sampling_time += INTERVAL;
}

// get a distance reading from USS. return value is in millimeter.
float USS_measure(int TRIG, int ECHO)
{
  float reading;
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  reading = pulseIn(ECHO, HIGH, timeout) * scale; // unit: mm
  if(reading < dist_min || reading > dist_max) reading = 0.0; // return 0 when out of range.
  return reading;
  // Pulse duration to distance conversion example (target distance = 17.3m)
  // - round trip distance: 34.6m
  // - expected pulse duration: 0.1 sec, or 100,000us
  // - pulseIn(ECHO, HIGH, timeout) * 0.001 * 0.5 * SND_VEL
  //           = 100,000 micro*sec * 0.001 milli/micro * 0.5 * 346 meter/sec
  //           = 100,000 * 0.001 * 0.5 * 346 * micro * sec * milli * meter
  //                                           ----------------------------
  //                                           micro * sec
  //           = 100 * 173 milli*meter = 17,300 mm = 17.3m
  // pulseIn() returns microseconds.
}
