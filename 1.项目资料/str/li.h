#ifndef _LI_H_
#define _LI_H_

#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h> //基本读写头文件
#include <string.h> //strcmp需调用的头文件
#include <sys/stat.h> //open需调用的头文件
#include <fcntl.h>  //open需调用的头文件
#include <unistd.h> //read需调用的头文件
#include <strings.h> //bzero需调用的头文件
#include <sys/types.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <sys/mman.h>

#define ON  0
#define OFF 1


int bmp(char * bmp_name, int bmp_w, int bmp_h, int new_x, int new_y);
int new_touch(int *x ,int *y);
int new_touch(int *x ,int *y);
void write_fifo(char *arg);
int wshowpicture();
int fifo_fd;
int choice();
int wvideoplay();
int mkfifo_init();
int musicplay();
int videoplay();
int showpicture();
int huoyan();
int cgjb();
int udp_init();
void *myrecvmsg(void *arg);




#endif