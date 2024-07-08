#include "lrled_gdevice.h"


struct gdevice lrled_gdev = 
{
     .dev_name = "living room led",
     .key = 0x41,
     .gpio_pin = 2,
     .gpio_mode = OUTPUT,
     .gpio_status = HIGH,
     .check_face_status = 0,
     .voice_set_status = 0,
};

struct gdevice *add_lrled_to_gdevice_list(struct gdevice *phead)
{
    return add_device_to_gdevice_list(phead, &lrled_gdev);
}



