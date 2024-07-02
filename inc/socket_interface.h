#ifndef __SOCKET_INTERFACE__H
#define __SOCKET_INTERFACE__H
#include <pthread.h>
#include "control.h"
#include "socket.h"
#include "global.h"
#include "msq_queue.h"
struct control *add_tcpsocket_to_ctrl_list(struct control *phead);

#endif