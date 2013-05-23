#include <Servo.h> 
 
Servo f,e,b;
 
int x = 0,led=10; 
 
void setup() 
{ 
  b.attach(45);
  f.attach(44);
  e.attach(46);
  pinMode(led, OUTPUT);
  
  digitalWrite(led,LOW);
  f.write(120);
  digitalWrite(led,HIGH);
  b.write(110);
} 

 
void loop() 
{ 

 //Forward 
  for(x = 120; x > 0; x -= 1)  
  {  
    digitalWrite(led, HIGH);
    f.write(x);
    //b.write(x);
    delay(7);                       
  }

  //
  for(x = 0; x<=120; x+=1)     // goes from 180 degrees to 0 degrees 
  { 
    digitalWrite(led, LOW);
    f.write(x);
    //b.write(x); 
    delay(7);
  }    
  }

