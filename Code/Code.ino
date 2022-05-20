#include <Servo.h>
#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

// motor Variables
int motor1pin1 = 2;
int motor1pin2 = 3;

//Servo Variables
Servo myservo;

//IR
int IRSensor = 8;

//Load Cell
//pins:
const int HX711_dout = 5;
const int HX711_sck = 6;

//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_eepromAdress = 0;
unsigned long t = 0;


void setup() {

  Serial.begin(57600); delay(10);
  Serial.println();
  Serial.println("Starting...");

  //Servo pins
  myservo.attach(9);  
  myservo.write(0);

  //motor pins
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);

  //Light Sensor
  pinMode (IRSensor, INPUT);
  
  //Load Cell
   LoadCell.begin();
  
  float calibrationValue;
  calibrationValue = 696.0;
#if defined(ESP8266)|| defined(ESP32)
#endif

  unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
    Serial.println("Startup is complete");
  }
}

void loop() {
  // Blender
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);
  
  delay(10000);
  
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, LOW);

  //Servo
  
  for (int pos = 0; pos <= 180; pos += 1) {
    myservo.write(pos);
    delay(50);
  }

  //Load cell
  if (LoadCell.getData()> 350){
    
    for (int pos = 0; pos <= 180; pos += 1) {
    myservo.write(pos);
    delay(50);
  }
  
  }
  
  //light sensor
  int dis = digitalRead (IRSensor);
  if (dis < 0.1 && dis > 0.08)
  {
    Serial.print("Thickness is within range:" + dis + "cm");
  }

 }
 
