// 包含 smoke_interface.h 和 wiringPi.h 头文件
#include "smoke_interface.h"

// 定义 smoke_pin 常量
#define SMOKE_PIN 6
// 定义 smoke_mode 常量
#define SMOKE_MODE INPUT

// 声明 smoke_init 函数
static int smoke_init(void)
{
    printf("%s|%s|%d\n", __FILE__, __func__, __LINE__);
    // 设置 SMOKE_PIN 为输入模式
    pinMode(SMOKE_PIN, SMOKE_MODE);
    return 0;
}

// 声明 smoke_final 函数
static void smoke_final(void)
{
}

// 声明 smoke_get 函数
static void *smoke_get(void *arg)
{
    int status = HIGH;
    int switch_status = 0;
    unsigned char buffer[6] = {0xAA, 0x55, 0x00, 0x00, 0x55, 0xAA};
    size_t byte_send = -1;
    mqd_t mqd = -1;
    ctrl_info_t *ctrl_info = NULL;
    if (arg != NULL)
    {
        ctrl_info = (ctrl_info_t *)arg;
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
    while (1)
    {
        status = digitalRead(SMOKE_PIN);
        if (status == LOW)
        {
            buffer[2] = 0x45;
            buffer[3] = 0x00;
            switch_status = 1;
            printf("%s|%s|%d: 0x%x, 0x%x,0x%x, 0x%x, 0x%x,0x%x\n", __FILE__,
                   __func__, __LINE__, buffer[0], buffer[1], buffer[2], buffer[3],
                   buffer[4], buffer[5]);
            byte_send = mq_send(mqd, buffer, 6, 0);
            if (byte_send == -1)
            {
                continue;
            }
        }
        else if(status == HIGH && switch_status == 1)
        {
            buffer[2] = 0x45;
            buffer[3] = 0x01;
            switch_status = 0;
            printf("%s|%s|%d: 0x%x, 0x%x,0x%x, 0x%x, 0x%x,0x%x\n", __FILE__,
                   __func__, __LINE__, buffer[0], buffer[1], buffer[2], buffer[3],
                   buffer[4], buffer[5]);
            byte_send = mq_send(mqd, buffer, 6, 0);
            if (byte_send == -1)
            {
                continue;
            }
        }
        sleep(5);
    }
    // 退出线程
    pthread_exit(0);
}

// 声明 smoke_control 结构体
struct control smoke_control = {
    // 结构体名称
    .control_name = "smoke",
    // 初始化函数
    .init = smoke_init,
    // 清理函数
    .final = smoke_final,
    // 获取函数
    .get = smoke_get,
    // 设置函数
    .set = NULL,
    // 下一个指针
    .next = NULL};

// 声明 add_smoke_to_ctrl_list 函数
struct control *add_smoke_to_ctrl_list(struct control *phead)
{
    return add_interface_to_ctrl_list(phead,&smoke_control);
}