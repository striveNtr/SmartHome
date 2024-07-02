#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "voice_interface.h"
#include "msq_queue.h"
#include "uartTool.h"
#include <pthread.h>


typedef struct
{
    mqd_t mqd;
    struct control *ctrl_phead;
} ctrl_info_t;

#endif