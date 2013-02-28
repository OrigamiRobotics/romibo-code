/************************************************************************************
 * 	
 * 	Name    : MMA8453_n0m1 Library Example: DataMode                       
 * 	Author  : Noah Shibley, Michael Grant, NoMi Design Ltd. http://n0m1.com                       
 * 	Date    : Feb 10th 2012                                    
 * 	Version : 0.1                                              
 * 	Notes   : Arduino Library for use with the Freescale MMA8453Q via i2c. 
 *
 ***********************************************************************************/

#include <I2C.h>
#include <MMA8453_n0m1.h>

MMA8453_n0m1 accel;
int accAddress = 0x1C;

void setup()
{
  Serial.begin(9600);
  accel.setI2CAddr(accAddress); //change your device address if necessary, default is 0x1C
  accel.dataMode(true, 2); //enable highRes 10bit, 2g range [2g,4g,8g]
  accel.regWrite(0x1,0);
  accel.regWrite(0x3,0);
  accel.regWrite(0x5,0);
  Serial.println("MMA8453_n0m1 library");
  Serial.println("XYZ Data Example");
  Serial.println("n0m1.com");
  
  
}

void loop()
{
  accel.update();
  
  Serial.print("x: ");
  Serial.print(accel.x());
  Serial.print(" y: ");
  Serial.print(accel.y());
  Serial.print(" z: ");
  Serial.println(accel.z());

}



