#include "voice_interface.h"
#include "msq_queue.h"
#include "uartTool.h"
#include <pthread.h>
#include "global.h"

static int serial_fd = -1;

static int voice_init(void)
{
    serial_fd = myserialOpen(SERIAL_DEV, BAUD);
    printf("%s | %s | %d |serial_fd:%d\n", __FILE__, __func__, __LINE__, serial_fd);
    return serial_fd;
}

static void voice_final(void)
{
    if (serial_fd != -1)
    {
        close(serial_fd);
        serial_fd = -1;
    }
}

// 接收语言指令
static void *voice_get(void *arg)
{
    unsigned char buffer[6] = {0xAA, 0x55, 0x00, 0x00, 0x55, 0xAA};
    int len = 0;
    mqd_t mqd = -1;
    ctrl_info_t *ctrl_info = NULL;
    if (arg != NULL)
    {
        ctrl_info = (ctrl_info_t *)arg;
    }
    if (serial_fd == -1)
    {
        serial_fd = voice_init();
        if (serial_fd == -1)
        {
            pthread_exit(0);
        }
    }
    if (ctrl_info != NULL)
    {
        mqd = ctrl_info->mqd;
    }

    if ((mqd_t)-1 == mqd)
    {
        pthread_exit(0);
    }
    pthread_detach(pthread_self());
    printf("%s thread start\n", __func__);
    while (1)
    {
        len = serialGetstring(serial_fd, buffer);
        printf("%s | %s | %d |0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n", __FILE__, __func__, __LINE__, buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5]);
        printf("%s | %s | %d | len:%d\n", __FILE__, __func__, __LINE__, len);
        if (len > 0)
        {
            if (buffer[0] == 0xAA && buffer[1] == 0x55 & buffer[4] == 0x55 && buffer[5] == 0xAA)
            {
                printf("%s | %s | %d |send:0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n", __FILE__, __func__, __LINE__, buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5]);
                send_message(mqd, buffer, len);
            }
        }
    }
    pthread_exit(0);
}
// 语言播报
static void *voice_set(void *arg)
{
    pthread_detach(pthread_self());
    unsigned char *buffer = (unsigned char *)arg;
    if (serial_fd == -1)
    {
        serial_fd = voice_init();
        if (serial_fd == -1)
        {
            pthread_exit(0);
        }
    }
    if (buffer != NULL)
    {
        serialSendstring(serial_fd, buffer, 6);
    }
    pthread_exit(0);
}

struct control voice_control = {
    .control_name = "voice",
    .init = voice_init,
    .final = voice_final,
    .get = voice_get,
    .set = voice_set,
    .next = NULL};

struct control *add_voice_to_ctrl_list(struct control *phead)
{
    return add_interface_to_ctrl_list(phead, &voice_control);
}
