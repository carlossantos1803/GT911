#include "GT911.h"

GT911::GT911(TwoWire *wire, uint8_t _addr, uint8_t _pin1, uint8_t _pin2):
_wire(wire),addr(_addr),GT911_RESET(_pin1),GT911_INT(_pin2)
{
    _wire->setClock(400000);
    _wire->begin(); 
}

void GT911::gt911setup() {

  delay(300);
  pinMode(GT911_RESET, OUTPUT);
  pinMode(GT911_INT, OUTPUT);
  digitalWrite(GT911_RESET, LOW);
  delay(2);
  digitalWrite(GT911_INT, LOW);
  delay(50);
  digitalWrite(GT911_RESET, HIGH);
  delay(100);
  pinMode     (GT911_INT, INPUT_PULLUP);
  delay(100);

}

//function that writes to the GT911...do not edit
void GT911::writeGT911TouchRegister( uint16_t regAddr, uint8_t *val, uint16_t cnt)
{
  uint16_t i = 0;

  _wire->beginTransmission(addr);
  _wire->write( regAddr >> 8 ); // register 0
  _wire->write( regAddr);  // register 0
  for (i = 0; i < cnt; i++, val++) //data
  {
    _wire->write( *val );  // value
  }
  uint8_t retVal = _wire->endTransmission();

}

uint8_t GT911::readGT911TouchAddr( uint16_t regAddr, uint8_t * pBuf, uint8_t len )
{
  uint8_t i;
  uint8_t returned;
  uint8_t retVal;
  _wire->beginTransmission(addr);
  _wire->write( regAddr >> 8 ); // register 0
  _wire->write( regAddr);  // register 0
  retVal = _wire->endTransmission();

  returned = _wire->requestFrom(addr, len);    // request 1 bytes from slave device #2
  for (i = 0; (i < len) && _wire->available(); i++)

  {
    pBuf[i] = _wire->read();
  }
  return i;
}

////function that works out the touch coordinates for GT911...do not edit
uint8_t GT911::readGT911TouchLocation( TouchLocation * pLoc, uint8_t num )
{
  uint8_t retVal;
  uint8_t i;
  uint8_t k;
  uint8_t  ss[1];
  
  do
  {
    if (!pLoc) break; // must have a buffer
    if (!num)  break; // must be able to take at least one
    ss[0] = 0;

    readGT911TouchAddr( 0x814e, ss, 1);
    uint8_t status = ss[0];

    if ((status & 0x0f) == 0) break; // no points detected
    uint8_t hitPoints = status & 0x0f;
    //Serial.print("number of hit points = ");
    //  Serial.println( hitPoints );
    
    uint8_t tbuf[40];//changed to 40 as that is number called for in  readGT911TouchAddrTest( 0x8150, tbuf, 40);
    uint8_t tbuf1[8];
    readGT911TouchAddr( 0x8150, tbuf, 40);
    readGT911TouchAddr( 0x8150 + 32, tbuf1, 8);

    for (k = 0, i = 0; (i <  4 * 8) && (k < num); k++, i += 8)
    {
      pLoc[k].x = tbuf[i + 1] << 8 | tbuf[i + 0];
      pLoc[k].y = tbuf[i + 3] << 8 | tbuf[i + 2];
    }
    pLoc[k].x = tbuf1[1] << 8 | tbuf1[0];
    pLoc[k].y = tbuf1[3] << 8 | tbuf1[2];

    retVal = hitPoints;

  } while (0);

  ss[0] = 0;
  writeGT911TouchRegister( 0x814e, ss, 1);
	
  delay(2);
  return retVal;
}
