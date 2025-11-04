#include "oled.h"
#include "oledfont.h"

// print a character
// x: 0~127
// y: 0~63         
// size_y: for font choosing. eg.: 8 for 6x8
static void OLED_print_char(unsigned x, unsigned y, const unsigned chr, unsigned size_y, client interface i2c_master_if i2c) {       
  unsigned c = 0, size_x = size_y / 2, temp;
  unsigned font_x;
  if (size_y == 8) font_x = 6;
  else font_x = (size_y / 8 + ((size_y % 8) ? 1 : 0)) * (size_y / 2);
  
  c = chr - ' '; // get font index
  OLED_set_pos(x, y, i2c);
  
  for (unsigned i = 0; i < font_x; i++) {
    if (i % size_x == 0 && size_y != 8) OLED_set_pos(x, y++, i2c);
    if (size_y == 8) {
      temp = asc2_0806[c][i];
    } else if (size_y == 16) {
      temp = asc2_1608[c][i];
    } else if (size_y == 18) {
      temp = asc2_1809[c][i];
    } else return;
    OLED_REGWRITE(OLED_DATA, temp, i2c);
  }
}

void OLED_print(unsigned x,unsigned y,const char *str,unsigned size_y, client interface i2c_master_if i2c) {
  unsigned i = 0;
  while (str[i] != '\0') {   
    OLED_print_char(x, y, str[i++], size_y, i2c);
    if (size_y == 8)
      x += 6;
    else
      x += size_y / 2;
  }
}
