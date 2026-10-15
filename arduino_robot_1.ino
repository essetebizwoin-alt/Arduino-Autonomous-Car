
#include <Servo.h>
const unsigned long interval = 100;
unsigned long previousTime = 0;
unsigned long currentTime = 0;
unsigned long motorStartTime = 0;
unsigned long ledPreviousTime = 0; // all the values needed to use millis()

const byte rightForwardPin = 13;
const byte rightBackwardPin = 12;
const byte leftForwardPin = 11;
const byte leftBackwardPin = 10;
const byte rightSpeed = 5;
const byte leftSpeed = 6; // motor pins

const byte trigPin = 9;
const byte echoPin = 7; // ultrasonic sensor pins

long duration;
long cm; 
const long threshold = 100; // ultrasonic sensor variables

Servo servo; // servo motor

const byte LED1 = 4;
const byte LED2 = 3; // indicator LEDs for danger

bool servoAt90 = false;
bool ledState = false;
bool led2State = false;
bool motorRunning = false; // all the booleans needed for the if statements

void setup() {
  Serial.begin(9600);// beginning serial communication
  
  pinMode(rightForwardPin, OUTPUT);
  pinMode(rightBackwardPin, OUTPUT);
  pinMode(leftForwardPin, OUTPUT);
  pinMode(leftBackwardPin, OUTPUT);
  pinMode(rightSpeed, OUTPUT);
  pinMode(leftSpeed, OUTPUT); // motor pins

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT); // sensor pins
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  
  Serial.print("Distance: ");
  Serial.println(cm);

  servo.attach(8); // Attach the servo to pin 8 instead of pin 9
}

void loop() {
  currentTime = millis(); //making the variable current time hold the value of millis
  
  sonarSensor(); // UltraSonic sensor logic ( uses delay not to over complicate the function just for 2 to 10 microseconds)
  
  if (motorRunning && currentTime - motorStartTime >= 2000) {
    stopMotor(); // for control
  }
    
  if ( cm < threshold){
  
    stopMotor();       // stopping the motor
    ledF2(); // lighting the red led to indicate some thing is in front of the sensor
    servoTo90(); //turn the servo motor to 90 degrees
    
  }else{
    
    ledF1(); // led to indicate a safe passage
    servoTo0(); //return the motor back to the original position
    driveForward(255);  // DC motor will drive forward
    
  }
}

void driveForward(byte speed) {
    motorRunning = true;
    motorStartTime = currentTime;

    // Turn on motor and activating the forward pins
    digitalWrite(rightForwardPin, HIGH);
    digitalWrite(leftForwardPin, HIGH);
    digitalWrite(rightBackwardPin, LOW);
    digitalWrite(leftBackwardPin, LOW);
    analogWrite(rightSpeed, speed);
    analogWrite(leftSpeed, speed);
 
}

void driveBackward(byte speed) {
    motorRunning = true;
    motorStartTime = currentTime;
    
    // turning on the motor the activating the backward pins
    digitalWrite(rightForwardPin, LOW);
    digitalWrite(leftForwardPin, LOW);
    digitalWrite(rightBackwardPin, HIGH);
    digitalWrite(leftBackwardPin, HIGH);
    analogWrite(rightSpeed, speed);
    analogWrite(leftSpeed, speed);
  
}

void turnLeft(byte right_speed, byte left_speed) {
     motorRunning = true;
     motorStartTime = currentTime;
     
     // turning on the motor the activating the opposite pins
     digitalWrite(rightForwardPin, LOW);
     digitalWrite(leftForwardPin, HIGH);
     digitalWrite(rightBackwardPin, HIGH);
     digitalWrite(leftBackwardPin, LOW);
     analogWrite(rightSpeed, right_speed);
     analogWrite(leftSpeed, left_speed);
   
}

void turnRight(byte right_speed, byte left_speed) {
    motorRunning = true;
    motorStartTime = currentTime;
    
    // turning on the motor the activating the opposite pins (the ones opposite from the turn left function)
    digitalWrite(rightForwardPin, HIGH);
    digitalWrite(leftForwardPin, LOW);
    digitalWrite(rightBackwardPin, LOW);
    digitalWrite(leftBackwardPin, HIGH);
    analogWrite(rightSpeed, right_speed);
    analogWrite(leftSpeed, left_speed);
 
}

void stopMotor() {
    motorRunning = false;
    motorStartTime = currentTime;
  
   // turning the motor off and keeping all the pins low
   digitalWrite(rightForwardPin, LOW);
   digitalWrite(rightBackwardPin, LOW);
   digitalWrite(leftForwardPin, LOW);
   digitalWrite(leftBackwardPin, LOW);
 
}

void sonarSensor(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10); // releasing a trigger (sound) from the trig pin
  
  duration = pulseIn(echoPin, HIGH); //reading how long it took the trigger to be released and come back
  cm= duration * 0.034/2; // multiplying the time with the speed of sound then dividing by 2, to get half the distance
  
}
void ledF2(){
  
   if (currentTime - ledPreviousTime >= interval){
      previousTime = currentTime;
      
       // Toggle LED state
     ledState = !ledState;
     digitalWrite(LED2, ledState);
   }
  
  digitalWrite(LED1, LOW);
}
void ledF1(){
    if (currentTime - ledPreviousTime >= interval){
      previousTime = currentTime;
      
       // Toggle LED state
     ledState = !ledState;
     digitalWrite(LED1, ledState);
     digitalWrite(LED2, led2State); // trying a different logic to see if there is a difference
     // no difference was detected on the simulation
    }
 
}

void servoTo90(){
   if (!servoAt90) {
      servo.write(90);        // Only write if needed
      servoAt90 = true;       // Remember it's now at 90
    }
}
void servoTo0(){
   if (servoAt90) {
      servo.write(0);        // Only write if needed
      servoAt90 = false;       // Remember it's now at 0
    }
}
  