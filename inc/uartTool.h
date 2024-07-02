#ifndef __UARTTOOL__H
#define __UARTTOOL__H

int myserialOpen(const char *device ,const int baud);
void serialSendstring(const int fd, const unsigned char *s ,int len);
int serialGetstring(const int fd,unsigned char *buffer);

#define SERIAL_DEV "/dev/ttyS5"
#define BAUD 115200

#endif

