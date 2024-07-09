#include "receive_interface.h"

static int oled_fd = -1;
static struct gdevice *pdevhead = NULL;

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
	// 设备类链表添加
	pdevhead = add_lrled_to_gdevice_list(pdevhead); // 客厅灯
	pdevhead = add_bled_to_gdevice_list(pdevhead);	// 卧室灯
	pdevhead = add_fan_to_gdevice_list(pdevhead);	// 风扇
	pdevhead = add_beep_to_gdevice_list(pdevhead);	// 蜂鸣器
	pdevhead = add_lock_to_gdevice_list(pdevhead);	// 门锁
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
	struct gdevice *cur_gdev = NULL;
	char success_or_failed[20] = "success";
	int ret = -1;
	pthread_t tid = -1;
	int somke_status = 0;
	double face_result = 0.0;
	pthread_detach(pthread_self());
	if (arg != NULL)
	{
		recv_msg = (recv_msg_t *)arg;
		printf("recv_msg->msg_len = %d\n", recv_msg->msg_len);
		printf("%s | %s | %d 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n", __FILE__, __func__, __LINE__,
			   recv_msg->buffer[0], recv_msg->buffer[1], recv_msg->buffer[2], recv_msg->buffer[3],
			   recv_msg->buffer[4], recv_msg->buffer[5]);
	}

	if (recv_msg != NULL && recv_msg->buffer != NULL)
	{
		cur_gdev = find_gdevice_by_key(pdevhead, recv_msg->buffer[2]);
	}
	printf("%s | %s | %d \n", __FILE__, __func__, __LINE__);
	if (cur_gdev != NULL)
	{
		cur_gdev->gpio_status = recv_msg->buffer[3] == 0 ? LOW : HIGH;
		printf("%s|%s|%d:cur_gdev->check_face_status=%d\n", __FILE__, __func__,
			   __LINE__, cur_gdev->check_face_status);
		// 锁处理
		if (cur_gdev->check_face_status == 1)
		{
			face_result = face_category();
			printf("%s|%s|%d:face_result=%f\n", __FILE__, __func__, __LINE__, face_result);
			if (face_result > 0.6)
			{
				ret = set_gpio_gdevice_status(cur_gdev);
				recv_msg->buffer[2] = 0x47;
			}
			else
			{
				recv_msg->buffer[2] = 0x46;
				ret = -1;
			}
		}
		else if (cur_gdev->check_face_status == 0)
		{
			ret = set_gpio_gdevice_status(cur_gdev);
		}
		printf("%s | %s | %d \n", __FILE__, __func__, __LINE__);
		// 语音播报
		if (cur_gdev->voice_set_status == 1)
		{
			// printf("%s|%s|%d:cur_gdev->voice_set_status=%d\n",__FILE__, __func__, __LINE__,cur_gdev->voice_set_status);
			if (recv_msg != NULL && recv_msg->ctrl_info != NULL && recv_msg->ctrl_info->ctrl_phead != NULL)
			{
				struct control *pcontrol = recv_msg->ctrl_info->ctrl_phead;
				while (pcontrol != NULL)
				{
					// printf("%s|%s|%d:pcontrol->control_name=%s\n",__FILE__, __func__, __LINE__,pcontrol->control_name);
					if (strstr(pcontrol->control_name, "voice"))
					{
						if (recv_msg->buffer[2] == 0x45 && recv_msg->buffer[3] == 0)
						{
							somke_status = 1;
						}
						pthread_create(&tid, NULL, pcontrol->set, (void *)recv_msg->buffer);
						break;
					}
					pcontrol = pcontrol->next;
				}
			}
		}
		printf("%s | %s | %d \n", __FILE__, __func__, __LINE__);

		if (ret == -1)
		{
			memset(success_or_failed, '\0', sizeof(success_or_failed));
			strncpy(success_or_failed, "failed", 6);
		}
		// oled显示
		char oled_msg[512];
		memset(oled_msg, 0, sizeof(oled_msg));
		char *change_status = cur_gdev->gpio_status == LOW ? "Open" : "Close";
		sprintf(oled_msg, "%s %s %s", cur_gdev->dev_name, change_status, success_or_failed);
		// 特殊处理
		if (somke_status == 1)
		{
			memset(oled_msg, 0, sizeof(oled_msg));
			strcpy(oled_msg, "Smoke detected");
		}
		oled_show(oled_msg);
		// 特殊处理关锁
		if (cur_gdev->check_face_status == 1 && ret == 0 && face_result > 0.6)
		{
			sleep(5);
			cur_gdev->check_face_status = HIGH;
			set_gpio_gdevice_status(cur_gdev);
			printf("%s|%s|%d\n",__FILE__, __func__, __LINE__);
		}
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
	// pthread_detach(pthread_self());
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
		else if (buffer[0] == 0xAA && buffer[1] == 0x55 && buffer[4] == 0x55 && buffer[5] == 0xAA)
		{
			recv_msg->msg_len = recv_len;
			memcpy(recv_msg->buffer, buffer, recv_len);
			pthread_create(&(tid), NULL, handle_device, (void *)recv_msg);
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
