#ifndef __CONTROL_H__
#define __CONTROL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct control
{
    char control_name[128];
    int (*init)(void);
    void (*final)(void);
    void *(*get)(void *arg);
    void *(*set)(void *arg);
    struct control *next;
};

//头插法：用于control类链表的创建
struct control *add_device_to_ctrl_list(struct control *phead, struct control *device);

#endif