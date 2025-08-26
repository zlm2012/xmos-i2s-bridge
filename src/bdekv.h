#ifndef _BDEKV_H_
#define _BDEKV_H_

#ifdef __XC__

#include "i2c.h"

#define BDEKV_ADDR 0x1C

static inline void BDEKV_REGREAD(unsigned reg, unsigned &val, client interface i2c_master_if i2c)
{
    i2c_regop_res_t result;
    val = i2c.read_reg(BDEKV_ADDR, reg, result);
}

static inline void BDEKV_REGWRITE(unsigned reg, unsigned val, client interface i2c_master_if i2c)
{
    i2c.write_reg(BDEKV_ADDR, reg, val);
}

static inline unsigned GetFirAlgo(uint32_t nFs)
{
    unsigned nVal = 0x00;

    switch (nFs)
    {
    case 32000:
    case 44100:
    case 48000:
        nVal = 0x01;
        break;
    case 88200:
    case 96000:
        nVal = 0x02;
        break;
    case 176400:
    case 192000:
        nVal = 0x04;
        break;
    case 352800:
    case 384000:
        nVal = 0x08;
        break;
    }

    return nVal;
}

static inline unsigned GetFirCoef(uint32_t nFs)
{
    unsigned nVal = 0x00;

    switch (nFs)
    {
    case 32000:
    case 44100:
    case 48000:
        nVal = 0x80; // シャープロールオフ
        break;
    case 88200:
    case 96000:
        nVal = 0x01; // シャープロールオフ & 高精度演算オン
        break;
    case 176400:
    case 192000:
        nVal = 0x05; // スローロールオフ & 高精度演算オン
        break;
    case 352800:
    case 384000:
        nVal = 0x80; // FIRバイパス
        break;
    }

    return nVal;
}

static inline void BDEKV_resol(unsigned freq, client interface i2c_master_if i2c) {
    // 04h  Clock 1
    switch(freq)
    {
    case 32000:
        BDEKV_REGWRITE(0x04, 0x03, i2c);
        break;
    case 44100:
    case 48000:
        BDEKV_REGWRITE(0x04, 0x02, i2c);
        break;
    default:
        BDEKV_REGWRITE(0x04, 0x00, i2c);
        break;
    }
    // 06h  Clock 2
    BDEKV_REGWRITE(0x06, 0x00, i2c);
    // 10h  Audio I/F 1
    BDEKV_REGWRITE(0x10, 0x0B, i2c);
    // 30h  FIR Filter 1
    BDEKV_REGWRITE(0x30, GetFirAlgo(freq), i2c);
    //BDEKV_REGWRITE(0x30, 0x08, i2c);
    // 31h  FIR Filter 2
    BDEKV_REGWRITE(0x31, GetFirCoef(freq), i2c);
    // 40h  Delta Sigma
    BDEKV_REGWRITE(0x40, (freq <= 48000)? 0x02 : 0x11, i2c);
    // 60h  Setting 5
    BDEKV_REGWRITE(0x60, 0x16, i2c);
    // 61h  Setting 6
    BDEKV_REGWRITE(0x61, 0x16, i2c);
}

static inline unsigned BDEKV_init(unsigned freq, client interface i2c_master_if i2c) {
    unsigned ver = 0x00;
    BDEKV_REGREAD(0x01, ver, i2c);
    
    BDEKV_REGWRITE(0x04, 0x00, i2c);
    BDEKV_REGWRITE(0x06, 0x01, i2c); // phase adj
    BDEKV_REGWRITE(0x12, 0x00, i2c);
    BDEKV_REGWRITE(0x13, 0x00, i2c);
    BDEKV_REGWRITE(0x14, 0x00, i2c);
    BDEKV_REGWRITE(0x17, 0x00, i2c);

    switch (ver) {
        case 0x52:
            BDEKV_REGWRITE(0x20, 0x7B, i2c);
            break;
        case 0x01:
            BDEKV_REGWRITE(0x20, 0x4B, i2c);
            break;
    }

    BDEKV_REGWRITE(0x21, 0xFF, i2c);
    BDEKV_REGWRITE(0x22, 0xFF, i2c);
    BDEKV_REGWRITE(0x29, 0x08, i2c);
    BDEKV_REGWRITE(0x2A, 0x00, i2c);
    BDEKV_REGWRITE(0x33, 0x00, i2c);
    BDEKV_REGWRITE(0x34, 0x00, i2c);

    switch (ver) {
        case 0x52:
            BDEKV_REGWRITE(0x41, 0x00, i2c);
            BDEKV_REGWRITE(0x42, 0x16, i2c);
            BDEKV_REGWRITE(0x43, 0x2D, i2c);
            BDEKV_REGWRITE(0x48, 0x0D, i2c);
            break;
        case 0x01:
            BDEKV_REGWRITE(0x41, 0x00, i2c);
            BDEKV_REGWRITE(0x42, 0x34, i2c);
            BDEKV_REGWRITE(0x43, 0xB8, i2c);
            BDEKV_REGWRITE(0x48, 0x0D, i2c);
            break;
    }

    BDEKV_resol(freq, i2c);
    BDEKV_REGWRITE(0x21, 0x00, i2c);
    BDEKV_REGWRITE(0x22, 0x00, i2c);
    return ver;
}

static inline void BDEKV_init_reset(client interface i2c_master_if i2c)
{
    // 00h = 01h
    BDEKV_REGWRITE(0x00, 0x01, i2c);
    // 02h = 01h
    BDEKV_REGWRITE(0x02, 0x01, i2c);
    // D0h = 6Ah
    BDEKV_REGWRITE(0xD0, 0x6A, i2c);
    // D3h = 10h
    BDEKV_REGWRITE(0xD3, 0x10, i2c);
    // D3h = 00h
    BDEKV_REGWRITE(0xD3, 0x00, i2c);
    // D0h = 00h
    BDEKV_REGWRITE(0xD0, 0x00, i2c);
}

static inline void BDEKV_analog_pwr(unsigned pwr, client interface i2c_master_if i2c) {
    if (pwr) {
        BDEKV_REGWRITE(0x03, 0x01, i2c);
    } else {
        BDEKV_REGWRITE(0x03, 0x00, i2c);
    }
}

static inline void BDEKV_ram_clear(client interface i2c_master_if i2c) {
    BDEKV_REGWRITE(0x2F, 0x80, i2c);
    BDEKV_REGWRITE(0x2F, 0x00, i2c);
}

static inline void BDEKV_soft_reset(unsigned rst, client interface i2c_master_if i2c) {
    if (rst) {
        BDEKV_REGWRITE(0x02, 0x00, i2c);
        BDEKV_REGWRITE(0x00, 0x00, i2c);
    } else {
        BDEKV_REGWRITE(0x00, 0x01, i2c);
        BDEKV_REGWRITE(0x02, 0x01, i2c);
    }
}

static inline void BDEKV_mute(unsigned mute, client interface i2c_master_if i2c) {
    if (mute) {
        BDEKV_REGWRITE(0x2A, 0x00, i2c);
    } else {
        BDEKV_REGWRITE(0x2A, 0x03, i2c);
    }
}

#endif

#endif
