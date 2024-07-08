#include "gdevice.h"
// 根据key值(buffer[2])查找设备节点
struct gdevice *find_gdevice_by_key(struct gdevice *pdev, int key)
{
    struct gdevice *p = NULL;
    if (NULL == pdev)
    {
        return NULL;
    }

    p = pdev;

    while (NULL != p)
    {
        if (p->key == key)
        {
            return p;
        }
        p = p->next;
    }

    return NULL;
}

// 设置GPIO引脚状态，输入输出和高低电平
int set_gpio_gdevice_status(struct gdevice *pdev)
{
    if (NULL == pdev)
    {
        return -1;
    }
    if (-1 != pdev->gpio_pin)
    {
        if (-1 != pdev->gpio_mode)
        {
            pinMode(pdev->gpio_pin, pdev->gpio_mode);
        }
        if (-1 != pdev->gpio_status)
        {
            digitalWrite(pdev->gpio_pin, pdev->gpio_status);
        }
    }
    return 0;
}

// 链表头插法
struct gdevice *add_device_to_gdevice_list(struct gdevice *phead, struct gdevice *device)
{
    struct gdevice *pgdevice;
    if (NULL == phead)
    {
        pgdevice = device;
        return pgdevice;
    }
    else
    {
        device->next = phead;
        phead = device;
        return phead;
    }
}