#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "main.h"

#define OUT_NUM         8
#define MAG_NUM         2

#define REL_L_PIN_IDX   6
#define REL_M_PIN_IDX   7

enum
{
    VER_HW = 0,
    VER_FW,
    VER_NUM
};

struct dev_info
{
    uint8_t addr;
    uint8_t pwm;
    uint16_t out_rpm[OUT_NUM];
    GPIO_PinState out_en[OUT_NUM];
    uint16_t mag_field[MAG_NUM];
    uint16_t mag_status[MAG_NUM];
    const char *versions[VER_NUM];
};

void fc11_init(void);
void fc11_address_set(uint8_t address);
uint8_t fc11_address_get(void);
bool fc11_address_confirm(uint8_t address);
void fc11_out_state_set(uint8_t out_idx, GPIO_PinState state);
int8_t fc11_out_state_get(uint8_t out_idx);
void fc11_status_string_get(char *p_buf);
void fc11_version_string_get(char *p_buf);