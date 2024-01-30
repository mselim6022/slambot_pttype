#include <AFMotor.h>

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);

//The sample code for driving one way motor encoder
const byte encoder0pinA = 20;//A pin -> the interrupt pin X
const byte encoder0pinB = 40;//B pin -> the digital pin Y
byte encoder0PinALast;

const byte encoder1pinA = 21;//A pin -> the interrupt pin X
const byte encoder1pinB = 41;//B pin -> the digital pin Y
byte encoder1PinALast;

int duration0;//the number of the pulses
int duration1;

int fwdRotation0; //the number of full wheel rotations forward
int fwdRotation1;
int backRotation0; //the number of full wheel rotations backwards
int backRotation1;

bool Direction0;//the rotation direction
bool Direction1;

bool RwheelFwd;
bool LwheelFwd;

bool bluetoothOverride;
int state;

const float pi = 3.1415;
const float WHEEL_DIAMETER = 6.5;
const float CIRCUMFERENCE = pi * WHEEL_DIAMETER;
const int PULSES_PER_ROTATION = 2 * 960;  //Both pulse rising/falling edge counted 
const float DIST_PER_ROTATION = CIRCUMFERENCE / PULSES_PER_ROTATION;
unsigned long startTime;
unsigned long endTime;
long dist;


#define ledPin 13
//int state = 0;
int counter;

void setup() {
  pinMode(ledPin, OUTPUT);
  //digitalWrite(ledPin, LOW);
  //Serial1.begin(38400); // Default communication rate of the Bluetooth module
  ////Serial.begin(9600);

  //Serial.begin(57600);//Initialize the serial port
  Serial.begin(9600);
  Serial.setTimeout(1);

  EncoderInit();//Initialize the module

  Serial1.begin(9600);
  Serial1.println("Starting Up...");
  
  motor1.setSpeed(200);
	motor1.run(RELEASE);
  motor2.setSpeed(200);
	motor2.run(RELEASE);
  RwheelFwd = false;
  LwheelFwd = false;
  startTime = 0.0;
  endTime = 0.0;
  counter = 0;

  bluetoothOverride = false; //True if regular USB-Serial communication, False for Bluetooth override
  state = 0;
}

void loop() {
  uint8_t i;

  if (duration0 > PULSES_PER_ROTATION || (duration0 < (-PULSES_PER_ROTATION))){
    if (duration0 < 0) {
      backRotation0++;
    } else {
      fwdRotation0++;
    }
    duration0 = 0;
  }

  if ((duration1 > PULSES_PER_ROTATION) || (duration1 < (-PULSES_PER_ROTATION))){
    if (duration1 < 0) {
      backRotation1++;
    } else {
      fwdRotation1++;
    }
    duration1 = 0;
  }
  Serial.write(duration0/2);  //Left Wheel Pulses
  Serial.write(duration1/2);  //Right Wheel Pulses

  if(Serial.available() > 0){ // Retrieves the data from jetson sent over Serial
    int USBstate = Serial.readString().toInt();
    state = USBstate;
  
    updateState();
   
    if (LwheelFwd && RwheelFwd) {           //STRAIGHT FORWARD
      motor1.run(FORWARD);
      motor2.run(FORWARD);
      Serial.println("GO FORWARD");
      delay(1000);
    }
    else if (LwheelFwd && !(RwheelFwd)) {   //TURN LEFT
      motor1.run(FORWARD);
      motor2.run(BACKWARD);
      Serial.println("LEFT TURN");
      delay(1000);
    }
    else if (!(LwheelFwd) && RwheelFwd) {   //TURN RIGHT
      motor1.run(BACKWARD);
      motor2.run(FORWARD);
      Serial.println("RIGHT TURN");
      delay(1000);
    } 
    else if (!(LwheelFwd) && !(RwheelFwd)) {
      motor1.run(RELEASE);
      motor2.run(RELEASE);
      Serial.println("STOP");
      delay(1000);
    }
    else {  //STOP
      motor1.run(RELEASE);
      motor2.run(RELEASE);
      Serial.println("ERROR");
      delay(1000);
    }
  }

  if(Serial1.available() > 0){ // Checks whether data is coming from the Serial1 port
    bluetoothOverride = true;

    int BTstate = Serial1.read(); // Reads the data from the Serial1 port
    
    state = BTstate;
    
    updateState();

    if (LwheelFwd && RwheelFwd) {           //STRAIGHT FORWARD
      motor1.run(FORWARD);
      motor2.run(FORWARD);
    }
    else if (LwheelFwd && !(RwheelFwd)) {   //TURN LEFT
      motor1.run(FORWARD);
      motor2.run(BACKWARD);
    }
    else if (!(LwheelFwd) && RwheelFwd) {   //TURN RIGHT
      motor1.run(BACKWARD);
      motor2.run(FORWARD);
    } 
    else {  //STOP
      motor1.run(RELEASE);
      motor2.run(RELEASE);
    }
    bluetoothOverride = false;

  } else {
    ; // NO BLUETOOTH DATA TRANSMITTED
  }
}

void EncoderInit()
{
  Direction0 = true;  //default -> Forward
  Direction1 = true;
  pinMode(encoder0pinB, INPUT);
  pinMode(encoder1pinB, INPUT);
  attachInterrupt(3, wheelSpeed0, CHANGE);
  attachInterrupt(2, wheelSpeed1, CHANGE);
}

void wheelSpeed0()
{
  int Lstate0 = digitalRead(encoder0pinA);

  if ((encoder0PinALast == LOW) && Lstate0 == HIGH) {
    int val = digitalRead(encoder0pinB);

    if (val == LOW && Direction0) {
      Direction0 = false; //Reverse
    }
    else if (val == HIGH && !Direction0) {
      Direction0 = true;  //Forward
    }
  }
  encoder0PinALast = Lstate0;

  if (!Direction0) {
    duration0++;
  } else {
    duration0--;
  }
}

void wheelSpeed1()
{
  int Lstate1 = digitalRead(encoder1pinA);
 
  if ((encoder1PinALast == LOW) && Lstate1 == HIGH) {
    int val = digitalRead(encoder1pinB);

    if (val == LOW && Direction1) {
      Direction1 = false; //Reverse
    } 
    else if (val == HIGH && !Direction1) {
      Direction1 = true;  //Forward
    }
  }
  encoder1PinALast = Lstate1;

  if (!Direction1) { 
    duration1++;
  } else {
    duration1--;
  }
}

void updateState(){
  switch (state) {
      case '1':  //UP ARROW ↑
          LwheelFwd = true;
          RwheelFwd = true;
        break;

      case '2': //DOWN ARROW ↓
          LwheelFwd = false;
          RwheelFwd = false;
        break;

      case '3': //RIGHT ARROW →
          LwheelFwd = false;
          RwheelFwd = true;
        break;

      case '4': //LEFT ARROW ←
          LwheelFwd = true;
          RwheelFwd = false;
        break;

      default:
          LwheelFwd = false;
          RwheelFwd = false;
          Serial.println("\nInvalid Input...");
          Serial1.println("\nInvalid Input...");
        break;
    }
}