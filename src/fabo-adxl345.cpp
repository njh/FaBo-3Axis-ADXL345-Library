#include "fabo-adxl345.h"

bool adxl345::searchDevice()
{
    
  byte device = 0x00;
  readI2c(ADXL345_DEVID_REG, 1, &device);
  
  if(device == ADXL345_DEVICE){
      return true;
  } else{
      return false;
  }
}

void adxl345::powerOn()
{
   byte POWER = ADXL345_AUTO_SLEEP_OFF;
   POWER |= ADXL345_MEASURE_ON;
   POWER |= ADXL345_SLEEP_OFF;
   POWER |= ADXL345_WAKEUP_8HZ;
   Serial.println(POWER,HEX);
   writeI2c(ADXL345_POWER_CTL_REG, POWER);
}

void adxl345::readXYZ(int *x, int *y, int *z)
{
  uint8_t length = 6;
  byte axis_buff[6];
  readI2c(0x32, length, axis_buff); //レジスターアドレス 0x32から6バイト読む
  *x = (((int)axis_buff[1]) << 8) | axis_buff[0];   
  *y = (((int)axis_buff[3]) << 8) | axis_buff[2];
  *z = (((int)axis_buff[5]) << 8) | axis_buff[4];
}

void adxl345::configuration()
{
   byte CONF = ADXL345_SELF_TEST_OFF;
   CONF |= ADXL345_SPI_OFF;
   CONF |= ADXL345_INT_INVERT_OFF;
   CONF |= ADXL345_FULL_RES_OFF;
   CONF |= ADXL345_JUSTIFY_OFF;
   CONF |= ADXL345_RANGE_16G;
   writeI2c(ADXL345_DATA_FORMAT_REG, CONF);
}

// I2Cへの書き込み
void adxl345::writeI2c(byte register_addr, byte value) {
  Wire.begin();       // I2Cの開始
  Wire.beginTransmission(ADXL345_SLAVE_ADDRESS);  
  Wire.write(register_addr);         
  Wire.write(value);                 
  Wire.endTransmission();        
}

// I2Cへの読み込み
void adxl345::readI2c(byte register_addr, int num, byte buffer[]) {
  Wire.begin();       // I2Cの開始
  Wire.beginTransmission(ADXL345_SLAVE_ADDRESS); 
  Wire.write(register_addr);           
  Wire.endTransmission();         

  Wire.beginTransmission(ADXL345_SLAVE_ADDRESS); 
  Wire.requestFrom(ADXL345_SLAVE_ADDRESS, num);   

  int i = 0;
  while(Wire.available())        
  { 
    buffer[i] = Wire.read();   
    i++;
  }
  Wire.endTransmission();         
}

adxl345 fabo3Axis;