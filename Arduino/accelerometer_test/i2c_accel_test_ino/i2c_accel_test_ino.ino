#include <I2C.h>
//I2c.h is available at:
//http://www.dsscircuits.com/articles/arduino-i2c-master-library.html
//or directley...
//http://dsscircuits.com/images/code/I2C_Rev2.zip

int ACC = 0x1C;
void setup() 
{
  I2c.begin();
  Serial.begin(9600);
}

void loop() 
{
  /*WHO AM I - ASK FOR DEVICE ID DEFAULT: 0x58*/
  //I2c.read(ACC,0x0D,1) ;//who am I
  //Serial.print("I AM:");
  //byte WHO = I2c.receive();
  //Serial.print(WHO,BIN);
  //Serial.print("  ");
  //Serial.println(WHO, DEC);

  /*CNTRL REG 1 - Will tell if Active bit has been set or not*/
  //I2c.read(ACC,0x2A,1) ;
  //Serial.print("CTRL REG 1: ");
  //Serial.println(I2c.receive(), BIN);
  
  /*SYSTEM MODE
    
    00 - Sleep
    01 - Awake
    10 - Standby
    
  */
  
  I2c.read(ACC,0x0B,1) ;//
  //Serial.print("SYS MODE:");
  //Serial.println(I2c.receive(), BIN);
  if(I2c.receive() == 0x00)
  {
    Serial.print("Waking Device");
    I2c.write(ACC,0x2A,0x01);
  }
  else if(I2c.receive() == 0x01)
  {
    Serial.print("AWAKE\n");
  }
  else if(I2c.receive() == 0x02)
  {
    Serial.print("STANDBY\n");
  }
  
  /*X Register*/
  I2c.read(ACC,0x01,1);
  byte X1 = I2c.receive();
  Serial.print("X1:");
  Serial.println(X1, DEC);
 
  /*Y Register*/
  I2c.read(ACC,0x03,1);
  byte Y1 = I2c.receive();
  Serial.print("Y1:");
  Serial.println(Y1, DEC);

  
  /*Z Registers*/
  I2c.read(ACC,0x05,1); 
  byte Z1 = I2c.receive();
  Serial.print("Z1:");
  Serial.println(Z1, DEC);

  delay(100); //Not necessary just so we can see the info
}
