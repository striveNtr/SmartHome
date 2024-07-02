#ifndef __GDEVICE_H
#define __GDEVICE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
struct gdevice
{
    char dev_name[128];    // 设备名称
    int key;               // key值，用于匹配控制指令的值
    int gpio_pin;          // 控制的gpio引脚
    int gpio_mode;         // 输入输出模式
    int gpio_status;       // 高低电平状态
    int check_face_status; // 是否进行人脸检测状态
    int voice_set_status;  // 是否语音语音播报
    struct gdevice *next;
};
#endif