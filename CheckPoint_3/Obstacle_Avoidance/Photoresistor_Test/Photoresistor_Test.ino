#define phsPin A0

int v_l = 0;
int v_r = 0;
float shade = 0.0;

int pwmLeft = 120;
int pwmRight = 120;

void setup()                                 // Built-in initialization block
{
  pinMode(phsPin, INPUT);

  Serial.begin(9600);                        // Set data rate to 9600 bps
}

/*
-------------------------------------
Large value: Dark
Small value: Bright
-------------------------------------
*/

void loop()                                  // Main loop auto-repeats
{
  v_l = analogRead(phsPin);

  Serial.print("Left Analog: ");                     // Display "A3 = " 
  Serial.println(v_l);
  /*
  Serial.print("   ");
  Serial.print("Left Photo: ");                     // Display "A3 = " 
  Serial.print(volts(v_l));                    // Display measured A3 volts
  Serial.println(" volts");                  // Display " volts" & newline
  Serial.print("Right Analog: ");                     // Display "A3 = " 
  Serial.print(v_r);
  Serial.print("   ");
  Serial.print("Right Photo: ");
  Serial.print(volts(v_r));                 // Display " volts" & newline
  Serial.println(" volts");
  */
  delay(500);                               // Delay for 1 second
}

// ZNED & shade direction decision
/*
void loop()
{
  v_l = analogRead(phsPin_l);
  v_r = analogRead(phsPin_r);
  shade = volts(v_l)/( volts(v_l) + volts(v_r) ) - 0.5;

  if (shade < 0.0)                         // Shade on right?
  {                                          // Slow down left wheel
    pwmLeft = int(120.0 + (shade * 100.0));
    pwmLeft = constrain(pwmLeft, -200, 200);
    pwmRight = 120;                        // Full speed right wheel
  } 
  else                                       // Shade on Left?
  {                                          // Slow down right wheel
    pwmRight = int(120 - (shade * 1000.0));
    pwmRight = constrain(pwmRight, -200, 200);
    pwmLeft = 120;                         // Full speed left wheel
  }  

  Serial.print("Left PWM: ");
  Serial.print(pwmLeft);
  Serial.print("   ");

  Serial.print("ZNED: ");
  Serial.print(shade);
  Serial.print(" volts   ");

  Serial.print("Right PWM: ");
  Serial.print(pwmRight);
  Serial.println("");
  
  delay(300);
}
*/
                                      
float volts(int ad)                       // Measures volts at adPin 
{                                            // Returns floating point voltage
 return (ad * 5.0 / 1024.0); 
}    

