#include <iostream>
#include <wiringSerial.h>
#include <wiringPi.h>
#include <unistd.h>
#include "peit.h"
using namespace std;
const double PI=3.14159;
unsigned timeTick=0;
double Amp=150,T=6000,percent=50,w;
string input,output;
double waveCreator(const int &Amp);
void inputSwitch();
double ABS(double num);
double currentCtrl(double setValue,int model);
int pressureAdjust(int percent);
void serialComulication();
int main(void){
    cout<<"hello"<<endl;
    sysInf();
    inputSwitch();
    w=2*PI/T;
    //~ cout<<"w: "<<w<<endl;
    FILE *stream;
    stream=fopen("/home/pi/Desktop/data.txt","w+");
    //~ thread t1(fresh40);//creat a 40Hz,fresh thread;
    //~ thread t2(fresh1000);//creat a 1000Hz,fresh thread;
    delay(1000);
    //~ pressureAdjust((int) percent);
    int fg=1,i=0,k=0;
    double setPos,lastPos;
    cout<<"into main"<<endl;
    while(fg){
        
        canReceive(0x201);
        fprintf(stream,"%d %0.2f %0.2f %0.2f %0.2f %0.2f %0.2f %0.2f %0.2f ",timeTick,torque,pos,sourceVoltage,sourceCurrent,speed,motorPos,current,temperature);
        delayMicroseconds(800);
        i++;
        if(i==20){
            serialComulication();
            setPos=waveCreator(Amp);
            //~ cout<<setPos<<endl;
            //~ cout<<Amp<<endl;
            if(lastPos<-10&&pos>-10){k++;}
            lastPos=pos; 
            //~ if(k>5){Amp=0;}
            //~ if(speed==0){l++;if(l==100){break;}}
            canSend(0,currentCtrl(setPos,0));
            //~ printf("\r\n");
            //~ printf("tor,%0.2f pos,%0.2f sV,%0.2f sC,%0.2f speed,%0.2f mP,%0.2f current,%0.2f tem,%0.2f ",torque,pos,sourceVoltage,sourceCurrent,speed,motorPos,current,temperature);
            i=0;
            fprintf(stream,"%d %0.2f %0.2f %0.2f %0.2f %0.2f %0.2f %0.2f %0.2f ",timeTick,torque,pos,sourceVoltage,sourceCurrent,speed,motorPos,current,temperature);
            printf("setPos:%0.2f,\tPos:%0.2f\r\n",setPos,pos);
            }   
        }
            
}
unsigned int timeTreat(unsigned int a){
    if(a>2*T){
    a-=2*T;
    timeTreat(a);
    }
    return a;
    }
double wa;
double waveCreator(const int &Amp){
    timeTick=millis();
    //~ timeTick=(timeTick%((int)T))-PI;
    wa=Amp*cos(timeTick/2/T*PI);
    //~ wa=Amp*cos(1.0*w*timeTick);
    //~ wa=300;
    //~ cout<<"waveCreator: "<<1.0*w*timeTick<<endl;
    //~ cout<<"waveCreator: "<<wa<<endl;
    return wa;
    }
void inputSwitch(){
    cout<<"A T P Q"<<endl;
    cin>>input;
    switch(input[0]){
        case 'a':for(int i=0;i<(int)input.size();i++){output[i]=input[i+1];}Amp=stoi(output);break;
        case 't':for(int i=0;i<(int)input.size();i++){output[i]=input[i+1];}T=stoi(output);break;
        case 'p':for(int i=0;i<(int)input.size();i++){output[i]=input[i+1];}percent=stoi(output);break;
        case 'q':break;
        default:cout<<"foll"<<endl;inputSwitch();
        }
    }
int pressureAdjust(int percent){
    printf("into adjustment\n,8:%d",digitalRead(8));
    int k=50000,ij=0;
    double jifen2=0,speedLast=0;
    double setSpeed2=-350;
    //zero
    while(digitalRead(8)==1){
    canReceive2(0x202);
        jifen2+=1.5*((setSpeed2-speed2)-(setSpeed2-speedLast))+0.021*(setSpeed2-speed2);
        if(ABS(jifen2)>1000)
        {
            jifen2=1000;
        }
        speedLast=speed2;
        printf("jifen2:%0.2f",jifen2);
        if(ij==15){
            canSend(1,jifen2);
            ij=0;
            }
        ij++;
        delayMicroseconds(800);
        printf("\n");
        pos2=0;
        }
    canSend(1,0);
    printf("zreo done\n");
    setSpeed2=350;
    while(k--){
        canReceive2(0x202);
        jifen2+=1.5*((setSpeed2-speed2)-(setSpeed2-speedLast))+0.021*(setSpeed2-speed2);
        if(ABS(jifen2)>1000)
        {
            jifen2=1000;}
        speedLast=speed2;
        printf("jifen2:%0.2f",jifen2);
        if(ij==15){
            canSend(1,jifen2);
            ij=0;
            }
        printf("pos2 %0.1f",pos2);
        if((percent-pos2)<0.01)
        {canSend(1,0);
            break;
        }
        ij++;
        delayMicroseconds(800);
        printf("\n");
    }
    //~ scanf("%d",&ij);terriable
    cout<<"done"<<endl;
    return 0;
    }
double ABS(double num){return num>0?num:-num;}
double err=0,sendCurrent2=0,lasterr=0,sendCurrent=0,lastSetValue=0;
double currentCtrl(double setValue,int model)
{
    if(model==0){
        //~ printf(" speederr=%0.0f",err);
        err=setValue-pos;
        //~ sendCurrent2+=2*(err>0?1:-1);
        sendCurrent=10*err+400*(err>0?1:-1);//+sendCurrent2;
        if(ABS(sendCurrent)>6000){
                sendCurrent=6000;
                }
        //~ printf("\r\n");
        //~ printf("sendCurrent=%0.0f",sendCurrent);
        lasterr=err;
        lastSetValue=setValue;
}
return sendCurrent;
}
int d0,d1,d2,d3;
const char command[]={0x11,0x03,0x00,0x00,0x00,0x02,0xc6,0x9b};
char dataSource[10],dataArduino[10];
void serialComulication(){
    write(source,command,8);
    delay(15);
    //~ serialPutchar(arduino,'1');
    delay(15);
    read(source,dataSource,10);
    read(arduino,dataArduino,20);
    for(int i=0;i<20;i++){if(dataArduino[i]=='a'&&dataArduino[i+1]=='a'){
        d0=dataArduino[i+2];
        d1=dataArduino[i+3];
        d2=dataArduino[i+4];
        d3=dataArduino[i+5];
        //~ printf("d0-d3:%x %x %x %x .",d0,d1,d2,d3);
        break;
        }
    }
    sourceVoltage=0.001373291*((int)(dataSource[3]*256+dataSource[4]));
    sourceCurrent=0.0006179801*(int)(dataSource[5]*256+dataSource[4]);
    //~ int c =dataArduino[0]*256;
    //~ cout<<c<<endl;
    //~ d0=dataArduino[0];d1=dataArduino[1];
    torque=(d0*256.0+d1)/2000-20.01;
    //~ cout<<torque<<" "<<pos<<endl;
    //d2=dataArduino[2];d3=dataArduino[3];
    pos=(d2*255.0+d3)/45-360;
    }
