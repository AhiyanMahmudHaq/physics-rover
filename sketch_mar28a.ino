#include <Servo.h>

// L293D Control Pins
const int motorEnable = 3; // PWM Pin for speed (optional)
const int motorIn1 = 5;    // Logic Forward
const int motorIn2 = 6;    // Logic Backward

// Sensor & Servo Pins
const int trigPin = 9;
const int echoPin = 10;
const int servoPin = 11;

Servo steeringServo;
char command;
bool autoMode = false;

void setup() {
  Serial.begin(9600);
  pinMode(motorIn1, OUTPUT);
  pinMode(motorIn2, OUTPUT);
  pinMode(motorEnable, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  digitalWrite(motorEnable, HIGH); // Set motor to full speed
  steeringServo.attach(servoPin);
  steeringServo.write(90); // Center steering
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
    case 'F': forward(); break;
    case 'B': backward(); break;
    case 'S': stopRover(); break;
    case 'L': steeringServo.write(60);  break; // Turn Left
    case 'R': steeringServo.write(120); break; // Turn Right
    case 'C': steeringServo.write(90);  break; // Center
    case 'W': autoMode = true; break;
    case 'w': autoMode = false; break;
  }
}

void forward() {
  digitalWrite(motorIn1, HIGH);
  digitalWrite(motorIn2, LOW);
}

void backward() {
  digitalWrite(motorIn1, LOW);
  digitalWrite(motorIn2, HIGH);
}

void stopRover() {
  digitalWrite(motorIn1, LOW);
  digitalWrite(motorIn2, LOW);
}

void checkObstacles() {
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  if (distance < 25 && distance > 0) {
    stopRover();
    delay(200);
    backward();
    delay(500);
    stopRover();
  }
}
