#define ENCA 12 // YELLOW
#define ENCB 3 // WHITE
#define pwm 6
#define IN3 10
#define IN4 11
int dir_0 = 1;
volatile int pos = 0; 

void setup()
{ 
  //Pin assign
  pinMode(ENCA,INPUT);
  pinMode(ENCB,INPUT);
  pinMode(pwm,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  //attachInterrupt(1,readEncoder,CHANGE);

}

void loop()
{
  //CW
  digitalWrite(10,HIGH);
  digitalWrite(11,LOW);
  analogWrite(6,100);
  
  digitalWrite(8,HIGH);
  digitalWrite(9,LOW);
  analogWrite(5,100);
}

