#include "GT911.h"

#define TOUCH_SDA      20
#define TOUCH_SCL      21
#define TOUCH_INT      22
#define TOUCH_RST      23
#define TOUCH_ADDR     0x5D

#ifndef __WIRE0_DEVICE
#define __WIRE0_DEVICE i2c0
#endif

TwoWire Wire10(__WIRE0_DEVICE, TOUCH_SDA, TOUCH_SCL);

GT911 touch(&Wire10, TOUCH_ADDR, TOUCH_RST, TOUCH_INT);
bool istouch = false;

void setup() {
  // put your setup code here, to run once:
  touch.gt911setup();
    
  attachInterrupt(digitalPinToInterrupt(TOUCH_INT),[](){istouch = true;}, FALLING);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(istouch)
  {
    TouchLocation touchLocations[5] = {0,0};
    uint8_t points = touch.readGT911TouchLocation( touchLocations, 5 ); 
    for(uint8_t i = 0; i < points; i++)
    {
      Serial.printf("Point %d {%d,%d}\n", i, touchLocations[i].x, touchLocations[i].y);
    }
    istouch = false;
  }
}
