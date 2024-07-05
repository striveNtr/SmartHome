#ifndef __SMOKE_INTERFACE__H
#define __SMOKE_INTERFACE__H

#include <pthread.h>
#include <wiringPi.h>
#include "control.h"
#include "global.h"
#include "msq_queue.h"
struct control *add_smoke_to_ctrl_list(struct control *phead);

#endif