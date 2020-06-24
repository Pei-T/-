#include "peit.h"
struct can_frame frame;
int source,arduino,s,nbytes;
double torque,pos,sourceVoltage,sourceCurrent;
double speed,motorPos=0,current,temperature,lastMiniPos=4096,miniPos;
//~ char dataSource[10],dataArduino[10];
//~ const char command[]={0x11,0x03,0x00,0x00,0x00,0x02,0xc6,0x9b};
void sysInf(){
     wiringPiSetup();
    if((source=serialOpen("/dev/ttyUSB0",9600))<0)
        cout<<"serialSource error!"<<endl;
    else{cout<<"serialSource sucess!"<<endl;}
    if((arduino=serialOpen("/dev/ttyACM2",9600))<0)
        cout<<"serialArduino error!"<<endl;
    else{cout<<"serialArduino sucess!"<<endl;}
    canInf();
}
//~ int d0,d1,d2,d3;
//~ void fresh40(){
    //~ cout<<"fresh40 started!"<<endl;
    //~ while(true){
        //~ write(source,command,8);
        //~ delay(15);
        //~ serialPutchar(arduino,'1');
        //~ delay(15);
        //~ read(source,dataSource,10);
        //~ read(arduino,dataArduino,20);
        //~ for(int i=0;i<10;i++){printf("%x ",dataArduino[i]);}printf("\r\n");
        //~ sourceVoltage=0.001373291*((int)(dataSource[3]*256+dataSource[4]));
        //~ sourceCurrent=0.0006179801*(int)(dataSource[5]*256+dataSource[4]);
        //~ int c =dataArduino[0]*256;
        //~ cout<<c<<endl;
        //~ d0=dataArduino[0];d1=dataArduino[1];
        //~ torque=(d0*256.0+d1)/2000-20.01;
        //~ cout<<torque<<" "<<pos<<endl;
        //~ d2=dataArduino[2];d3=dataArduino[3];
        //~ pos=(d2*256.0+d3)/45-360;
        //~ }
//~ }
void fresh1000(){
    cout<<"fresh1000 started!"<<endl;
    while(true){
    //~ canReceive(0x201);
    canReceive2(0x202);
    delayMicroseconds(800);
    }
}
short sp,cu,po,te;
void canReceive(int id)
{//receive a piece of candata and caculate the speed pos,current,tempureture
 //please open a new thread to run this fuction, the delay time should be less than 1ms
    struct can_filter rfilter[1];
    rfilter[0].can_id = id;
    rfilter[0].can_mask = CAN_SFF_MASK;
    setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));
    //5.Receive data and exit
    while(1) 
    {nbytes = read(s, &frame, sizeof(frame));if(nbytes > 0) {break;}}
    po=frame.data[0]<<8|frame.data[1];
    sp=frame.data[2]<<8|frame.data[3];
    cu=frame.data[4]<<8|frame.data[5];
    te=frame.data[6];
    speed=sp;
    miniPos=po;
    current=cu;
    temperature=te;
    if((miniPos-lastMiniPos)>4096){motorPos+=((miniPos-lastMiniPos)-8192)/6621.86667;}
    else if((miniPos-lastMiniPos)<-4096){motorPos+=((miniPos-lastMiniPos)+8192)/6621.86667;}
    else{motorPos+=(miniPos-lastMiniPos)/6621.86667;}
    lastMiniPos=miniPos;    
}
double speed2,miniPos2,current2,temperature2,pos2,lastMiniPos2;
void canReceive2(int id)
{//receive a piece of candata and caculate the speed pos,current,tempureture
 //please open a new thread to run this fuction, the delay time should be less than 1ms
    //~ printf("into canReceive2");
    struct can_filter rfilter[1];
    rfilter[0].can_id = id;
    rfilter[0].can_mask = CAN_SFF_MASK;
    setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));
    //5.Receive data and exit
    while(1) 
    {
        nbytes = read(s, &frame, sizeof(frame));
        if(nbytes > 0) 
        {
             break;
        }       
    }
   short sp,cu,po,te;
    po=frame.data[0]<<8|frame.data[1];
    sp=frame.data[2]<<8|frame.data[3];
    cu=frame.data[4]<<8|frame.data[5];
    te=frame.data[6];
    speed2=sp;
    miniPos2=po;
    current2=cu;
    temperature2=te;
    //~ printf("speed2 %f\r\n",speed2);
    //~ printf("(miniPos-lastMiniPos)/6621.86667=%f",(miniPos-lastMiniPos)/6621.86667);
    if((miniPos2-lastMiniPos2)>4096)
    {
        pos2+=((miniPos2-lastMiniPos2)-8192)/23347.2;
        //~ printf("8192-(miniPos-lastMiniPos)=%f ",8192-(miniPos-lastMiniPos));
    }
    else if((miniPos2-lastMiniPos2)<-4096)
    {
        pos2+=((miniPos2-lastMiniPos2)+8192)/23347.2;
        //~ printf("(miniPos-lastMiniPos)+8192=%f ",(miniPos-lastMiniPos)+8192);
    }
    else
    {
        pos2+=(miniPos2-lastMiniPos2)/23347.2;
        //~ printf("(miniPos-lastMiniPos)=%f ",(miniPos-lastMiniPos));
    }
    //~ printf("\n");
    lastMiniPos2=miniPos2;    
}
int canInf()
{//creat a can
    
    int ret;
    struct sockaddr_can addr;
    struct ifreq ifr;
    memset(&frame, 0, sizeof(struct can_frame));
    system("sudo ip link set can0 type can bitrate 1000000");
    system("sudo ifconfig can0 up");
    printf("CAN inf success\r\n");
    //1.Create socket
    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (s < 0) {perror("socket PF_CAN failed");return 1;}
     //2.Specify can0 device
    strcpy(ifr.ifr_name, "can0");
    ret = ioctl(s, SIOCGIFINDEX, &ifr);
    if (ret < 0) {perror("ioctl failed"); return 1;}
    //3.Bind the socket to can0
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    ret = bind(s, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0) {perror("bind failed");return 1;}
    //4.Disable filtering rules, do not receive packets, only send
    setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);
    return 0;
}
void canSend(int id,double sendData)
{//send function
    frame.can_id = 0x200;
    frame.can_dlc = 8;
    //~ printf("sendData=%0.2f",sendData);
    short shortsend=(short)sendData;
    char s1=(shortsend>>8)&0x000000ff;
    char s2=shortsend&0x000000ff;
    switch(id){
        case(0):{       
            frame.data[0] =s1;
            frame.data[1] =s2;
            //~ printf(" shortsend=%d ",shortsend);
            //~ printf(" data0=%x data1=%x ",frame.data[0],frame.data[1]);
            frame.data[2] = 0;
            frame.data[3] = 0;
            frame.data[4] = 0;
            frame.data[5] = 0;
            frame.data[6] = 0;
            frame.data[7] = 0;
            }break;
        case(1):{
            frame.data[0] =0;
            frame.data[1] =0;
            //~ printf(" shortsend=%d ",shortsend);
            //~ printf(" data0=%x data1=%x ",frame.data[0],frame.data[1]);
            frame.data[2] = s1;
            frame.data[3] = s2;
            frame.data[4] = 0;
            frame.data[5] = 0;
            frame.data[6] = 0;
            frame.data[7] = 0;
            }
        }
	nbytes = write(s, &frame, sizeof(frame)); 
    if(nbytes != sizeof(frame)) {
        printf("Send Error frame[0]!\r\n");
        system("sudo ifconfig can0 down");
        //delay(100);
   } 
}

