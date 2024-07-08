#include "fan_gdevice.h"


struct gdevice fan_gdev = 
{
     .dev_name = "fan",
     .key = 0x43,
     .gpio_pin = 7,
     .gpio_mode = OUTPUT,
     .gpio_status = HIGH,
     .check_face_status = 0,
     .voice_set_status = 0,
};

struct gdevice *add_fan_to_gdevice_list(struct gdevice *phead)
{
    return add_device_to_gdevice_list(phead, &fan_gdev);
}



