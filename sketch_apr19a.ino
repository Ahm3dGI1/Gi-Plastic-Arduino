#include <Servo.h>

Servo myservo;
int motor1pin1 = 30;
int motor1pin2 = 31;
void setup() {

  //Servo pins
  myservo.attach(9);
  myservo.write(0);

  //motor pins
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);

  //Light Sensor

  //Weight Cell
}

void loop() {
  myservo.write(0);
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

}



/*
// motor Variables
int motor1pin1 = 30;
int motor1pin2 = 31;

//Servo Variables
Servo myservo;

void setup() {

  //Servo pins
  myservo.attach(9);  
  myservo.write(0);

  //motor pins
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);

  //Light Sensor

  //Weight Cell
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

  //button for the feedback to start

  //light sensor

  //weight cell
 }
 */
