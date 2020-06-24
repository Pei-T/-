#ifndef PEIT_H
#define PEIT_H
#include <iostream>
#include <wiringSerial.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <wiringPi.h>
#include <sstream>
#include <wiringSerial.h>
#include <cmath>
#include <unistd.h>
#include <thread>
using namespace std;
extern double torque,pos,sourceVoltage,sourceCurrent;
extern double speed,motorPos,current,temperature,pos2,speed2;
extern int source,arduino;
void sysInf();
int canInf();
void fresh40();
void fresh1000();
void canReceive(int id);
void canReceive2(int id);
void canSend(int id,double sendData);

#endif 
