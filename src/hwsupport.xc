// Copyright 2017-2025 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <xs1.h>
#include <assert.h>
#include <platform.h>
#include <string.h>

#include "xua.h"

#include "config.h"

#ifdef ENABLE_OLED
#include "oled.h"
#endif

#if CLK_CHIP == CLK_5356
#include "si5356a.h"
#endif
#if CLK_CHIP == CLK_5351
#include "si5351a.h"
#endif

#if DAC_CHIP == DAC_BDEKV
#include "bdekv.h"
#endif
#if DAC_CHIP == DAC_ES9039
#include "es9039.h"
#endif

typedef enum
{
    AUDIOHW_CMD_INIT,
    AUDIOHW_CMD_RESOL,
    AUDIOHW_CMD_EXIT,
    AUDIOHW_CMD_MUTE,
    AUDIOHW_CMD_LED
} audioHwCmd_t;

on tile[1]: port p_i2c_sda = PORT_I2C_SDA1;
on tile[1]: port p_i2c_scl = PORT_I2C_SCL1;
on tile[1]: out port p_en_ctrl = PORT_ENCTRL;
on tile[1]: out buffered port:1 p_freq_led = PORT_FREQ_LED;
on tile[1]: out port p_depth_led = PORT_DEPTH_LED;

static int muted = 0;
static unsigned oled_color = 0;

static inline void audio_init(unsigned freq, client interface i2c_master_if i2c) {
    unsigned status = 0;
#if CLK_CHIP == CLK_5356
    //SI5356A_2x_mclk(i2c);
    SI5356A_switch_mclk(freq, i2c);
#elif CLK_CHIP == CLK_5351
    status = SI5351A_init(i2c);
#endif
    delay_microseconds(10);
    p_en_ctrl <: 1;
    p_freq_led <: 1;
    delay_milliseconds(1);
#ifdef ENABLE_OLED
    OLED_init(i2c);
    OLED_color_invert(oled_color, i2c);
    OLED_rotate(0, i2c);
    OLED_clear(i2c);
    OLED_print(0, 1, "     KHz", 18, i2c);
#endif
#if DAC_CHIP == DAC_BDEKV
    unsigned ver = BDEKV_init(freq, i2c);
    delay_microseconds(10);
    BDEKV_init_reset(i2c);
    delay_microseconds(10);
    BDEKV_analog_pwr(1, i2c);
    BDEKV_ram_clear(i2c);
    BDEKV_mute(muted, i2c);
    if (ver == 0x52)
    p_depth_led <: 1;
    else if (ver == 0) {
        p_depth_led <: 1;
        delay_milliseconds(500);
        p_depth_led <: 0;
    }
    if (status == 0xFF) {
        p_freq_led <: 0;
    }
    if (status & 0x20) {
        delay_milliseconds(1000);
        p_freq_led <: 0;
    }
#elif DAC_CHIP == DAC_ES9039
    ES9039_config(ES9039_SYNC, i2c);
#endif
}

static inline void audio_resol(unsigned freq, unsigned depth, client interface i2c_master_if i2c) {
#ifdef ENABLE_OLED
    char *freq_str;
    switch (freq) {
        case 44100:  freq_str=" 44.1"; break;
        case 48000:  freq_str=" 48.0"; break;
        case 88200:  freq_str=" 88.2"; break;
        case 96000:  freq_str=" 96.0"; break;
        case 176400: freq_str="176.4"; break;
        case 192000: freq_str="192.0"; break;
        case 352800: freq_str="352.8"; break;
        case 384000: freq_str="384.0"; break;
        case 705600: freq_str="705.6"; break;
        case 768000: freq_str="768.0"; break;
    }
    OLED_print(0, 1, freq_str, 18, i2c);
#endif
#if DAC_CHIP == DAC_BDEKV
    BDEKV_soft_reset(1, i2c);
    delay_microseconds(10);
#endif

#if CLK_CHIP == CLK_5356
    SI5356A_disable_mclk(i2c);
#elif CLK_CHIP == CLK_5351
    SI5351A_disable_clk(i2c);
#endif

#if DAC_CHIP == DAC_BDEKV
    delay_microseconds(10);
    BDEKV_resol(freq, i2c);
#endif

#if CLK_CHIP == CLK_5356
    SI5356A_switch_mclk(freq, i2c);
#elif CLK_CHIP == CLK_5351
    SI5351A_switch_mclk(freq, DAC_CHIP == DAC_ES9039, i2c);
    SI5351A_switch_i2s_clk(freq, depth, i2c);
    SI5351A_enable_clk(i2c);
#endif

#if DAC_CHIP == DAC_BDEKV
    delay_milliseconds(500);
    BDEKV_soft_reset(0, i2c);
    BDEKV_ram_clear(i2c);
#endif
}

static inline void audio_mute(unsigned mute, client interface i2c_master_if i2c) {
    muted = mute;
#if DAC_CHIP == DAC_BDEKV
    BDEKV_mute(mute, i2c);
#elif DAC_CHIP == DAC_ES9039
    ES9039_mute(mute, i2c);
#endif
}

[[combinable]]
void AudioHwRemote2(chanend c, client interface i2c_master_if i2c)
{
    unsigned freq = 48000, depth = 32;
    //p_en_ctrl <: 0;
    timer t;
    uint32_t time ;
    unsigned oled_counter;
    const uint32_t period = 100000000;
    while(1)
    {
        select {
            case c :> unsigned cmd:
                if(cmd == AUDIOHW_CMD_INIT)
                {
                    c :> freq;
                    audio_init(freq, i2c);
                    c <: 0;
                }
                else if (cmd == AUDIOHW_CMD_RESOL)
                {
                    unsigned new_freq;
                    c :> new_freq;
                    c :> depth;
                    if (freq != new_freq) {
                        freq = new_freq;
                        audio_resol(freq, depth, i2c);
                    }
                }
                else if (cmd == AUDIOHW_CMD_EXIT)
                {
                    i2c.shutdown();
                    return;
                }
                else if (cmd == AUDIOHW_CMD_MUTE)
                {
                    unsigned mute;
                    c :> mute;
                    audio_mute(mute, i2c);
                    c <: 0;
                }
            break;
            case t when timerafter ( time ) :> void:
#ifdef ENABLE_OLED
                oled_counter++;
                if (oled_counter >= 60) {
                    oled_color = !oled_color;
                    OLED_color_invert(oled_color, i2c);
                    oled_counter = 0;
                }
#endif
                time += period ;
                break;
        }
    }
}

void AudioHwRemote(chanend c)
{

    i2c_master_if i2c[1];
    [[combine]]
    par
    {
        i2c_master(i2c, 1, p_i2c_scl, p_i2c_sda, 100);
        AudioHwRemote2(c, i2c[0]);
    }
}

unsafe chanend uc_audiohw;

static inline void DAC_init(unsigned freq)
{
    unsigned val;
    unsafe
    {
        uc_audiohw <: (unsigned) AUDIOHW_CMD_INIT;
        uc_audiohw <: freq;
        uc_audiohw :> val;
    }
}

static inline void DAC_config(unsigned freq, unsigned depth)
{
    unsafe
    {
        uc_audiohw <: (unsigned) AUDIOHW_CMD_RESOL;
        uc_audiohw <: freq;
        uc_audiohw <: depth;
    }
}

static inline void DAC_mute(unsigned mute)
{
    unsigned val;
    unsafe
    {
        uc_audiohw <: (unsigned) AUDIOHW_CMD_MUTE;
        uc_audiohw <: mute;
        uc_audiohw :> val;
    }
}

/* Configures the external audio hardware for the required sample frequency */
void AudioHwConfig(unsigned samFreq, unsigned mClk, unsigned dsdMode,
    unsigned sampRes_DAC, unsigned sampRes_ADC)
{
    DAC_config(samFreq, sampRes_DAC);
}

void AudioHwInit()
{
    /* wait for LDO */
    delay_milliseconds(1000);

    const int samFreq = 48000;
    DAC_init(samFreq);

    return;
}

void AudioHwConfig_Mute()
{
    DAC_mute(1);
}

void AudioHwConfig_UnMute()
{
    DAC_mute(0);
}

void UserHostActive(int active)
{

}

//:
