#ifndef __RECEIVE_INTERFACE__H
#define __RECEIVE_INTERFACE__H
#include <pthread.h>
#include "control.h"
#include "global.h"
#include "msq_queue.h"
#include "myoled.h"
#include "face.h"
struct control *add_receive_to_ctrl_list(struct control *phead);

#endif