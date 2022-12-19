#define tPin_l A2
#define tPin_r A3

String v_l;
String v_r;
String status_;
byte wLeftOld = 0;                               // Previous loop whisker values 
byte wRightOld = 1;                              
byte counter = 0; 

void setup()
{ 
  pinMode(tPin_l, INPUT);                         // Set right whisker pin to input
  pinMode(tPin_r, INPUT);                         // Set left whisker pin to input   

  Serial.begin(9600);                        // Set serial data rate to 9600 
}  


void loop()                                  // Main loop auto-repeats
{                                            
  byte wLeft = digitalRead(tPin_l);               // Copy left result to wLeft  
  byte wRight = digitalRead(tPin_r);              // Copy right result to wRight

  if(wLeft == 1)                             // If left whisker contact
  {
    v_l = "Untouch";
  } 
  else                                       // if no left whisker contact
  {
     v_l = "touch";                    // Left LED off
  }

  if(wRight == 1)                            // If right whisker contact
  {
    v_r = "Untouch";                  // Right LED on
  } 
  else                                       // If no right whisker contact
  {
    v_r = "touch";                    // Right LED off
  }
  
  Serial.print("Left Touch Sensor: ");
  Serial.print(v_l);
  Serial.print("   ");
  Serial.print("Right Touch Sensor: ");
  Serial.print(v_r);
  Serial.println("");
  delay(500);                                 // Pause for 50 ms
}

/*
void loop()                                  // Main loop auto-repeats
{

  // Corner Escape

  byte wLeft = digitalRead(tPin_l);               // Copy left result to wLeft  
  byte wRight = digitalRead(tPin_r);              // Copy right result to wRight

  if(wLeft != wRight)                        // One whisker pressed?
  {                                          // Alternate from last time?
    if ((wLeft != wLeftOld) && (wRight != wRightOld))  
    {                                       
      counter++;                             // Increase count by one
      wLeftOld = wLeft;                      // Record current for next rep
      wRightOld = wRight;
      
      if(counter == 4)                       // Stuck in a corner?
      {
        wLeft = 0;                           // Set up for U-turn
        wRight = 0;
        counter = 0;                         // Clear alternate corner count
      }
    }  
    else                                     // Not alternate from last time
    {
      counter = 0;                           // Clear alternate corner count
    }
  }  

  // Whisker Navigation 
  if((wLeft == 0) && (wRight == 0))          // If both whiskers contact
  {
    status_ = "U Turn";
  }
  else if(wLeft == 0)                        // If only left whisker contact
  {
    status_ = "Turn Right";
  }
  else if(wRight == 0)                       // If only right whisker contact
  {
    status_ = "Turn Left";
  }
  else                                       // Otherwise, no whisker contact
  {
    status_ = "Forward";
  }

  Serial.print("Maneuver: ");
  Serial.print(status_);
  Serial.println("");
  Serial.print("count: ");
  Serial.print(counter);
  Serial.println("");
  delay(500);       
}
*/
