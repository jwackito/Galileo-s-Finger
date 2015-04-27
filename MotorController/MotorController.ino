/*
Arduino Motor Controller
For Arduino-Stellarium integration
Keegan Crankshaw, 2015
Joaquin Bogado, 2015
*/

//Initialize pin variables
const byte xDir = 4;
const byte yDir = 6;

const byte xMotor = 5;
const byte yMotor = 7;

const byte laser = 12;

//The total steps the motor has to make
int tSteps = 0;

//byte array buffer for comunications
byte msg[10];

//Assuming the device has been aligned
int xAbsPos=0;
int yAbsPos=0;

//Setup Loop
void setup(){
  //To link to the python script
    Serial.begin(9600);
    
  //Configure pins
    pinMode(xDir, OUTPUT);
    pinMode(yDir, OUTPUT);
    pinMode(xMotor, OUTPUT);
    pinMode(yMotor, OUTPUT);
    
    pinMode(laser, OUTPUT);
  
  //Set initial direction
    digitalWrite(xDir, LOW);
    digitalWrite(yDir, LOW);
    digitalWrite(laser, HIGH);
}


void loop() {
  int xPos = 0;
  int yPos = 0;
  while(Serial.available()) {
    if(Serial.available() >= 6){
      for (int i = 0; i < 6; i++){
        msg[i] = Serial.read();
      }
      // assuming msg = 'x'byte0byte1'y'byte0byte1
      xPos = msg[1] + (msg[2] << 8);
      yPos = msg[4] + (msg[5] << 8);
      int xSteps = xPos - xAbsPos;
      xAbsPos += xSteps;
      int ySteps = yPos - yAbsPos;
      yAbsPos += ySteps;
      Serial.print(xSteps);
      Serial.print('|');
      Serial.print(ySteps);
      Serial.print('|');
      moveMotors(xSteps, ySteps);
    }
  }
}

void moveMotors(int xSteps, int ySteps){
  bool xdir, ydir;
  digitalWrite(laser,LOW);
  (xSteps < 0) ? xdir = false : xdir = true;
  (ySteps < 0) ? ydir = false : ydir = true;
  digitalWrite(xDir, xdir);
  digitalWrite(yDir, ydir);
  int xRemaining = abs(xSteps);
  int yRemaining = abs(ySteps);
  while (xRemaining || yRemaining){
    if (xRemaining){
      digitalWrite(xMotor, HIGH);
      digitalWrite(xMotor, LOW);
      delay(10);
      //Serial.write("xstep");
      xRemaining--;
    }
    if (yRemaining){
      digitalWrite(yMotor, HIGH);
      digitalWrite(yMotor, LOW);
      delay(10);
      //Serial.write("ystep");
      yRemaining--;
    }
    if(Serial.available() >= 6){
      break;
    }
  }
  digitalWrite(laser,HIGH);
}


