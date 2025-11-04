#ifndef __SI5351A_H__
#define __SI5351A_H__

#ifdef __XC__

#include "i2c.h"
#include "xua_conf.h"

#define SI5351A_ADDR 0x60
#define SI5351A_ENABLE 0x00

static inline void SI5351A_REGWRITE(unsigned reg, unsigned val, client interface i2c_master_if i2c)
{
    i2c.write_reg(SI5351A_ADDR, reg, val);
}

static inline void SI5351A_REGREAD(unsigned reg, unsigned &val, client interface i2c_master_if i2c)
{
    i2c_regop_res_t result;
    val = i2c.read_reg(SI5351A_ADDR, reg, result);
}

static inline void SI5351A_switch_mclk(unsigned lrck, int double_rate, client interface i2c_master_if i2c)
{
#ifndef INIT_ONLY
    if (lrck % 44100 == 0) {
        SI5351A_REGWRITE(26, 0x3D, i2c);
        SI5351A_REGWRITE(27, 0x09, i2c);
        SI5351A_REGWRITE(30, 0x29, i2c);
        SI5351A_REGWRITE(32, 0x00, i2c);
        SI5351A_REGWRITE(33, 0x8F, i2c);
        SI5351A_REGWRITE(53, 0x5E, i2c);
    } else {
        SI5351A_REGWRITE(26, 0x0C, i2c);
        SI5351A_REGWRITE(27, 0x35, i2c);
        SI5351A_REGWRITE(30, 0xB1, i2c);
        SI5351A_REGWRITE(32, 0x0A, i2c);
        SI5351A_REGWRITE(33, 0x5B, i2c);
        SI5351A_REGWRITE(53, 0x3E, i2c);
    }
    if (double_rate) {
        SI5351A_REGWRITE(45, 0x07, i2c);
        if (lrck % 44100 == 0) {
            SI5351A_REGWRITE(46, 0x80, i2c);
        } else {
            SI5351A_REGWRITE(46, 0x00, i2c);
        }
    } else {
        if (lrck % 44100 == 0) {
            SI5351A_REGWRITE(45, 0x11, i2c);
        } else {
            SI5351A_REGWRITE(45, 0x10, i2c);
        }
        SI5351A_REGWRITE(46, 0x00, i2c);
    }
    SI5351A_REGWRITE(177, 0xAC, i2c); // SOFT RESET
#endif
}

static inline void SI5351A_disable_clk(client interface i2c_master_if i2c)
{
#ifndef INIT_ONLY
    SI5351A_REGWRITE(3, 0x07, i2c);
#endif
}

static inline void SI5351A_enable_clk(client interface i2c_master_if i2c)
{
#ifndef INIT_ONLY
    SI5351A_REGWRITE(3, SI5351A_ENABLE, i2c);
#endif
}

static inline void SI5351A_switch_i2s_clk(unsigned lrck, unsigned width, client interface i2c_master_if i2c)
{
#if CODEC_MASTER == 1
    if (width == 16) {
        switch (lrck) {
        case 44100:
            SI5351A_REGWRITE(52, 0x42, i2c);
            SI5351A_REGWRITE(60, 0x01, i2c);
            SI5351A_REGWRITE(61, 0x2E, i2c);
            SI5351A_REGWRITE(62, 0x00, i2c);
            break;
        case 48000:
            SI5351A_REGWRITE(52, 0x42, i2c);
            SI5351A_REGWRITE(60, 0x01, i2c);
            SI5351A_REGWRITE(61, 0x1E, i2c);
            SI5351A_REGWRITE(62, 0x00, i2c);
            break;
        case 88200:
            SI5351A_REGWRITE(52, 0x32, i2c);
            SI5351A_REGWRITE(60, 0x00, i2c);
            SI5351A_REGWRITE(61, 0x96, i2c);
            SI5351A_REGWRITE(62, 0x00, i2c);
            break;
        case 96000:
            SI5351A_REGWRITE(52, 0x32, i2c);
            SI5351A_REGWRITE(60, 0x00, i2c);
            SI5351A_REGWRITE(61, 0x8E, i2c);
            SI5351A_REGWRITE(62, 0x00, i2c);
            break;
        case 176400:
            SI5351A_REGWRITE(52, 0x22, i2c);
            SI5351A_REGWRITE(60, 0x00, i2c);
            SI5351A_REGWRITE(61, 0x4A, i2c);
            SI5351A_REGWRITE(62, 0x00, i2c);
            break;
        case 192000:
            SI5351A_REGWRITE(52, 0x22, i2c);
            SI5351A_REGWRITE(60, 0x00, i2c);
            SI5351A_REGWRITE(61, 0x46, i2c);
            SI5351A_REGWRITE(62, 0x00, i2c);
            break;
        case 352800:
            SI5351A_REGWRITE(52, 0x12, i2c);
            SI5351A_REGWRITE(60, 0x00, i2c);
            SI5351A_REGWRITE(61, 0x24, i2c);
            SI5351A_REGWRITE(62, 0x00, i2c);
            break;
        case 384000:
            SI5351A_REGWRITE(52, 0x12, i2c);
            SI5351A_REGWRITE(60, 0x00, i2c);
            SI5351A_REGWRITE(61, 0x22, i2c);
            SI5351A_REGWRITE(62, 0x00, i2c);
            break;
        case 705600:
            SI5351A_REGWRITE(52, 0x02, i2c);
            SI5351A_REGWRITE(60, 0x00, i2c);
            SI5351A_REGWRITE(61, 0x11, i2c);
            SI5351A_REGWRITE(62, 0x00, i2c);
            break;
        case 768000:
            SI5351A_REGWRITE(52, 0x02, i2c);
            SI5351A_REGWRITE(60, 0x00, i2c);
            SI5351A_REGWRITE(61, 0x10, i2c);
            SI5351A_REGWRITE(62, 0x00, i2c);
            break;
        }
    } else {
        switch (lrck) {
        case 44100:
            SI5351A_REGWRITE(52, 0x42, i2c);
            SI5351A_REGWRITE(60, 0x00, i2c);
            SI5351A_REGWRITE(61, 0x96, i2c);
            SI5351A_REGWRITE(62, 0x00, i2c);
            break;
        case 48000:
            SI5351A_REGWRITE(52, 0x42, i2c);
            SI5351A_REGWRITE(60, 0x00, i2c);
            SI5351A_REGWRITE(61, 0x8E, i2c);
            SI5351A_REGWRITE(62, 0x00, i2c);
            break;
        case 88200:
            SI5351A_REGWRITE(52, 0x32, i2c);
            SI5351A_REGWRITE(60, 0x00, i2c);
            SI5351A_REGWRITE(61, 0x4A, i2c);
            SI5351A_REGWRITE(62, 0x00, i2c);
            break;
        case 96000:
            SI5351A_REGWRITE(52, 0x32, i2c);
            SI5351A_REGWRITE(60, 0x00, i2c);
            SI5351A_REGWRITE(61, 0x46, i2c);
            SI5351A_REGWRITE(62, 0x00, i2c);
            break;
        case 176400:
            SI5351A_REGWRITE(52, 0x22, i2c);
            SI5351A_REGWRITE(60, 0x00, i2c);
            SI5351A_REGWRITE(61, 0x24, i2c);
            SI5351A_REGWRITE(62, 0x00, i2c);
            break;
        case 192000:
            SI5351A_REGWRITE(52, 0x22, i2c);
            SI5351A_REGWRITE(60, 0x00, i2c);
            SI5351A_REGWRITE(61, 0x22, i2c);
            SI5351A_REGWRITE(62, 0x00, i2c);
            break;
        case 352800:
            SI5351A_REGWRITE(52, 0x12, i2c);
            SI5351A_REGWRITE(60, 0x00, i2c);
            SI5351A_REGWRITE(61, 0x11, i2c);
            SI5351A_REGWRITE(62, 0x00, i2c);
            break;
        case 384000:
            SI5351A_REGWRITE(52, 0x12, i2c);
            SI5351A_REGWRITE(60, 0x00, i2c);
            SI5351A_REGWRITE(61, 0x10, i2c);
            SI5351A_REGWRITE(62, 0x00, i2c);
            break;
        case 705600:
            SI5351A_REGWRITE(52, 0x02, i2c);
            SI5351A_REGWRITE(60, 0x00, i2c);
            SI5351A_REGWRITE(61, 0x07, i2c);
            SI5351A_REGWRITE(62, 0x80, i2c);
            break;
        case 768000:
            SI5351A_REGWRITE(52, 0x02, i2c);
            SI5351A_REGWRITE(60, 0x00, i2c);
            SI5351A_REGWRITE(61, 0x07, i2c);
            SI5351A_REGWRITE(62, 0x00, i2c);
            break;
        }
    }
    SI5351A_REGWRITE(177, 0xAC, i2c); // SOFT RESET
#endif
}

static inline unsigned SI5351A_status(client interface i2c_master_if i2c)
{
    unsigned status = 0x00;
    SI5351A_REGREAD(0x00, status, i2c);
    return status;
}

static inline unsigned SI5351A_init(client interface i2c_master_if i2c)
{
    unsigned status = 0x00;
    int i;

    for (i = 0; i < 5; i++) {
        SI5351A_REGREAD(0x00, status, i2c);
        if (status & 0x80) delay_milliseconds(100);
        else break;
    }
    if (i == 5) {
        return 0xFF;
    }

    SI5351A_REGWRITE(3, 0xFF, i2c);
    SI5351A_REGWRITE(16, 0x80, i2c);
    SI5351A_REGWRITE(17, 0x80, i2c);
    SI5351A_REGWRITE(18, 0x80, i2c);

    SI5351A_REGWRITE(0x0002, 0x53, i2c);
	SI5351A_REGWRITE(0x0003, 0x00, i2c);
	SI5351A_REGWRITE(0x0004, 0x20, i2c);
	SI5351A_REGWRITE(0x0007, 0x00, i2c);
	SI5351A_REGWRITE(0x000F, 0x00, i2c);
	SI5351A_REGWRITE(0x0010, 0x0F, i2c);
#if CODEC_MASTER == 1
	SI5351A_REGWRITE(0x0011, 0x0F, i2c);
	SI5351A_REGWRITE(0x0012, 0x0F, i2c);
#else
    SI5351A_REGWRITE(0x0011, 0x8C, i2c);
	SI5351A_REGWRITE(0x0012, 0x8C, i2c);
#endif
	SI5351A_REGWRITE(0x0013, 0x8C, i2c);
	SI5351A_REGWRITE(0x0014, 0x8C, i2c);
	SI5351A_REGWRITE(0x0015, 0x8C, i2c);
	SI5351A_REGWRITE(0x0016, 0x8C, i2c);
	SI5351A_REGWRITE(0x0017, 0x8C, i2c);
	SI5351A_REGWRITE(0x0018, 0x2A, i2c);
	SI5351A_REGWRITE(0x001A, 0x0C, i2c);
	SI5351A_REGWRITE(0x001B, 0x35, i2c);
	SI5351A_REGWRITE(0x001C, 0x00, i2c);
	SI5351A_REGWRITE(0x001D, 0x0F, i2c);
	SI5351A_REGWRITE(0x001E, 0xB1, i2c);
	SI5351A_REGWRITE(0x001F, 0x00, i2c);
	SI5351A_REGWRITE(0x0020, 0x0A, i2c);
	SI5351A_REGWRITE(0x0021, 0x5B, i2c);
	SI5351A_REGWRITE(0x002A, 0x00, i2c);
	SI5351A_REGWRITE(0x002B, 0x01, i2c);
	SI5351A_REGWRITE(0x002C, 0x00, i2c);
	SI5351A_REGWRITE(0x002D, 0x10, i2c);
	SI5351A_REGWRITE(0x002E, 0x00, i2c);
	SI5351A_REGWRITE(0x002F, 0x00, i2c);
	SI5351A_REGWRITE(0x0030, 0x00, i2c);
	SI5351A_REGWRITE(0x0031, 0x00, i2c);
	SI5351A_REGWRITE(0x0032, 0x00, i2c);
	SI5351A_REGWRITE(0x0033, 0x01, i2c);
	SI5351A_REGWRITE(0x0034, 0x42, i2c);
	SI5351A_REGWRITE(0x0035, 0x3E, i2c);
	SI5351A_REGWRITE(0x0036, 0x00, i2c);
	SI5351A_REGWRITE(0x0037, 0x00, i2c);
	SI5351A_REGWRITE(0x0038, 0x00, i2c);
	SI5351A_REGWRITE(0x0039, 0x00, i2c);
	SI5351A_REGWRITE(0x003A, 0x00, i2c);
	SI5351A_REGWRITE(0x003B, 0x01, i2c);
	SI5351A_REGWRITE(0x003C, 0x00, i2c);
	SI5351A_REGWRITE(0x003D, 0x8E, i2c);
	SI5351A_REGWRITE(0x003E, 0x00, i2c);
	SI5351A_REGWRITE(0x003F, 0x00, i2c);
	SI5351A_REGWRITE(0x0040, 0x00, i2c);
	SI5351A_REGWRITE(0x0041, 0x00, i2c);
	SI5351A_REGWRITE(0x005A, 0x00, i2c);
	SI5351A_REGWRITE(0x005B, 0x00, i2c);
	SI5351A_REGWRITE(0x0095, 0x00, i2c);
	SI5351A_REGWRITE(0x0096, 0x00, i2c);
	SI5351A_REGWRITE(0x0097, 0x00, i2c);
	SI5351A_REGWRITE(0x0098, 0x00, i2c);
	SI5351A_REGWRITE(0x0099, 0x00, i2c);
	SI5351A_REGWRITE(0x009A, 0x00, i2c);
	SI5351A_REGWRITE(0x009B, 0x00, i2c);
	SI5351A_REGWRITE(0x00A2, 0x00, i2c);
	SI5351A_REGWRITE(0x00A3, 0x00, i2c);
	SI5351A_REGWRITE(0x00A4, 0x00, i2c);
	SI5351A_REGWRITE(0x00A5, 0x00, i2c);
	SI5351A_REGWRITE(0x00A6, 0x00, i2c);
	SI5351A_REGWRITE(0x00A7, 0x00, i2c);
	SI5351A_REGWRITE(0x00B7, 0x92, i2c);

    SI5351A_REGWRITE(177, 0xAC, i2c); // SOFT RESET
    SI5351A_REGWRITE(3, SI5351A_ENABLE, i2c);

    SI5351A_REGREAD(0x00, status, i2c);
    return status;
}

#ifdef INIT_ONLY
#undef INIT_ONLY
#endif

#endif

#endif
