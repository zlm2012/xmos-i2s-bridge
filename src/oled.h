#ifndef _OLED_H_
#define _OLED_H_

#define OLED_ADDR 0x3C
#define OLED_CMD 0
#define OLED_DATA 0x40

#ifdef __XC__

#include "i2c.h"

static inline void OLED_REGWRITE(unsigned reg, unsigned val, client interface i2c_master_if i2c) {
    i2c.write_reg(OLED_ADDR, reg, val);
}

static inline void OLED_color_invert(unsigned invert, client interface i2c_master_if i2c) {
  if (!invert)
    OLED_REGWRITE(OLED_CMD, 0xA6, i2c); // Normal color
  else
    OLED_REGWRITE(OLED_CMD, 0xA7, i2c); // Inverted color
}

// Screen rotate
static inline void OLED_rotate(unsigned rotated, client interface i2c_master_if i2c) {
  if (rotated) {
      OLED_REGWRITE(OLED_CMD, 0xC0, i2c); // rotated
      OLED_REGWRITE(OLED_CMD, 0xA0, i2c);
  } else {
      OLED_REGWRITE(OLED_CMD, 0xC8, i2c); // normal
      OLED_REGWRITE(OLED_CMD, 0xA1, i2c);
  }
}

static inline void OLED_clear(client interface i2c_master_if i2c)  
{  
  unsigned i, j;       
  for(i = 0; i < 5; i++) {  
    OLED_REGWRITE(OLED_CMD, 0xb0+i, i2c);    // set page addr (0~7)
    OLED_REGWRITE(OLED_CMD, 0x0c, i2c);      // set pos (lo)
    OLED_REGWRITE(OLED_CMD, 0x11, i2c);      // set pos (hi)  
    
    for(j = 0; j < 72; j++)
      OLED_REGWRITE(OLED_DATA, 0, i2c); 
  }
}

static inline void OLED_set_pos(unsigned x, unsigned y, client interface i2c_master_if i2c) 
{ 
  x += 28;
  OLED_REGWRITE(OLED_CMD, 0xB0 + y, i2c);
  OLED_REGWRITE(OLED_CMD, ((x & 0xF0) >> 4) | 0x10, i2c);
  OLED_REGWRITE(OLED_CMD, (x & 0x0F), i2c);
}

static inline void OLED_init(client interface i2c_master_if i2c)
{
  OLED_REGWRITE(OLED_CMD, 0xAE, i2c); /*display off*/
  OLED_REGWRITE(OLED_CMD, 0xD5, i2c); /*set osc division*/
  OLED_REGWRITE(OLED_CMD, 0xF0, i2c);
  OLED_REGWRITE(OLED_CMD, 0xA8, i2c); /*multiplex ratio*/
  OLED_REGWRITE(OLED_CMD, 0x27, i2c); /*duty = 1/40*/
  OLED_REGWRITE(OLED_CMD, 0xD3, i2c); /*set display offset*/
  OLED_REGWRITE(OLED_CMD, 0x00, i2c);
  OLED_REGWRITE(OLED_CMD, 0x40, i2c); /*Set Display Start Line */
  OLED_REGWRITE(OLED_CMD, 0x8d, i2c); /*set charge pump enable*/
  OLED_REGWRITE(OLED_CMD, 0x14, i2c);
  OLED_REGWRITE(OLED_CMD, 0x20, i2c); /*Set page address mode*/
  OLED_REGWRITE(OLED_CMD, 0x02, i2c);
  OLED_REGWRITE(OLED_CMD, 0xA1, i2c); /*set segment remap*/
  OLED_REGWRITE(OLED_CMD, 0xC8, i2c); /*Com scan direction*/
  OLED_REGWRITE(OLED_CMD, 0xDA, i2c); /*set COM pins*/
  OLED_REGWRITE(OLED_CMD, 0x12, i2c);
  OLED_REGWRITE(OLED_CMD, 0xAD, i2c); /*Internal IREF Setting*/
  OLED_REGWRITE(OLED_CMD, 0x30, i2c);
  OLED_REGWRITE(OLED_CMD, 0x81, i2c); /*contract control*/
  OLED_REGWRITE(OLED_CMD, 0x30, i2c); /*128*/
  OLED_REGWRITE(OLED_CMD, 0xD9, i2c); /*set pre-charge period*/
  OLED_REGWRITE(OLED_CMD, 0x22, i2c);
  OLED_REGWRITE(OLED_CMD, 0xdb, i2c); /*set vcomh*/
  OLED_REGWRITE(OLED_CMD, 0x20, i2c);
  OLED_REGWRITE(OLED_CMD, 0xA4, i2c); /*Set Entire Display On/Off*/
  OLED_REGWRITE(OLED_CMD, 0xA6, i2c); /*normal / reverse*/
  OLED_REGWRITE(OLED_CMD, 0x0C, i2c); /*set lower column address*/
  OLED_REGWRITE(OLED_CMD, 0x11, i2c); /*set higher column address*/  
  OLED_clear(i2c);
  OLED_REGWRITE(OLED_CMD, 0xAF, i2c); /*display ON*/ 
}

void OLED_print(unsigned x,unsigned y,const char *chr,unsigned size_y, client interface i2c_master_if i2c);

#endif
#endif
