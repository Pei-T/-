#include <SoftwareSerial.h>
#include <string.h>
int A=20,B=21,FR=3;
unsigned short numPos=1024,numTorque=0;
auto time1=micros(), time2=micros();
double torque=0,pos;
byte code[8]={0};
void setup() {
  // put your setup code here, to run once:
  pinMode(A,INPUT_PULLUP);
  pinMode(B,INPUT_PULLUP);
  pinMode(FR,INPUT);
  attachInterrupt(digitalPinToInterrupt(A), addA,RISING);
  attachInterrupt(digitalPinToInterrupt(B), addB,RISING);
  //attachInterrupt(digitalPinToInterrupt(FR),addF,CHANGE);
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println("kaishile!");
  code[0]='a';code[1]='a';code[6]='b';code[7]='\n';
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2);
  pos=0.17578*numPos;
  //编码规则：
  //扭矩编码范围±20Nm，占code[0],code[1].角度加减360°，占code[2],code[3]
  //电压电流保持原编码不变。
  unsigned short _data=(unsigned short)((torque+20)*2000);
  code[2]=(byte)(_data>>8);
  code[3]=(byte)(_data);
  code[4]=(byte)(numPos>>8);
  code[5]=(byte)(numPos); 
  Serial.println(pos);
}
void addA(){numPos+=(digitalRead(B)?-1:+1);}
void addB(){numPos+=(digitalRead(A)?+1:-1);}
void addF() { 
  numTorque++;
  if(numTorque==200){
    time1=micros();
    torque=2000000.0/(time1-time2)-200.2;
    time2=time1;
    numTorque=0;
  }
}
