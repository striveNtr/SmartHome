#ifndef __RECEIVE_INTERFACE__H
#define __RECEIVE_INTERFACE__H
#include <pthread.h>
#include "control.h"
#include "global.h"
#include "msq_queue.h"
#include "myoled.h"
#include "face.h"
#include "lrled_gdevice.h"
#include "bled_gdevice.h"
#include "fan_gdevice.h"
#include "beep_gdevice.h"
#include "lock_gdevice.h"
struct control *add_receive_to_ctrl_list(struct control *phead);

#endif