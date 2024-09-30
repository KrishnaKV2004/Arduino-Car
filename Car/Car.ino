/*
    Arduino Car
    Author --> Krishna Verma

    Requirements -->

    • Arduino
    • L293D Motor Shield
*/

#include <AFMotor.h>
#include <SoftwareSerial.h>

// Define motors
AF_DCMotor motor1(1); // Motor 1
AF_DCMotor motor2(2); // Motor 2
AF_DCMotor motor3(3); // Motor 3
AF_DCMotor motor4(4); // Motor 4

// Define Ultrasonic Sensor Pins
const int trigPin = 8;
const int echoPin = 9;

// Bluetooth communication pins
SoftwareSerial Bluetooth(10, 11); // RX, TX for Bluetooth (use pins 10, 11)

// Variables for distance and speed control
long duration, distance;
int speedLevel = 255; // Default speed level (max)

// Function declarations
void controlMotors(char command);
void controlSpeed(char speedCommand);
void stopMotors();
void avoidObstacle();

void setup() {
  // Setup communication and pins
  Serial.begin(9600); // Serial Monitor for debugging
  Bluetooth.begin(9600); // Bluetooth communication at 9600 baud rate
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.println("System Initialized. Ready for commands.");
}

void loop() {
  // Measure distance using ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = (duration * 0.034) / 2; // Calculate distance in cm

  // Obstacle avoidance logic
  if (distance < 20) { 
    avoidObstacle();
  } else {
    // Receive commands from Bluetooth
    if (Bluetooth.available()) {
      char command = Bluetooth.read();
      if (command >= '1' && command <= '3') {
        controlSpeed(command); // Control speed levels
      } else {
        controlMotors(command); // Control car movements
      }
    }
  }
}

// Function to control speed levels
void controlSpeed(char speedCommand) {
  switch (speedCommand) {
    case '1': // Low speed
      speedLevel = 100;
      Serial.println("Speed set to low.");
      break;
    case '2': // Medium speed
      speedLevel = 180;
      Serial.println("Speed set to medium.");
      break;
    case '3': // High speed
      speedLevel = 255;
      Serial.println("Speed set to high.");
      break;
    default:
      speedLevel = 255; // Default speed
  }
}

// Function to control motor movements based on Bluetooth command
void controlMotors(char command) {
  switch (command) {
    case 'F': // Move Forward
      motor1.setSpeed(speedLevel);
      motor2.setSpeed(speedLevel);
      motor3.setSpeed(speedLevel);
      motor4.setSpeed(speedLevel);
      motor1.run(FORWARD);
      motor2.run(FORWARD);
      motor3.run(FORWARD);
      motor4.run(FORWARD);
      Serial.println("Moving forward.");
      break;
      
    case 'B': // Move Backward
      motor1.setSpeed(speedLevel);
      motor2.setSpeed(speedLevel);
      motor3.setSpeed(speedLevel);
      motor4.setSpeed(speedLevel);
      motor1.run(BACKWARD);
      motor2.run(BACKWARD);
      motor3.run(BACKWARD);
      motor4.run(BACKWARD);
      Serial.println("Moving backward.");
      break;
      
    case 'L': // Turn Left
      motor1.setSpeed(speedLevel);
      motor2.setSpeed(speedLevel);
      motor3.setSpeed(0);
      motor4.setSpeed(0);
      motor1.run(BACKWARD);
      motor2.run(BACKWARD);
      Serial.println("Turning left.");
      break;
      
    case 'R': // Turn Right
      motor1.setSpeed(0);
      motor2.setSpeed(0);
      motor3.setSpeed(speedLevel);
      motor4.setSpeed(speedLevel);
      motor3.run(FORWARD);
      motor4.run(FORWARD);
      Serial.println("Turning right.");
      break;
      
    case 'S': // Stop the car
      stopMotors();
      Serial.println("Car stopped.");
      break;
      
    default:
      Serial.println("Invalid command.");
      break;
  }
}

// Function to stop all motors
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

// Function to avoid obstacle
void avoidObstacle() {
  Serial.println("Obstacle detected! Avoiding...");
  
  // Stop the car first
  stopMotors();
  delay(500); // Wait a bit

  // Simple obstacle avoidance logic: move backward and then turn right
  motor1.setSpeed(speedLevel);
  motor2.setSpeed(speedLevel);
  motor3.setSpeed(speedLevel);
  motor4.setSpeed(speedLevel);
  
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  
  delay(1000); // Move backward for 1 second

  stopMotors();
  delay(500);
  
  // Turn right after moving backward
  motor1.setSpeed(0);
  motor2.setSpeed(0);
  motor3.setSpeed(speedLevel);
  motor4.setSpeed(speedLevel);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  
  delay(1000); // Turn for 1 second
  stopMotors();
}