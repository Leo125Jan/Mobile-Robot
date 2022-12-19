// Defination of motors
//------------------------------------------------//

#define NMOTORS 2

class Motor
{
  public:
    Motor(int ENCA_, int ENCB_, int pwmPin_, int inp1_, int inp2_, int S);
    void setMotor(int pwmVal, int pwmPin, int in1, int in2);
    int getENCA(void);
    int getENCB(void);
    int getpwmPin(void);
    int getinp1(void);
    int getinp2(void);
    
  private:
     int ENCA;
     int ENCB;
     int pwmPin;
     int inp1;
     int inp2;
     int side;// 0: left, 1: right
};

Motor::Motor(int ENCA_, int ENCB_, int pwmPin_, int inp1_, int inp2_, int S)
{
  ENCA = ENCA_;
  ENCB = ENCB_;
  pwmPin = pwmPin_;
  inp1 = inp1_;
  inp2 = inp2_;
  side = S;
  
  pinMode(ENCA,INPUT);
  pinMode(ENCB,INPUT);
  pinMode(pwmPin,OUTPUT);
  pinMode(inp1,OUTPUT);
  pinMode(inp2,OUTPUT);
    
  analogWrite(pwmPin,0);
  digitalWrite(inp1,LOW);
  digitalWrite(inp2,LOW);
  
  if (ENCB-2 == 0)
  {
    attachInterrupt(ENCB-2, readEncoder_l, RISING);
  }
  else
  {
    attachInterrupt(ENCB-2, readEncoder_r, RISING);
  }
}

void Motor::setMotor(int pwmVal, int pwmPin, int in1, int in2)
{
  if (pwmVal > 0)
  {
    //Positive
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
    analogWrite(pwmPin,pwmVal);
  }
  else if (pwmVal < 0)
  {
    //Negative
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
    analogWrite(pwmPin,-pwmVal);
  }
  else
  {
    digitalWrite(in1,LOW);
    digitalWrite(in2,LOW);
  }
}

int Motor::getENCA(void)
{
  return ENCA;
}

int Motor::getENCB(void)
{
  return ENCB;
}

int Motor::getpwmPin(void)
{
  return pwmPin;
}

int Motor::getinp1(void)
{
  return inp1;
}

int Motor::getinp2(void)
{
  return inp2;
}

//------------------------------------------------//

// Motor
Motor m[] = {Motor(4,2,6,10,11,0), Motor(12,3,5,8,9,1)};

// Encoder
volatile int pos[2];

// RPS Variable
int posp[2] = {0,0};
int pwm[2] = {0,0};
float e[2] = {0,0};
float u[2] = {0,0};
float ediff[2] = {0,0};
float rpst[2] = {0,0};
float rpsp[2] = {0,0};
int post[2] = {0,0};

// Timer
#define Interval_count 10000

unsigned long currT = 0;
float deltaT = 0.0;

// Control Effort
#define virtual_master_L 2.0
#define virtual_master_R 2.0

double v_ZEND = 0;
const float kpl = 2000;
const float kpr = 2000;
const float kdl = 2.5;
const float kdr = 2.5;

// Main
//------------------------------------------------//

void setup()
{
  currT = micros();

  m[0].setMotor(100, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
  m[1].setMotor(100, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
  
  Serial.begin(9600);
}

void loop()
{
  if ( micros() - currT > Interval_count)
  {
    noInterrupts();
    post[0] = pos[0];
    post[1] = pos[1];
    interrupts();

    deltaT = ((float) Interval_count)/1.0e6;

    rpst[0] = (post[0]-posp[0])/deltaT/969;
    rpst[1] = (post[1]-posp[1])/deltaT/967;
    
    Serial.print("R: ");
    Serial.print(rpst[1]);
    Serial.print(" L: ");
    Serial.print(rpst[0]);
    Serial.println("");

    e[0] = (virtual_master_L - rpst[0]);
    e[1] = (virtual_master_R - rpst[1]);
    ediff[0] = (rpst[0] - rpsp[0])/deltaT;
    ediff[1] = (rpst[1] - rpsp[1])/deltaT;

    posp[0] = post[0];
    posp[1] = post[1];
    rpsp[0] = rpst[0];
    rpsp[1] = rpst[1];

    u[0] = (kpl*e[0] + kdl*ediff[0]);
    u[1] = (kpr*e[1] + kdr*ediff[1]);

    pwm[0] = constrain(pwm[0] + floor(u[0]), -255, 120);
    pwm[1] = constrain(pwm[1] + floor(u[1]), -255, 120);
    m[0].setMotor(pwm[0], m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
    m[1].setMotor(pwm[1], m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());

    currT = micros(); 
  }
}

// IRS
//------------------------------------------------//

void readEncoder_l()
{
  int b = digitalRead(m[0].getENCA());
  int c;
  
  if (b > 0)
  {
    pos[0]++;
  }
  else
  {
    pos[0]--;
  }
}

void readEncoder_r()
{
  int b = digitalRead(m[1].getENCA());
  int c;
  
  if (b > 0)
  {
    pos[1]--;
  }
  else
  {
    pos[1]++;
  }
}
