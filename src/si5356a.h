#ifndef __SI5356A_H__
#define __SI5356A_H__

#ifdef __XC__

#include "i2c.h"

#define SI5356A_ADDR 0x70
#define SI5356A_OEB_441 0x04
#define SI5356A_OEB_48 0x02
#define SI5356A_OEB_ALL 0x06

static inline void SI5356A_REGWRITE(unsigned reg, unsigned val, client interface i2c_master_if i2c)
{
    i2c.write_reg(SI5356A_ADDR, reg, val);
}

static inline void SI5356A_switch_mclk(unsigned mclk, client interface i2c_master_if i2c)
{
    SI5356A_REGWRITE(230, (mclk % 44100 == 0) ? SI5356A_OEB_441 : SI5356A_OEB_48, i2c);
}

static inline void SI5356A_disable_mclk(client interface i2c_master_if i2c)
{
    SI5356A_REGWRITE(230, SI5356A_OEB_ALL, i2c);
}

static inline void SI5356A_2x_mclk(client interface i2c_master_if i2c)
{
    SI5356A_REGWRITE(230, 0x10, i2c); // OEBall = 1
    SI5356A_REGWRITE(241, 0x65, i2c);

    SI5356A_REGWRITE(64, 0x5B, i2c);
    SI5356A_REGWRITE(65, 0x16, i2c);
    SI5356A_REGWRITE(66, 0xA0, i2c);
    SI5356A_REGWRITE(67, 0x2D, i2c);
    SI5356A_REGWRITE(70, 0xC8, i2c);
    SI5356A_REGWRITE(71, 0x0D, i2c);
    SI5356A_REGWRITE(75, 0x61, i2c);
    SI5356A_REGWRITE(76, 0x14, i2c);
    SI5356A_REGWRITE(81, 0x00, i2c);
    SI5356A_REGWRITE(82, 0x03, i2c);

    SI5356A_REGWRITE(246, 0x02, i2c); // SOFT RESET
    SI5356A_REGWRITE(230, SI5356A_OEB_48, i2c);
}

#endif

#endif
