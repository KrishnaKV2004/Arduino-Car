#include <AFMotor.h>

AF_DCMotor motor1(1); // Motor 1
AF_DCMotor motor2(2); // Motor 2
AF_DCMotor motor3(3); // Motor 3
AF_DCMotor motor4(4); // Motor 4

const int trigPin = 8;
const int echoPin = 9;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  long duration, distance;
  
  // Trigger the ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = (duration * 0.034) / 2; // Calculate distance in cm
  
  // Check distance
  if (distance < 20) { // If distance is less than 20 cm
    stopMotors();
  } else {
    if (Serial.available()) {
      char command = Serial.read();
      controlMotors(command);
    }
  }
}

void controlMotors(char command) {
  switch (command) {
    case 'F': // Forward
      motor1.setSpeed(255);
      motor2.setSpeed(255);
      motor3.setSpeed(255);
      motor4.setSpeed(255);
      motor1.run(FORWARD);
      motor2.run(FORWARD);
      motor3.run(FORWARD);
      motor4.run(FORWARD);
      break;
    case 'B': // Backward
      motor1.setSpeed(255);
      motor2.setSpeed(255);
      motor3.setSpeed(255);
      motor4.setSpeed(255);
      motor1.run(BACKWARD);
      motor2.run(BACKWARD);
      motor3.run(BACKWARD);
      motor4.run(BACKWARD);
      break;
    case 'L': // Left
      motor1.setSpeed(255);
      motor2.setSpeed(255);
      motor3.setSpeed(0);
      motor4.setSpeed(0);
      motor1.run(BACKWARD);
      motor2.run(BACKWARD);
      break;
    case 'R': // Right
      motor1.setSpeed(0);
      motor2.setSpeed(0);
      motor3.setSpeed(255);
      motor4.setSpeed(255);
      motor3.run(FORWARD);
      motor4.run(FORWARD);
      break;
    case 'S': // Stop
      stopMotors();
      break;
    default:
      break;
  }
}

void stopMotors() {
  motor1.setSpeed(0);
  motor2.setSpeed(0);
  motor3.setSpeed(0);
  motor4.setSpeed(0);
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}