#include <AFMotor.h>

AF_DCMotor motor1(1); // Initialize motor #1 using M1 output
AF_DCMotor motor2(2); // Initialize motor #2 using M2 output

const byte encoder0pinA = 20; // Encoder 0 A pin
const byte encoder0pinB = 40; // Encoder 0 B pin
volatile long duration0 = 0; // Encoder 0 pulse count

const byte encoder1pinA = 21; // Encoder 1 A pin
const byte encoder1pinB = 41; // Encoder 1 B pin
volatile long duration1 = 0; // Encoder 1 pulse count

void setup() {
  Serial.begin(57600); // Start serial communication at 9600 baud rate
  Serial.setTimeout(100); // Increase timeout to ensure command is fully received

  // Initialize motor speed and stop motors
  motor1.setSpeed(0);
  motor1.run(RELEASE);
  motor2.setSpeed(0);
  motor2.run(RELEASE);

  // Setup encoder pins
  pinMode(encoder0pinA, INPUT);
  pinMode(encoder0pinB, INPUT);
  attachInterrupt(digitalPinToInterrupt(encoder0pinA), updateEncoder0, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoder1pinA), updateEncoder1, CHANGE);
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\r'); // Read the incoming command until carriage return
    //Serial.println("Received: " + command); // Debug: Print received command

    if (command.startsWith("m ")) {
      command.remove(0, 2); // Remove 'm ' from the command
      int spaceIndex = command.indexOf(' '); // Find the space separating motor values
      if (spaceIndex == -1) {
        Serial.println("Error: Invalid command format."); // Error handling for invalid command
        return;
      }
      int motor1Speed = command.substring(0, spaceIndex).toInt(); // Extract motor1 speed
      int motor2Speed = command.substring(spaceIndex + 1).toInt(); // Extract motor2 speed

      //Serial.print("Motor1 Speed: "); // Debug: Print motor1 speed
      //Serial.println(motor1Speed);
      //Serial.print("Motor2 Speed: "); // Debug: Print motor2 speed
      //Serial.println(motor2Speed);

      // Set motor speeds and directions
      motor1.setSpeed(abs(motor1Speed));
      motor2.setSpeed(abs(motor2Speed));
      motor1.run(motor1Speed >= 0 ? FORWARD : BACKWARD);
      motor2.run(motor2Speed >= 0 ? BACKWARD : FORWARD);
    } else if (command == "e") {
      // Send back encoder values
      Serial.print(duration0);
      Serial.print(" ");
      Serial.println(duration1);
    }
    // Additional commands can be added here
  }
}

void updateEncoder0() {
  int val = digitalRead(encoder0pinA);
  if (val == HIGH) {
    if (digitalRead(encoder0pinB) == LOW) {
      duration0++; // Forward
    } else {
      duration0--; // Backward
    }
  } else {
    if (digitalRead(encoder0pinB) == LOW) {
      duration0--; // Backward
    } else {
      duration0++; // Forward
    }
  }
}

void updateEncoder1() {
  int val = digitalRead(encoder1pinA);
  if (val == HIGH) {
    if (digitalRead(encoder1pinB) == LOW) {
      duration1--; // Forward
    } else {
      duration1++; // Backward
    }
  } else {
    if (digitalRead(encoder1pinB) == LOW) {
      duration1++; // Backward
    } else {
      duration1--; // Forward
    }
  }
}
