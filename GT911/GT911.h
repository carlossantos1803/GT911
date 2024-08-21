#pragma once
#include <Wire.h>
#include "Arduino.h"

#define GT911_I2C_ADDR_BA_BB    (0x5D | 0x80)  // 0xBA/0xBB - 0x5D (7bit address)
#define GT911_I2C_ADDR_28_29    (0x14 | 0x80)  // 0x28/0x29 - 0x14


struct TouchLocation
{
  uint16_t x;
  uint16_t y;
};

class GT911{
    uint8_t addr;  //CTP IIC ADDRESS
    //Pins
    uint8_t GT911_RESET;   //CTP RESET
    uint8_t GT911_INT;   //CTP  INT
    TwoWire *_wire;
  	  
  public:
    GT911();
    GT911(TwoWire *_wire, uint8_t _addr, uint8_t _pin1, uint8_t _pin2);

    void    gt911setup();    
    void    writeGT911TouchRegister( uint16_t regAddr, uint8_t *val, uint16_t cnt);
    uint8_t readGT911TouchAddr( uint16_t regAddr, uint8_t * pBuf, uint8_t len );
    uint8_t readGT911TouchLocation( TouchLocation * pLoc, uint8_t num );
    
};
