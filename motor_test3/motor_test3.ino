#include <Timers.h>
#include <interrupt.h>

#define X_STEP_PIN         15
#define X_DIR_PIN          21

#define Y_STEP_PIN         22
#define Y_DIR_PIN          23

#define X_ENABLE_PIN       14
#define Y_ENABLE_PIN       14

#define SCK_PIN          7
#define MISO_PIN         6
#define MOSI_PIN         5

int act1 = 0;
int act2 = 0;
int MoveX;
int MoveY;
int a = 100;//convert(300,20) to (20.3mm,15.2mm)
int b = 100;//100steps perMM
int Vmax = 1000;
int CurrentdestinationX = 0;
int CurrentdestinationY = 0;
int LastdestinationX = 0;
int LastdestinationY=0;
int SumX;
int SumY;
int StepX;
int StepY;
unsigned long nowtime1;
unsigned long nowtime2;

void TimeInterrupt1()
{
  act1=1;
}

void TimeInterrupt2()
{
  act2=1;
}
 
void setup()
{
pinMode(X_STEP_PIN, OUTPUT);
pinMode(X_DIR_PIN, OUTPUT);
pinMode(X_ENABLE_PIN, OUTPUT);
digitalWrite(X_DIR_PIN,HIGH);
pinMode(Y_STEP_PIN, OUTPUT);
pinMode(Y_DIR_PIN, OUTPUT);
pinMode(Y_ENABLE_PIN, OUTPUT);
digitalWrite(Y_DIR_PIN,HIGH);
digitalWrite(X_ENABLE_PIN,LOW);
digitalWrite(Y_ENABLE_PIN,LOW);
Serial.begin(9600);
}

void loop()
{digitalWrite(MISO_PIN,HIGH);
int i;
CurrentdestinationX = 10;
CurrentdestinationY = 20;
MoveX = a*(CurrentdestinationX - LastdestinationX);
if(CurrentdestinationX < LastdestinationX) digitalWrite(X_DIR_PIN,LOW);
MoveY = a*(CurrentdestinationY - LastdestinationY);
if(CurrentdestinationY < LastdestinationY) digitalWrite(Y_DIR_PIN,LOW);
StepX=abs(b*MoveX);
StepY=abs(b*MoveY);
int c=1;
if(StepX>StepY) c=0;
TCs.AddFunc(0,TimeInterrupt1,1);
TCs.AddFunc(1,TimeInterrupt2,3);
while(StepX!=0||StepY!=0)
  {
   while(act1==1)
    {
     digitalWrite(X_STEP_PIN,HIGH);
     for(i=0;i<100;i++)
     {
       digitalWrite(SCK_PIN,HIGH);
     }
     digitalWrite(X_STEP_PIN,LOW);
     StepX-=1;
    }
    while(act2==1)
     {
      digitalWrite(Y_STEP_PIN,HIGH);
      for(i=0;i<100;i++)
     {
       digitalWrite(SCK_PIN,HIGH);
     }
      digitalWrite(Y_STEP_PIN,LOW);
      StepY-=1;
     }
   
  }
}
