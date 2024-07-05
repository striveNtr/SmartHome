#include "control.h"

struct control *add_interface_to_ctrl_list(struct control *phead, struct control *control_interface)
{
    struct control *pcontrol;
    if(phead == NULL)
    {
        pcontrol =  control_interface;
        return pcontrol;
    }
    else
    {
        control_interface->next = phead;
        phead = control_interface;
        return phead;
    }
}