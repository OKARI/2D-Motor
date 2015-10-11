#define X_STEP_PIN         15
#define X_DIR_PIN          21
#define X_MIN_PIN          18
#define X_MAX_PIN           -2

#define Y_STEP_PIN         22
#define Y_DIR_PIN          23
#define Y_MIN_PIN          19
#define Y_MAX_PIN          -1

#define X_ENABLE_PIN       14
#define Y_ENABLE_PIN       14

#define SCK_PIN          7
#define MISO_PIN         6
#define MOSI_PIN         5

int MoveX;
int MoveY;
int a = 25;//convert(300,20) to (7500mm,500mm)
int b = 4;//4steps perMM
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
String comdata="";
int Mdirection=0;
int Vbode=100;
int c=0;//needed to reset when the machine is determined
int v=0;

void setup()
{
pinMode(X_STEP_PIN, OUTPUT);
pinMode(X_DIR_PIN, OUTPUT);
pinMode(X_ENABLE_PIN, OUTPUT);
digitalWrite(X_DIR_PIN,HIGH);//needed to reset
pinMode(Y_STEP_PIN, OUTPUT);
pinMode(Y_DIR_PIN, OUTPUT);
pinMode(Y_ENABLE_PIN, OUTPUT);
digitalWrite(Y_DIR_PIN,HIGH);//needed to reset
digitalWrite(X_ENABLE_PIN,LOW);
digitalWrite(Y_ENABLE_PIN,LOW);
Serial.begin(115200);
}
  
void loop()
{
    while(Serial.available()<6)
  {
    digitalWrite(SCK_PIN,LOW);
  }
  for(int i=0;i<6;i++)
  {
    comdata += char(Serial.read());
    delay(2);
  }
  //get data
  
    Mdirection=(comdata[0]-'0');
   if(Mdirection==1)digitalWrite(SCK_PIN,HIGH);
   if(Mdirection==0)digitalWrite(SCK_PIN,LOW);
    for(int j=0;j<3;j++)
    {
     CurrentdestinationX=CurrentdestinationX*10+(comdata[j]-'0');
    }
    for(int k=3;k<6;k++)
    {
     CurrentdestinationY=CurrentdestinationY*10+(comdata[k]-'0');
    }
    if(v==0)
    {
    CurrentdestinationY=CurrentdestinationX=0;
    v++;
    }
  comdata="";
  //transform data
  
  MoveX = a*(CurrentdestinationX - LastdestinationX);
  if(CurrentdestinationX < LastdestinationX) 
  {
    digitalWrite(X_DIR_PIN,LOW);
  }
  MoveY = a*(CurrentdestinationY - LastdestinationY);
  if(CurrentdestinationY < LastdestinationY)
 {
   digitalWrite(Y_DIR_PIN,LOW);
 }
  StepX=abs(b*MoveX);
  StepY=abs(b*MoveY);
  for(int sd=0;sd<6;sd++)
  {if(comdata[sd]-'0'>=10)
      {StepY=StepX=0;
      break;
      }
  }
  //calculate Steps
  
  if(StepX!=0&&StepY!=0)
  {
  if(StepX>StepY)
  {
    c=0;
    Vbode=StepX/StepY*100;
  }
  else
  {
    c=1;
    Vbode=StepY/StepX*100;
  }
  }
  if(StepX==0) c=1;
  if(StepY==0) c=0;  
  //determine direction
  
  nowtime1 = nowtime2 = micros();
 // Cnowtime1 = Cnowtime2 = (double)nowtime1;
 // Serial.println(StepY);used for test
  while(StepX!=0||StepY!=0)
  {if(c==0)
  {while(micros()>=nowtime1&&StepX!=0)
    {
     digitalWrite(X_STEP_PIN,HIGH);
     delayMicroseconds(20);
     digitalWrite(X_STEP_PIN,LOW);
     StepX-=1;
    // Serial.println(StepX);
    nowtime1+=100;
    }
    while(micros()>=nowtime2&&StepY!=0)
     {
      digitalWrite(Y_STEP_PIN,HIGH);
      delayMicroseconds(20);
      digitalWrite(Y_STEP_PIN,LOW);
      StepY-=1;
      nowtime2+=Vbode;
     }
  }
  else
  {
    while(millis()>=nowtime1&&StepY!=0)
    {
     digitalWrite(Y_STEP_PIN,HIGH);
     delayMicroseconds(20);
     digitalWrite(Y_STEP_PIN,LOW);
     StepY-=1;
     nowtime1+=100;
    }
    while(millis()>=nowtime2&&StepX!=0)
     {
     digitalWrite(X_STEP_PIN,HIGH);
     delayMicroseconds(20);
     digitalWrite(X_STEP_PIN,LOW);
      StepX-=1;
      nowtime2+=Vbode;
     }
   }
 }//draw the picture
 
 digitalWrite(X_DIR_PIN,HIGH);
 digitalWrite(Y_DIR_PIN,HIGH);
  LastdestinationX=CurrentdestinationX;
  LastdestinationY=CurrentdestinationY;
 CurrentdestinationX = 0;
 CurrentdestinationY = 0;
// Compare = 1;
  digitalWrite(SCK_PIN,LOW);//reset height
}