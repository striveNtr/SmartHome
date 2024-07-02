#ifndef __FACE__H
#define __FACE__H



void face_init(void);
void face_fianl(void);
double face_category(void);

#define WGET_CMD  "wget https://192.168.1.54:8080/?action=snapshot -O /tmp/SearchFace.jpg"
#define FACE_FILE "/tmp/SearchFace.jpg"

#endif