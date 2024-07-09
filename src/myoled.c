#include <error.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include "oled.h"
#include "font.h"
#include "myoled.h"

#define FILENAME "/dev/i2c-3"

static struct display_info disp;

int oled_show(void *arg)
{
    unsigned char *buffer = (unsigned char *)arg;
    if (buffer != NULL)
    {
        oled_putstrto(&disp, 0, 9 + 1, buffer);
    }
    disp.font = font2;
    oled_send_buffer(&disp);
    return 0;
}

int myoled_init(void)
{
    int e;
    disp.address = OLED_I2C_ADDR;
    disp.font = font2;

    e = oled_open(&disp, FILENAME);
    e = oled_init(&disp);
    oled_clear(&disp);
    return e;
}