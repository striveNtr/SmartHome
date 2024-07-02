#ifndef __MSG_QUEUE__H
#define __MSG_QUEUE__H

#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#define QUEQUE_NAME "/test_queue"
mqd_t create_mqd();
void msg_queue_final(mqd_t mqd);
int send_message(mqd_t mqd, void *msg, int msg_len);
#endif 