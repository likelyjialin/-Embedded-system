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
//#include "wiff.h"

int iphonex;
//封装初始化udp通信的函数
int udp_init()
{
	int ret;
	//定义一个ipv4地址结构体变量
	struct sockaddr_in bindaddr;
	//初始化该结构体
	bindaddr.sin_family=AF_INET;
	bindaddr.sin_addr.s_addr=inet_addr("192.168.0.10");//开发板无线网卡的ip地址
	bindaddr.sin_port=htons(2234);//端口号也需要转换
	
	//买手机---》创建一个udp套接字(数据报套接字)用于通信
	iphonex=socket(AF_INET,SOCK_DGRAM,0);
	if(iphonex==-1)
	{
		printf("create socket failed!\n");
		return -1;
	}
	//绑定 ---》自己ip和端口号   
	ret=bind(iphonex,(struct sockaddr *)&bindaddr,sizeof(bindaddr));
	if(ret==-1)
	{
		printf("bind failed!\n");
		return -1;
	}
	return 0;
}
//线程的功能函数，接收来自手机app发送的控制命令
void *myrecvmsg(void *arg)
{
	char buf[200];
	struct sockaddr_in phoneaddr;
	int addrsize=sizeof(phoneaddr);
	while(1)
	{
		bzero(buf,200);//清零
		//聊天，收发数据，接收手机发送过来的控制命令
		recvfrom(iphonex,buf,200,0,(struct sockaddr *)&phoneaddr,&addrsize);
		printf("huawei send msg is:%s\n",buf);
		//判断收到的命令
		if(strcmp(buf,"CONNECT_OK")==0)//字符串的比较
		{
			
			printf("11111\n");
		}
		if(strcmp(buf,"MUSIC_PLAY")==0)//字符串的比较
		{
			
			printf("22222\n");
		}
		if(strcmp(buf,"MUSIC_NEXT")==0)//字符串的比较
		{
			
			printf("33333\n");
		}
		if(strcmp(buf,"MUSIC_PREV")==0)//字符串的比较
		{
			
			printf("44444\n");
		}
	}
}




int main()
{
	pthread_t recvid;

	//初始化udp
	udp_init();
	//创建一个线程专门接收来自手机app发送过来的控制命令
	pthread_create(&recvid,NULL,myrecvmsg,NULL);
	
	char buf[10];
	while(1)
	{
		bzero(buf , 10);
		printf("输入内容：");
		scanf("%s",buf);
		write(iphonex , buf , strlen(buf));
		
	}
}