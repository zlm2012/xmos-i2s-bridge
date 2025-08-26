#ifndef _ES9039_H_
#define _ES9039_H_

#ifdef __XC__

#include "i2c.h"
#include "config.es9039.h"

#define ES9039_ADDR 0x48

static inline void ES9039_REGREAD(unsigned reg, unsigned &val, client interface i2c_master_if i2c)
{
    i2c_regop_res_t result;
    val = i2c.read_reg(ES9039_ADDR, reg, result);
}

static inline void ES9039_REGWRITE(unsigned reg, unsigned val, client interface i2c_master_if i2c)
{
    i2c.write_reg(ES9039_ADDR, reg, val);
}

static inline void ES9039_config(int sync, client interface i2c_master_if i2c) {
    // SYSTEM CONFIG (DAC_MODE = 1)
    ES9039_REGWRITE(0x00, 0x02, i2c);
    // SYS MODE CONFIG
    ES9039_REGWRITE(0x01, sync ? 0xF1 : 0xB1, i2c);
    // INPUT SELECTION
    ES9039_REGWRITE(0x39, 0x41, i2c);
    // DAC MUTE
    ES9039_REGWRITE(0x56, 0x00, i2c);
    // FILTER
    ES9039_REGWRITE(0x58, 0xBE, i2c);
    // SPDIF/IIR CONFIG
    ES9039_REGWRITE(0x59, 0x04, i2c);
    // AUTOMUTE OFF
    ES9039_REGWRITE(0x7B, 0x00, i2c);
    // Enforce PCM
    ES9039_REGWRITE(0xF5, 0x00, i2c);
}

static inline void ES9039_resol(unsigned freq, client interface i2c_master_if i2c) {
    //ES9039_REGWRITE(0x04, (freq % 48000 == 0) ? (384000 / freq - 1) : (352800 / freq - 1), i2c);
}

static inline void ES9039_mute(unsigned mute, client interface i2c_master_if i2c) {
    if (mute) {
        ES9039_REGWRITE(0x56, 0x03, i2c);
    } else {
        ES9039_REGWRITE(0x56, 0x00, i2c);
    }
}

#endif

#endif
