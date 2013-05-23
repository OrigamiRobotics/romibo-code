#include <Servo.h> 
 
Servo f,e,b;
 
int x = 0,led=10; 
 
void setup() 
{ 
  b.attach(45);
  f.attach(44);
  e.attach(46);
  pinMode(led, OUTPUT);
  
  //Initial position
  digitalWrite(led,LOW);
  f.write(100);
  digitalWrite(led,HIGH);
  b.write(55);
} 

 
void loop() 
{ 
  for(x = 55; x <= 165; x+=1) 
  { 
    digitalWrite(led, LOW);
    b.write(x);
    //e.write(x); 
    delay(7);
  } 

  //LEFT
    for(x = 165; x >= 55; x -= 1)  // goes from 0 degrees to 180 degrees 
  {  
    digitalWrite(led, HIGH);
    b.write(x);
    //e.write(x);
    delay(7);                       // waits 15ms for the servo to reach the xition 
  }
  //
  
  }

