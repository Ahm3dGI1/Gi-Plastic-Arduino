#include <Servo.h>
#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

// motor Variables
int motor1pin1 = 6;
int motor1pin2 = 7;
bool blend = true;

//Servo Variables
Servo myservo;

//IR
int IRSensor = 2;

//Load Cell

//pins:
const int HX711_dout = 4; //mcu > HX711 dout pin
const int HX711_sck = 3; //mcu > HX711 sck pin

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
  float calibrationValue = 390.56; // calibration value (see example file "Calibration.ino")
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
    if (blend){
      digitalWrite(motor1pin1, HIGH);
      digitalWrite(motor1pin2, LOW);
    
      delay(60000);
    
      digitalWrite(motor1pin1, LOW);
      digitalWrite(motor1pin2, LOW);
    
  
      //Servo
  
    for (int pos = 0; pos <= 120; pos += 1) {
      myservo.write(pos);
      delay(100);
    }

    blend = false;
  }

  //Load cell
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0; //increase value to slow down serial print activity

  // check for new data/start next conversion:
  if (LoadCell.update()) newDataReady = true;

  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      float i = LoadCell.getData();
      Serial.println("Mass =");
      Serial.println(i);
      if (i > 350){
          myservo.write(0);
        }
      }
      newDataReady = 0;
      t = millis();
    } 

  // receive command from serial terminal, send 't' to initiate tare operation:
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't') LoadCell.tareNoDelay();
  }

  // check if last tare operation is complete:
  if (LoadCell.getTareStatus() == true) {
    Serial.println("Tare complete");
  }

 }
 
