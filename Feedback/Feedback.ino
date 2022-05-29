#include <Servo.h>
#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

//IR Sensor:

unsigned long beginTime = 0;
unsigned long endTime;
unsigned long distance;
unsigned long sp = 4.5;
int cir = 0;


//Load Cell:
const int HX711_dout = 4; //mcu > HX711 dout pin
const int HX711_sck = 3; //mcu > HX711 sck pin

float containerWeight= 270;
bool massRequirement = true;

//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_eepromAdress = 0;
unsigned long t = 0;

void setup() {

  Serial.begin(57600); delay(10);
  Serial.println();
  Serial.println("Starting...");

  //IR
  pinMode(22, INPUT);

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

  while(massRequirement){
 
  //Load cell
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0; //increase value to slow down serial print activity

  // check for new data/start next conversion:
  if (LoadCell.update()) newDataReady = true;

  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      float i = LoadCell.getData();
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

  if (LoadCell.getData()>=(277) && LoadCell.getData()<=(281)){
      Serial.print("Mass = ");
      Serial.println(LoadCell.getData()-containerWeight);
      Serial.println("The mass design requirement achieved :D");
    }

    else if (LoadCell.getData()<277){
      Serial.print("Mass = ");
      Serial.println(LoadCell.getData()-containerWeight);
      Serial.println("The mass is less than the range. increase the amount of the solution");
    }

    else{
      Serial.print("Mass = ");
      Serial.println(LoadCell.getData()-containerWeight);
      Serial.println("The mass is more than the range. Decrease the amount of the solution");
    }
    }

   //IR Sensor:

   if (digitalRead(22) == 1)
  {
    beginTime = millis();
    while (digitalRead(22))
      endTime = millis();
    Serial.print("begin : ");
    Serial.println(beginTime);
    Serial.print("end : ");
    Serial.println(endTime);
    distance = endTime - beginTime;
    distance *=  sp;
    distance /=  1000;
    distance -=  6;

    if (distance>=(0.08) && distance<=(0.12)){
      Serial.print("Mass = ");
      Serial.println(LoadCell.getData()-containerWeight);
      Serial.println("The mass design requirement achieved :D");
    }
  }

}
