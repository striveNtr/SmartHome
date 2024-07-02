#include <stdio.h>
#include "msq_queue.h"
#include "voice_interface.h"
#include "control.h"
#include <pthread.h>
#include "global.h"

int main(int argc, char *argv[])
{
    pthread_t thread_id;
    struct control *control_phead = NULL;
    struct control *pointer = NULL;
    ctrl_info_t *ctrl_info = NULL;

    ctrl_info = (ctrl_info_t *)malloc(sizeof(ctrl_info_t));
    ctrl_info->ctrl_phead = NULL;
    ctrl_info->mqd = -1;
    int node_num = 0;
    ctrl_info->mqd = create_mqd();

    if (ctrl_info->mqd == -1)
    {
        printf("%s | %s | %d,ctrl_info->mqd=%d\n",__FILE__,__func__,__LINE__,ctrl_info->mqd);
        exit(-1);
    }

    ctrl_info->ctrl_phead = add_voice_to_ctrl_list(ctrl_info->ctrl_phead);

    pointer = ctrl_info->ctrl_phead;
    while (pointer != NULL)
    {
        if (pointer->init != NULL)
        {
            pointer->init();
        }
        pointer = pointer->next;
        node_num++;
    }

    pthread_t *tid = malloc(sizeof(int) * node_num);
    int i = 0;
    pointer = ctrl_info->ctrl_phead;
    for (i = 0; i < node_num; i++)
    {
        if (pointer->get != NULL)
        {
            pthread_create(&tid[i], NULL, (void *)pointer->get, (void *)ctrl_info);
        }
    }
    for (i = 0; i < node_num; i++)
    {
        pthread_join(tid[i], NULL);
    }
    for (i = 0; i < node_num; i++)
    {
        if (pointer->final != NULL)
        {
            pointer->final();
        }

        pointer = pointer->next;
    }
    msg_queue_final(ctrl_info->mqd);
    return 0;
}
