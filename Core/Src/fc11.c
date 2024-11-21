#include <stdio.h>

#include "fc11.h"
#include "gpio.h"

static struct dev_info fc11;
static const char hw_version[] = "1.2";
static const char fw_version[] = "2.0";

void fc11_init(void)
{
    fc11.addr = 0x00;
    fc11.pwm = 0x00;
    fc11.versions[VER_HW] = hw_version;
    fc11.versions[VER_FW] = fw_version;

    return;
}

void fc11_address_set(uint8_t address)
{
    fc11.addr = address;

    return;
}

uint8_t fc11_address_get(void)
{
    return fc11.addr;
}

bool fc11_address_confirm(uint8_t address)
{
    return address == fc11.addr;
}

void fc11_out_state_set(uint8_t out_idx, GPIO_PinState state)
{
    if (out_idx >= OUT_NUM)
    {
        return;
    }

    gpio_en_set_state(0x01 << out_idx, state);
    fc11.out_en[out_idx] = state;

    return;
}

int8_t fc11_out_state_get(uint8_t out_idx)
{
    if (out_idx >= OUT_NUM)
    {
        return -1;
    }

    return fc11.out_en[out_idx];
}

void fc11_status_string_get(char *p_buf)
{
    sprintf(
        p_buf, 
        "%d %d %d %d %d ? ? ? ? %d %d\r", 
        fc11.out_rpm[0],
        fc11.out_rpm[1],
        fc11.out_rpm[2],
        fc11.out_rpm[3],
        fc11.out_rpm[4],
        fc11.out_en[6],
        fc11.out_en[7]
        );

    return;
}

void fc11_version_string_get(char *p_buf)
{
    sprintf(p_buf, "%s %s\r", fc11.versions[VER_HW],fc11.versions[VER_FW]);

    return;
}