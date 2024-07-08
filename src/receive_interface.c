#include "receive_interface.h"

static int oled_fd = -1;

// 定义一个结构体，用于存储消息长度、缓冲区和控制信息
typedef struct
{
	// 存储消息长度
	int msg_len;
	// 存储缓冲区
	unsigned char *buffer;
	// 存储控制信息
	ctrl_info_t *ctrl_info;
} recv_msg_t;

static int receive_init(void)
{
	//设备类链表添加
	oled_fd = myoled_init();
	face_init();
	return 0;
}

static void receive_final(void)
{
	face_fianl();
	if (oled_fd != -1)
	{
		close(oled_fd);
		oled_fd = -1;
	}
}

static void *handle_device(void *arg)
{
    recv_msg_t *recv_msg = NULL;
	pthread_detach(pthread_self());
	if(arg != NULL)
	{
		recv_msg = (recv_msg_t *)arg;
		printf("recv_msg->msg_len = %d\n", recv_msg->msg_len);
		printf("%s | %s | %d 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n", __FILE__, __func__, __LINE__, 
		recv_msg->buffer[0], recv_msg->buffer[1], recv_msg->buffer[2], recv_msg->buffer[3], 
		recv_msg->buffer[4], recv_msg->buffer[5]);
	}
	pthread_exit(0);
}

// 接收数据的线程函数
static void *receive_get(void *arg)
{
	recv_msg_t *recv_msg = NULL;
	struct mq_attr attr;
	ssize_t recv_len = -1;
	pthread_t tid = -1;
	char *buffer = NULL;
	
	if (arg != NULL)
	{
		recv_msg = (recv_msg_t *)malloc(sizeof(recv_msg_t));
		recv_msg->ctrl_info = (ctrl_info_t *)arg;
		recv_msg->msg_len = -1;
		recv_msg->buffer = NULL;
	}
	else
	{
		pthread_exit(0);
	}
	if (mq_getattr(recv_msg->ctrl_info->mqd, &attr) == -1)
	{
		pthread_exit(0);
	}
	recv_msg->buffer = (unsigned char *)malloc(attr.mq_msgsize);
	buffer = (unsigned char *)malloc(attr.mq_msgsize);
	memset(recv_msg->buffer, 0, attr.mq_msgsize);
	memset(buffer, 0, attr.mq_msgsize);
	pthread_detach(pthread_self());
	while (1)
	{
		recv_len = mq_receive(recv_msg->ctrl_info->mqd, buffer, attr.mq_msgsize, NULL);
		printf("%s|%s|%d: 0x%x, 0x%x,0x%x, 0x%x, 0x%x,0x%x\n", __FILE__,
				   __func__, __LINE__, buffer[0], buffer[1], buffer[2], buffer[3],
				   buffer[4], buffer[5]);
		printf("%s|%s|%d: recv_len = %ld\n", __FILE__, __func__, __LINE__, recv_len);
		if (recv_len == -1)
		{
			if (errno == EAGAIN)
			{
				printf("queue is empty\n");
			}
			else
			{
				break;
			}
		}
		else if(buffer[0] == 0xAA && buffer[1] == 0x55 && buffer[4] == 0x55 && buffer[5] == 0xAA)
		{ 
			recv_msg->msg_len = recv_len;
			memcpy(recv_msg->buffer, buffer, recv_len);
			pthread_create(&(tid),NULL,handle_device,(void *)recv_msg);
		}
	}
	pthread_exit(0);
}

struct control receive_control = {
	.control_name = "receive",
	.init = receive_init,
	.final = receive_final,
	.get = receive_get,
	.set = NULL,
	.next = NULL};

struct control *add_receive_to_ctrl_list(struct control *phead)
{
	return add_interface_to_ctrl_list(phead, &receive_control);
}
