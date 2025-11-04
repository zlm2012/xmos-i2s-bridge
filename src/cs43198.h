#ifndef _CS43198_H_
#define _CS43198_H_

#ifdef __XC__

#include "i2c.h"

#define CS43198_ADDR 0x30

static inline void CS43198_REGREAD(unsigned reg, unsigned &val, client interface i2c_master_if i2c)
{
    i2c_regop_res_t result;
    val = i2c.read_reg8_addr16(CS43198_ADDR, reg, result);
}

static inline void CS43198_REGWRITE(unsigned reg, unsigned val, client interface i2c_master_if i2c)
{
    i2c.write_reg8_addr16(CS43198_ADDR, reg, val);
}

static inline unsigned CS43198_freq_idx(unsigned freq) {
    switch (freq) {
        case 32000:
        return 0;
        case 44100:
        return 1;
        case 48000:
        return 2;
        case 88200:
        return 3;
        case 96000:
        return 4;
        case 176400:
        return 5;
        case 192000:
        return 6;
        case 352800:
        return 7;
        case 384000:
        return 8;
    }
    return 8;
}

static inline void CS43198_powerup(unsigned freq, unsigned depth, client interface i2c_master_if i2c) {
    // ASP Sample rate
    CS43198_REGWRITE(0x1000B, CS43198_freq_idx(freq), i2c);
    // ASP Bit size
    CS43198_REGWRITE(0x1000C, (depth == 32) ? 0x04 : 0x06, i2c);
    // ASP Clock (Slave, SCPOL IN/OUT = 1)
    CS43198_REGWRITE(0x40018, 0x0c, i2c);
    // ASP Frame
    CS43198_REGWRITE(0x40019, 0x0a, i2c);
    // ASP Ch.1 Location
    CS43198_REGWRITE(0x50000, 0x00, i2c);
    // ASP Ch.2 Location
    CS43198_REGWRITE(0x50001, 0x00, i2c);
    // ASP Ch.1 Size / Enable
    CS43198_REGWRITE(0x5000A, 0x07, i2c);
    // ASP Ch.2 Size / Enable
    CS43198_REGWRITE(0x5000B, 0x0F, i2c);
    // PCM Filter
    CS43198_REGWRITE(0x90000, 0x02, i2c);
    // PCM Vol. B
    CS43198_REGWRITE(0x90001, 0x00, i2c);
    // PCM Vol. A
    CS43198_REGWRITE(0x90002, 0x00, i2c);
    // PCM Sig Path Ctrl
    CS43198_REGWRITE(0x90003, 0xec, i2c);
    CS43198_REGWRITE(0x90004, 0x00, i2c);
    // Class H
    CS43198_REGWRITE(0xB0000, 0x1e, i2c);
    // Analog output ctrl
    CS43198_REGWRITE(0x80000, 0x30, i2c);
    // Headphone Detect
    CS43198_REGWRITE(0xD0000, 0x24, i2c);
    // Clock Select
    CS43198_REGWRITE(0x10006, (CS43198_freq_idx(freq) % 2) * 4, i2c);
}

static inline void CS43198_pd(client interface i2c_master_if i2c, void (*sleep_us)(int)) {
    // Power down amplifier
    unsigned val = 0x00;
    CS43198_REGREAD(0x20000, val, i2c);
    CS43198_REGWRITE(0x20000, val | 0x10, i2c);
    val = 0;
    while (val & 1 == 0) {
        sleep_us(10);
        CS43198_REGREAD(0xF0000, val, i2c);
    }
    // Power down ASP
    unsigned val = 0x00;
    CS43198_REGREAD(0x20000, val, i2c);
    CS43198_REGWRITE(0x20000, val | 0x40, i2c);
}

static inline void CS43198_switch_resol(client interface i2c_master_if i2c, void (*sleep_us)(int)) {
    // MCLK -> RCO
    CS43198_REGWRITE(0x10006, 0x06, i2c);
    sleep_us(150);

}

static inline void CS43198_conf(unsigned freq, unsigned depth, client interface i2c_master_if i2c) {
    // ASP Sample rate
    CS43198_REGWRITE(0x1000B, CS43198_freq_idx(freq), i2c);
    // ASP Bit size
    CS43198_REGWRITE(0x1000C, (depth == 32) ? 0x04 : 0x06, i2c);
    // ASP Clock (Slave, SCPOL IN/OUT = 1)
    CS43198_REGWRITE(0x40018, 0x0c, i2c);
    // ASP Frame
    CS43198_REGWRITE(0x40019, 0x0a, i2c);
    // ASP Ch.1 Location
    CS43198_REGWRITE(0x50000, 0x00, i2c);
    // ASP Ch.2 Location
    CS43198_REGWRITE(0x50001, 0x00, i2c);
    // ASP Ch.1 Size / Enable
    CS43198_REGWRITE(0x5000A, 0x07, i2c);
    // ASP Ch.2 Size / Enable
    CS43198_REGWRITE(0x5000B, 0x0F, i2c);
    // PCM Filter
    CS43198_REGWRITE(0x90000, 0x02, i2c);
    // PCM Vol. B
    CS43198_REGWRITE(0x90001, 0x00, i2c);
    // PCM Vol. A
    CS43198_REGWRITE(0x90002, 0x00, i2c);
    // PCM Sig Path Ctrl
    CS43198_REGWRITE(0x90003, 0xec, i2c);
    CS43198_REGWRITE(0x90004, 0x00, i2c);
    // Class H
    CS43198_REGWRITE(0xB0000, 0x1e, i2c);
    // Analog output ctrl
    CS43198_REGWRITE(0x80000, 0x30, i2c);
    // Headphone Detect
    CS43198_REGWRITE(0xD0000, 0x24, i2c);
    // Clock Select
    CS43198_REGWRITE(0x10006, (CS43198_freq_idx(freq) % 2) * 4, i2c);
}

#endif

#endif
