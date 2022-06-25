#include "li.h"
	

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
			wshowpicture();
			printf("22222\n");
		}
		if(strcmp(buf,"MUSIC_NEXT")==0)//字符串的比较
		{
			wvideoplay();
			printf("33333\n");
		}
		if(strcmp(buf,"MUSIC_PREV")==0)//字符串的比较
		{
			
			printf("44444\n");
		}
	}
}




int cgjb()
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
int wshowpicture()
{
	int x,y;
	
			bmp("../photo/5.bmp",600,480,0,0);
			bmp("../photo/33.bmp",200,120,600,0); //上一张
			bmp("../photo/34.bmp",200,120,600,120);//下一张
			bmp("../photo/35.bmp",200,120,600,240); //自动播放
			bmp("../photo/36.bmp",200,120,600,360); //退出
	
	char buf[200];
	struct sockaddr_in phoneaddr;
	int addrsize=sizeof(phoneaddr);
	
		
	//创建一个数组（用保存bmp图片名字）
	char *E_bmp_buf[3] = {"../photo/4.bmp" ,"../photo/5.bmp" ,"../photo/14.bmp" };
	int E_new_a = 0;
	//编写界面逻辑
	while(1)
	{
		bzero(buf,200);//清零
		//聊天，收发数据，接收手机发送过来的控制命令
		recvfrom(iphonex,buf,200,0,(struct sockaddr *)&phoneaddr,&addrsize);
		printf("huawei send msg is:%s\n",buf);
		//if(x >600 && x<800 && y >0&& y<120)
		if(strcmp(buf,"MUSIC_PLAY")==0)//上一张
		{			
			E_new_a++;
			bmp(E_bmp_buf[E_new_a%3] ,600, 480, 0, 0 );//  11.13.22.1.bmp   2.bmp  3.bmp
		}
		if(x>600 && x<800 && y>120 && y<240)//下一张
		{			
			E_new_a--;
			if(E_new_a == -1)//考虑临界点
			{
				E_new_a = 2;
			}
			bmp(E_bmp_buf[E_new_a%3] ,600, 480, 0, 0 );
		}
		if(x>600 && x<800 && y>240 && y<360)//自动播放
		{
			/*
			while(1)			
			{
				for(int i=0;i<3;i++)
				{
    
					new_a++;
				
					show_bmp(bmp_buf[new_a%3],540, 480, 0, 0);
					sleep(1);
				}
			}		
			*/
			
			bmp("../photo/4.bmp",600, 480, 0, 0);
			sleep(1);
			
			bmp("../photo/5.bmp",600, 480, 0, 0);
			sleep(1);
			
			bmp("../photo/14.bmp",600, 480, 0, 0);
			
		}
		if(strcmp(buf,"CONNECT_OK")==0)//退出
		{			
			bmp("../photo/100.bmp",600, 480, 0, 0);
			choice();
		}
	}
	
	return 0;
}

int wvideoplay()
{
	int x,y;
	int V_a = 0; //控制暂停/播放
	int V_b = 0; //控制静音/恢复音量
	int V_c = 0; //控制切换视频
	mkfifo_init();
	char *V_sp_buf[] = {"mplayer -slave -quiet -input file=/tmp/fifo -geometry 0:0 -zoom -x 700 -y 380 ../video/ssjs.avi &",
	"mplayer -slave -quiet -input file=/tmp/fifo -geometry 0:0 -zoom -x 700 -y 380 ../video/Faded3.avi &",
	"mplayer -slave -quiet -input file=/tmp/fifo -geometry 0:0 -zoom -x 700 -y 380 ../video/lol.avi &"};
	bmp("../photo/79.bmp",700,380,0,0);
	bmp("../photo/51.bmp",120,100,0,380);
	bmp("../photo/52.bmp",120,100,120,380); 
	bmp("../photo/53.bmp",120,100,240,380);
	bmp("../photo/54.bmp",120,100,360,380); 
	bmp("../photo/55.bmp",100,100,480,380);
	bmp("../photo/56.bmp",100,100,580,380);
	bmp("../photo/57.bmp",120,100,680,380);
	bmp("../photo/V_7.bmp",100,190,700,0);
	bmp("../photo/V_7_1.bmp",100,190,700,190);
	char buf[200];
	struct sockaddr_in phoneaddr;
	int addrsize=sizeof(phoneaddr);
	
	
	while(1)
   {
		bzero(buf,200);//清零
		//聊天，收发数据，接收手机发送过来的控制命令
		recvfrom(iphonex,buf,200,0,(struct sockaddr *)&phoneaddr,&addrsize);
		printf("huawei send msg is:%s\n",buf);
	if(x>0 && x<120 && y>380 && y<480)
	{
		write_fifo("seek -10\n");
		//bmp("../photo/2_1.bmp",120,100,120,380);
		printf("快退\n");
		V_a = 2;
	}
	
	if(strcmp(buf,"MUSIC_NEXT")==0)
	{
		if(V_a == 0)
		{
			system(V_sp_buf[0]);
			bmp("../photo/jixu.bmp",120,100,120,380);
			printf("开始播放\n");
			V_a = 2;
		}
		
		else if(V_a == 1)
		{
		write_fifo("pause\n");
		bmp("../photo/jixu.bmp",120,100,120,380);
		printf("继续播放\n");
		V_a = 2;
		}
		else if(V_a == 2)
		{
		write_fifo("pause\n");
		bmp("../photo/52.bmp",120,100,120,380);
		V_a = 1;
		printf("暂停播放\n");
		}
		
	}
	

	
	
	if(x>240 && x<360 && y>380 && y<480)
	{
		write_fifo("seek 10\n");
		//bmp("../photo/2_1.bmp",120,100,120,380);
		printf("快进\n");
		V_a = 2;
		
	}
	
	if(x>360 && x<480 && y>380 && y<480)
	{
		if(V_b == 0)
		{
		  write_fifo("mute\n");//静音
		  bmp("../photo/56.bmp",120,100,360,380);
		  //bmp("../photo/2.bmp",120,100,120,380);
		  V_b = 1;
		  V_a = 2;
		}
		else if(V_b == 1)
		{
		  write_fifo("mute\n");  //恢复音量
		  bmp("../photo/54.bmp",120,100,360,380);
		 // bmp("../photo/2.bmp",120,100,120,380);
		  V_b = 0;
		  V_a = 2;
		}
		
	}
	
	if(x>480 && x<580 && y>380 && y<480)
	{
		//bmp("../photo/2.bmp",120,100,120,380);
		write_fifo("volume -400\n"); //减音量
		printf("音量-\n");
		V_a = 2;
	}
	
	if(x>580 && x<680 && y>380 && y<480)
	{
		//bmp("../photo/2.bmp",120,100,120,380);
		write_fifo("volume 400\n"); //加音量
		printf("音量+\n");
		V_a = 2;
	}
	
		if(x>700 && x<800 && y>0 && y<190)
	{
		//bmp("../photo/7.bmp",100,190,0,190);
		sleep(1);
		if (V_a != 0 || V_a == 1 || V_a == 2) {
			write_fifo("quit 0\n");
		}	
		V_c--;
		if(V_c == -1)
		{
			V_c = 2;
		}
		system(V_sp_buf[V_c%3]);
		//bmp("../photo/2_1.bmp",120,100,120,380);
		printf("上一个视频\n");
		V_a = 2;
		
	}
	
	if(x>700 && x<800 && y>190 && y<380)
	{
		//bmp("../photo/7_1.bmp",100,190,190,380);
		sleep(1);
		if (V_a != 0 || V_a == 1 || V_a == 2) {
			write_fifo("quit 0\n");
		} 
		V_c++;
		if(V_c == 3)
		{
			V_c = 0;
		}
		system(V_sp_buf[V_c%3]);
		//bmp("../photo/2_1.bmp",120,100,120,380);
		printf("下一个视频\n");
		V_a = 2;
	}
	
	if(strcmp(buf,"MUSIC_PREV")==0)
	{
		write_fifo("quit 0\n"); //退出
		sleep(1);

		choice();
		
		
	}
}
}