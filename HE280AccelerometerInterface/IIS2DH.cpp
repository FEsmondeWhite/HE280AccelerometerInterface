#include <Wire.h>
#include "IIS2DH.h"

void accelerometer_send(uint8_t val)
{
  Wire.beginTransmission(ACCELEROMETER_I2C_ADDR);
  Wire.write(val);
  if(Wire.endTransmission(false)) {
#if DEBUG_ENABLED
    Serial.println("accelerometer send i2c error.");
#endif // DEBUG_ENABLED
  }
}

void accelerometer_write(uint8_t reg, uint8_t val)
{
  Wire.beginTransmission(ACCELEROMETER_I2C_ADDR);
  Wire.write(reg);
  Wire.write(val);
  if(Wire.endTransmission()) {
#if DEBUG_ENABLED
    Serial.println("accelerometer write i2c error.");
#endif // DEBUG_ENABLED
  }
}

bool accelerometer_recv(uint8_t reg)
{
  uint8_t receiveByte;

  accelerometer_send(reg); //Send an 8bit register to be read
  
  Wire.requestFrom(ACCELEROMETER_I2C_ADDR,1); //Request one 8bit response
  
  if(Wire.available()) 
  {
    receiveByte = Wire.read(); 

#if DEBUG_ENABLED
    Serial.print("accelerometer  value:");
    Serial.println(receiveByte);
#endif // DEBUG_ENABLED

    return true;
  }
  else
  {
    return false;
#if DEBUG_ENABLED
    Serial.println("accelerometer i2c recv error.");
#endif // DEBUG_ENABLED
  }
  return false;
}

void accelerometer_init()
{
#if DEBUG_ENABLED
  Serial.println("iis2dh accelerometer initializing...");
#endif // DEBUG_ENABLED
  Wire.begin(); // join i2c bus
  
  accelerometer_recv(0x0F); //WHO AM I = 0x6A

  accelerometer_recv(0x31); //INT1_SRC (31h)

  //CTRL_REG1 (20h)
  accelerometer_recv(0x20);
  accelerometer_write(0x20,0b10011100); // ODR 5.376kHz in LPMode [7-4]. Low power enable [3]. Z enable [2].
  accelerometer_recv(0x20);


  //CTRL_REG3 (22h)
  accelerometer_recv(0x22);
  accelerometer_write(0x22,0b01000000); // CLICK interrupt on INT1 pin [7]. AOI (And Or Interrupt) on INT1 en [6]. AOI on INT2 en [5].
  accelerometer_recv(0x22);

  //CTRL_REG6 (25h)
  accelerometer_recv(0x25);
  accelerometer_write(0x25,0b000000); //Click interrupt on INT2 pin [7]. Interrupt 1 function enable on INT2 pin [6]. Interrupt 2 on INT2 pin enable [5]. 0=INT Active High [1]. 
  accelerometer_recv(0x25);

  //CTRL_REG4 (23h)
  accelerometer_recv(0x23);
  accelerometer_write(0x23,0b00110000); // Full-scale selection 16G [5-4]. High resolution mode [3].
  accelerometer_recv(0x23);


  //CTRL_REG5 (24h)
  accelerometer_recv(0x24);
  accelerometer_write(0x24,0b01001010); // FIFO enable [6]. Latch INT1 [3]. Latch INT2 until cleared by read [1].
  accelerometer_recv(0x24);
  

  //INT1_CFG (30h)
  accelerometer_recv(0x30);
  accelerometer_write(0x30,0b100000); // ZHI events enabled [5]. ZLO events enabled [4].
  accelerometer_recv(0x30);

  //INT1_SRC (31h)
  accelerometer_recv(0x31);
  
  //INT1_THS (32h)  this is the i2c probe
  accelerometer_recv(0x32);
  accelerometer_write(0x32,Z_PROBE_SENSITIVITY); // 7bits
  accelerometer_recv(0x32);

  //INT1_DURATION (33h)
  accelerometer_recv(0x33);
  accelerometer_write(0x33,0);
  accelerometer_recv(0x33);

  //INT2_CFG (34h)
  accelerometer_recv(0x34);
  accelerometer_write(0x34,0b000000); // ZHI events not enabled on INT2 [5].
  accelerometer_recv(0x34);
  
  //INT2_SRC (35h)
  
  //INT2_THS (36h)
  accelerometer_recv(0x36);
  accelerometer_write(0x36,50); // 7bits
  accelerometer_recv(0x36);

  //INT2_DURATION (37h)
  accelerometer_recv(0x37);
  accelerometer_write(0x37,0);
  accelerometer_recv(0x37);

  
  //CLICK_CFG (38h)
  accelerometer_recv(0x38);
  accelerometer_write(0x38,0b10000); //Single Click Z axis
  accelerometer_recv(0x38);
  
  //CLICK_SRC (39h)
  accelerometer_recv(0x39);
  
  //CLICK_THS (3Ah)
  accelerometer_recv(0x3A);
  accelerometer_write(0x3A,50);
  accelerometer_recv(0x3A);
}

bool accelerometer_status()
{
    bool retValue = true;

#if DEBUG_ENABLED
  Serial.print("iis2dh accelerometer status: ");
#endif // DEBUG_ENABLED

    if(!accelerometer_recv(0x31)) { retValue = false; } //INT1_SRC (31h)
    if(!accelerometer_recv(0x35)) { retValue = false; } //INT1_SRC (31h)
    if(!accelerometer_recv(0x39)) { retValue = false; } //INT1_SRC (31h)
    if(!accelerometer_recv(0x2D)) { retValue = false; } //INT1_SRC (31h)

#if DEBUG_ENABLED
  Serial.println(retValue);
#endif // DEBUG_ENABLED

    return(retValue);
}
