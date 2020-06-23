//  0{ NC, NC }, 1{ 11, 10 }, 2{  3,  9 }, 3{ 12, 13 }, 4{  8,  2 },
//  5{ NC, NC }, 6{ A2, A3 }, 7{ A6, A1 }, 8{ A7, A0 }, 9{  6,  7 },
//  10{  5,  4 }, 11{ NC, NC }, { NC, NC }, { NC, NC }, { NC, NC },
//  { NC, NC },{ NC, NC },
//#include "AccelStepper.h"
#include <MeOrion.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Servo.h>
int posdata[] = {60, 70};
int dirPin = A7; 
int stpPin = A0;
//int x=0,y=0,w=0; 
MeEncoderMotor motor1(0x09, SLOT1);   //  motor at slot2
MeEncoderMotor motor2(0x09, SLOT2); 
MeEncoderMotor motor3(0x0a, SLOT1); 
MeEncoderMotor motor4(0x0a, SLOT2); 
//AccelStepper stepper(AccelStepper::DRIVER, dirPin, stpPin); 
Me7SegmentDisplay disp(PORT_6);
MeInfraredReceiver redLine(PORT_4);
//MePort port(PORT_6);
Servo myservo1;  // create servo object to control a servo 
Servo myservo2;  // create servo object to control another servo
//int servo1pin =  port.pin1();//旋转attaches the servo on PORT_3 SLOT1 to the servo object
//int servo2pin =  port.pin2();//举升attaches the servo on PORT_3 SLOT2 to the servo object

double a = 19.5, b = 24;
//double w1=0,w2=0,w3=0,w4=0;
int runTime=5;
int serflag1=0,serflag2=0,serflag3=3;
void setup() {
  pinMode(dirPin, OUTPUT);
  pinMode(stpPin, OUTPUT);
//  pinMode(servoupPin, OUTPUT);
//  pinMode(servoholdPin, OUTPUT);
  redLine.begin();
  //Serial.begin(9600);
  disp.display((uint8_t)0, 0x0);
  disp.display((uint8_t)1, 0x0);
  disp.display((uint8_t)2, 0x0);
  disp.display((uint8_t)3, 0x0);
  motor1.begin();
  motor2.begin();
  motor3.begin();
  motor4.begin();
  //stepper.setSpeed(600);
//  stepper.setMaxSpeed(1000); 
//  stepper.setAcceleration(20000); 
  myservo1.attach(mePort[PORT_6].s1);  // attaches the servo on servopin1
  myservo2.attach(mePort[PORT_6].s2);  // attaches the servo on servopin2
  myservo1.write(0);                  // sets the servo position according to the scaled value 
  myservo2.write(90);
}

void loop() {
  delay(50);
  redBack();
//  stepper.run();
  if(serflag1){myservo1.write(0);}
  else{myservo1.write(180);}
  if(serflag2){myservo2.write(90);}
  else{myservo2.write(100);}
  if(serflag3==0){
    digitalWrite(dirPin, HIGH);  
    for (int x = 0; x < 200; x ++) {
    digitalWrite(stpPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stpPin, LOW);
    delayMicroseconds(500);
    }
    serflag3=2;
  }
  if(serflag3==1){
    digitalWrite(dirPin, LOW);   
    for (int x = 0; x < 200; x ++) {
    digitalWrite(stpPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stpPin, LOW);
    delayMicroseconds(500);
    }
    serflag3=2;
  }
}

void redBack() {
  if (redLine.available()) {//Serial.println(13);
    switch (redLine.read()) {
      case IR_BUTTON_UP:      upGo();       break;
      case IR_BUTTON_DOWN:    downGo();     break;
      case IR_BUTTON_LEFT:    leftGO();     break;
      case IR_BUTTON_RIGHT:   rightGo();    break;
      case IR_BUTTON_A:       leftTurn();   break;
      case IR_BUTTON_B:       rightTurn();  break;
      case IR_BUTTON_C:       leftRoll();   break;
      case IR_BUTTON_D:       rightRoll();  break;
      case IR_BUTTON_E:       upRoll();     break;
      case IR_BUTTON_F:       downRoll();   break;
      case IR_BUTTON_1:       xuanzhuan();  break;
      case IR_BUTTON_2:       nizhuan();    break;      
      case IR_BUTTON_SETTING: hold();       break;
      default:                break;
    }
    //delay(2000);
  }
}
void upGo()     {disp.display((uint8_t)0, 0x1);motion(100,00,00);}
void downGo()   {disp.display((uint8_t)0, 0x2);motion(-100,00,00);}
void leftGO()   {disp.display((uint8_t)0, 0x3);motion(00,100,00);}
void rightGo()  {disp.display((uint8_t)0, 0x4);motion(00,-100,00);}
void leftTurn() {disp.display((uint8_t)0, 0xc);motion(00,00,0.8); }
void rightTurn(){disp.display((uint8_t)0, 0xd);motion(00,00,-0.8); }
void leftRoll() {disp.display((uint8_t)0, 0xa);serflag3==0;}
void rightRoll(){disp.display((uint8_t)0, 0xa);serflag3==1;}//stepper.moveTo(200);}
void upRoll()   {disp.display((uint8_t)0, 0xe);serflag2=0;}//myservo2.write(90);}
void downRoll() {disp.display((uint8_t)0, 0xf);serflag2=1;}//myservo2.write(100);}
void hold()     {disp.display((uint8_t)0, 0xa);}
void xuanzhuan(){disp.display((uint8_t)0, 0x1);serflag1=0;}//myservo1.write(0);}
void nizhuan()  {disp.display((uint8_t)0, 0x2);serflag1=1;}//myservo1.write(180);}
void motion(double x, double y, double w) {
  double a = 19.5, b = 24;
  double w1=0,w2=0,w3=0,w4=0;
  int runTime=1;
  w1 = y + x + w*(a + b);
  w2 = y - x + w * (a + b);
  w3 = -y - x+ w * (a + b);
  w4 = -y + x + w * (a + b);
  motor1.runSpeedAndTime((int)w1,runTime);
  motor2.runSpeedAndTime((int)w2,runTime);
  motor3.runSpeedAndTime((int)w3,runTime);
  motor4.runSpeedAndTime((int)w4,runTime);
  //delay(500);
}
