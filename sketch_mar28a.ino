#include <Servo.h>

// Pin Definitions
const int trigPin = 9;
const int echoPin = 10;
const int servoPin = 11;
const int motorPin1 = 5; // Left Motor Forward
const int motorPin2 = 6; // Left Motor Backward

Servo steeringServo;
char command;
bool autoMode = false;

void setup() {
  Serial.begin(9600); // Bluetooth default baud rate
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  
  steeringServo.attach(servoPin);
  steeringServo.write(90); // Center the steering
}

void loop() {
  if (Serial.available() > 0) {
    command = Serial.read();
    executeCommand(command);
  }

  if (autoMode) {
    checkObstacles();
  }
}

void executeCommand(char cmd) {
  switch (cmd) {
    case 'F': moveForward(); break;
    case 'B': moveBackward(); break;
    case 'L': steerLeft(); break;
    case 'R': steerRight(); break;
    case 'S': stopMotors(); break;
    case 'W': autoMode = true; break;  // Toggle Auto-Avoidance ON
    case 'w': autoMode = false; break; // Toggle Auto-Avoidance OFF
  }
}

long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;
}

void checkObstacles() {
  if (getDistance() < 20) {
    stopMotors();
    // Simple logic: If blocked, reverse slightly
    moveBackward();
    delay(500);
    stopMotors();
  }
}

// Movement Functions
void moveForward() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
}

void moveBackward() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
}

void stopMotors() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
}

void steerLeft() { steeringServo.write(45); }
void steerRight() { steeringServo.write(135); }
void steerCenter() { steeringServo.write(90); }
