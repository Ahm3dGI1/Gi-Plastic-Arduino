#include <Servo.h>
#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

//IR Sensor:

double duration; // variable for the duration of sound wave travel
float distance; // variable for the distance measurement

#define echoPin 12 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 11


//Load Cell:
const int HX711_dout = 4; //mcu > HX711 dout pin
const int HX711_sck = 3; //mcu > HX711 sck pin

float containerWeight= 273;
int mass;

//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_eepromAdress = 0;
unsigned long t = 0;

void setup() {

  Serial.begin(57600); delay(10);
  Serial.println();
  Serial.println("Starting...");

  //IR
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT

  //Load Cell
  LoadCell.begin();
  float calibrationValue = 390.56; // calibration value (see example file "Calibration.ino")
  unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.printlnx("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
    Serial.println("Startup is complete");
  }

}

void loop() {
 
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

  mass = LoadCell.getData() - containerWeight;
  if (mass>=(7) && mass<=(11)){
      Serial.print("Mass = ");
      Serial.println(LoadCell.getData()-containerWeight);
      Serial.println("The mass design requirement achieved");
    }

    else if (mass<7){
      Serial.print("Mass = ");
      Serial.println(mass);
      Serial.print("The mass is less than the range. increase the amount of the solution by ");
      Serial.print((9-mass)/0.0257);
      Serial.println("ml");
    }

    else{
      Serial.print("Mass = ");
      Serial.println(mass);
      Serial.print("The mass is more than the range. Decrease the amount of the solution by ");
      Serial.print((9-mass)/-0.0257);
      Serial.println("ml");
    }

//IR Sensor:

  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  distance -= 10.25; // Distance from the sensor to the container

    if (distance>=(0.08) && distance<=(0.12)){
      Serial.print("Thickness = ");
      Serial.print(distance);
      Serial.println(" cm");
      Serial.println("The thickness design requirement achieved");
    }

    else if (distance<(0.08)){
      Serial.print("Thickness = ");
      Serial.print(distance);
      Serial.println(" cm");
      Serial.println("The thickness is below the range. Increase the gelatin ratio");
    }

    else{
      Serial.print("Thickness = ");
      Serial.print(distance);
      Serial.println(" cm");
      Serial.println("The thickness is above the range. Decrease the gelatin ratio");
    }
  }
