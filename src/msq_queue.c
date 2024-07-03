// 包含msq_queue.h头文件
#include "msq_queue.h"

// 创建一个mqd_t类型的变量mqd，并初始化为-1
mqd_t create_mqd()
{
    mqd_t mqd = -1;
    // 定义一个mq_attr类型的变量attr，并初始化
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 256;
    attr.mq_curmsgs = 0;
    // 以O_CREAT | O_RDWR模式创建一个消息队列，并返回mqd
    mqd = mq_open(QUEQUE_NAME, O_CREAT | O_RDWR, 0666, &attr);
    // 打印当前文件名、函数名、行号和mqd
    printf("%s | %s | %d |mqd:%d\n", __FILE__, __func__, __LINE__, mqd);
    // 返回mqd
    return mqd;
}

// 删除mqd_t类型的变量mqd
void msg_queue_final(mqd_t mqd)
{
    if (mqd != -1)
    {
        // 关闭mqd
        mq_close(mqd);
        // 删除消息队列
        mq_unlink(QUEQUE_NAME);
        // 将mqd设置为-1
        mqd = -1;
    }
}

// 发送消息
int send_message(mqd_t mqd, void *msg, int msg_len)
{
    int byte_send = -1;
    // 以mqd作为参数，发送消息msg，消息长度为msg_len，等待时间为0
    byte_send = mq_send(mqd, (char *)msg, msg_len, 0);
    // 返回发送的字节数
    return byte_send;
}