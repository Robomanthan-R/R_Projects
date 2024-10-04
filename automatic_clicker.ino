#include <Servo.h>

Servo myServo;  // Create a servo object

int onPosition = 50;     // Servo position for "ON" (adjust as needed)
int offPosition = 80;   // Servo position for "OFF" (adjust as needed)
int servoPin = 13;       // Pin where the servo is connected

void setup() {
  myServo.attach(servoPin);  // Attach the servo to the pin
  myServo.write(offPosition);  // Start with the servo in the "OFF" position
}

void loop() {
  // Move servo to "ON" position
  myServo.write(onPosition);
  delay(2);  // Wait for 1 second
  
  // Move servo to "OFF" position
  myServo.write(offPosition);
  delay(2);  // Wait for 1 second
}
