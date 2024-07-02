#include "socket.h"

int socket_init(const char *ipaddr, const char *port)
{
    int s_fd = -1;
    int ret = -1;
    struct sockaddr_in s_addr;
    memset(&s_addr,0,sizeof(struct sockaddr_in));
    //1.socket
    s_fd = socket(AF_INET,SOCK_STREAM,0);
    if(s_fd == -1)
    {
        perror("socket");
        exit(-1);
    }
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(atoi(port));
    inet_aton(ipaddr,&s_addr.sin_addr);
    //2.bind
    ret = bind(s_fd,(struct sockaddr *)&s_addr,sizeof(struct sockaddr_in));
    if(ret == -1)
    {
        perror("bind");
        exit(-1);
    }
    //3.listen
    ret = listen(s_fd,1);
    if(ret == -1)
    {
        perror("listen");
        exit(-1);
    }
    return s_fd;
}
