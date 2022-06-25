#include "li.h"

#define ON  0
#define OFF 1

int bmp(char * bmp_name, int bmp_w, int bmp_h, int new_x, int new_y)
{
	
	//1.打开图片
	int bmp_fd = open(bmp_name, O_RDWR);
	if(bmp_fd == -1)
	{
		printf("open bmp error!\n");
		return -1;
	}
	//申请空间
	char lj_buf[54];
	char bmp_buf[bmp_w*bmp_h*3];
	
	//2.读取图片数据
	read(bmp_fd, lj_buf, 54);
	read(bmp_fd, bmp_buf, bmp_w*bmp_h*3);
	
	//3.关闭图片
	close(bmp_fd);
	
	//申请空间
	int lcd_buf[800*480];
	
	//合成像素点（一个像素点）
	//lcd_buf[0] = bmp_buf[0]<<0 | bmp_buf[1]<<8 | bmp_buf[2]<<16;
	
	//4.打开显示屏
	int lcd_fd = open("/dev/fb0" ,O_RDWR);
	if(lcd_fd == -1)
	{
		printf("open lcd error!\n");
		return -1;
	}
	
	//调用内存映射函数：mmap
	int *mmap_fd = (int*)mmap(NULL, 800*480*4, PROT_READ | PROT_WRITE, MAP_SHARED, lcd_fd, 0);
 /* 参数一:申请内存空间的地址（NULL）
	参数二:需要申请内存空间大小
	参数三:对申请内存空间赋予权限
	参数四:是否将申请内存空间里面的数据映射到硬件里面
	参数五:映射对象
	参数六:偏移量
	注意:mmap函数返回值:就是申请内存空间首地址 */
	

	
	int x,y;
	for(y=0;y<bmp_h;y++)
	{
		for(x=0;x<bmp_w;x++) //合成一行（800个像素点）
	    {
			
	    *(mmap_fd+(x+new_x)+((bmp_h-1-y)+new_y)*800) = bmp_buf[x*3+bmp_w*y*3]<<0 | bmp_buf[x*3+1+bmp_w*y*3]<<8 | bmp_buf[x*3+2+bmp_w*y*3]<<16;
	    }
	
	}
	
	
	
	//5.把合成完毕的lcd_buf写入显示屏
	//write(lcd_fd, lcd_buf, 800*480*4);
	//6.关闭显示屏
	close(lcd_fd);
	
	
	return 0;
}

int new_touch(int *x ,int *y)
{
	//1、打开触摸屏
	int fd = open("/dev/input/event0" ,O_RDWR);
	if(fd == -1)
	{
		printf("open event error!\n");
		return -1;
	}
	//int x,y;//局部变量（只能在当前函数使用）
	//2、声明触摸屏结构体：
	struct input_event  ts;
	while(1)//死循环
	{
		//3、把触摸屏数据读取到摸屏结构体里面
		read(fd , &ts , sizeof(struct input_event));
		
		if(ts.type == EV_ABS)//判断是不是触摸屏事件
		{
			if(ts.code == ABS_X )
			{
				*x = ts.value*800/1024;					
			}
			if(ts.code == ABS_Y )
			{
				*y = ts.value*480/600;				
			}	
			
		}
		
		
		if(ts.type == EV_KEY)//判断是不是压力事件
		{
			if(ts.code == BTN_TOUCH)//判断是不是压力事件
			{
				if(ts.value == 0)// 按压等于：1    松手等于：0
				{
					printf("x轴=%d\n",*x);
					printf("y轴=%d\n",*y);
					break;
				}
			}
			
		}
		
	
				
	}
	
	
	//4、关闭触摸屏
	close(fd);
	
	return 0;
	
}

int mkfifo_init()
{
	//1.创建管道文件
	//先判断文件是否存在：access
	if(access("/tmp/fifo" ,F_OK) == -1)
	{
	   int ret = mkfifo("/tmp/fifo" ,0777);
	   if(ret == -1)
	   {
		   printf("open fifo error!\n");
		   return -1;
	   }
	}
	
	//2.打开管道文件(目的：往管道文件写入)
	fifo_fd = open("/tmp/fifo" ,O_RDWR);
	if(fifo_fd == -1)
	{
		 printf("open fifo error!\n");
		 return -1;
	}
}

void write_fifo(char *arg)
{
	
	//把slave命令通过管道写入mplayer
	write(fifo_fd,arg, strlen(arg));//strlen自动计算字节
	
}

int musicplay()
{
	int x,y,a=0;
	int num = 0;
	char *bmp_buf[3] = {"mplayer ../music/yy.mp3 &","mplayer ../music/Apologize.mp3 &","mplayer ../music/faded.mp3 &"};
	
	bmp("../photo/45.bmp",800,380,0,0);
	bmp("../photo/49.bmp",200,100,0,380);
	bmp("../photo/47.bmp",200,100,200,380);
	bmp("../photo/48.bmp",200,100,400,380);
	bmp("../photo/59.bmp",200,100,600,380);
	
	new_touch(&x ,&y);
	
	if ((x > 0 && x < 800) && (y > 0 && y < 480))
	{
		system("madplay   ../music/yy.mp3 &");
	}
	
	while(1)
	{
		new_touch(&x ,&y);
		if((x>0 && x<200 && y>380 && y<480))
		{
			
			if(a==0)
			{
				
				system("killall -19 mplayer");
				a++;
			}else
			{
				
				system("killall -18 mplayer");			
				a=0;
			}
				
		}
		

		if((x>200 && x<400 && y>380 && y<480))
		{	
			
			num++;
			//show_bmp(bmp_buf[num%4] ,600,480,0,0);
			system("killall -9 mplayer");
			system(bmp_buf[num%3]);

					
		}
		if((x>400 && x<600 && y>380 && y<480))
		{	
		
			num--;
			if(num == -1)
			{
				num = 3;
			}
			system("killall -9 mplayer");
			system(bmp_buf[num%3]);
			
		}
		
		
		
		if((x>600 && x<800 && y>380 && y<480))
		{
			
			printf("退出播放！\n");
			system("killall  -9  madplay");
			choice();
			
			
		}
	}
	
	return 0;
}


int videoplay()
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
	
	
	
	while(1)
   {
	new_touch(&x , &y);
	if(x>0 && x<120 && y>380 && y<480)
	{
		write_fifo("seek -10\n");
		//bmp("../photo/2_1.bmp",120,100,120,380);
		printf("快退\n");
		V_a = 2;
	}
	
	if(x>120 && x<240 && y>380 && y<480)
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
	
	if(x>680 && x<800 && y>380 && y<480)
	{
		write_fifo("quit 0\n"); //退出
		sleep(1);

		choice();
		
		
	}
}
}

int showpicture()
{
	int x,y;
	
			bmp("../photo/5.bmp",600,480,0,0);
			bmp("../photo/33.bmp",200,120,600,0); //上一张
			bmp("../photo/34.bmp",200,120,600,120);//下一张
			bmp("../photo/35.bmp",200,120,600,240); //自动播放
			bmp("../photo/36.bmp",200,120,600,360); //退出
	
	//创建一个数组（用保存bmp图片名字）
	char *E_bmp_buf[3] = {"../photo/4.bmp" ,"../photo/5.bmp" ,"../photo/14.bmp" };
	int E_new_a = 0;
	//编写界面逻辑
	while(1)
	{
		new_touch(&x ,&y);
		//if(x >600 && x<800 && y >0&& y<120)
		if(x>600&& x<800 && y>0 && y<120)//上一张
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
		if(x>600 && x<800 && y>360 && y<480)//退出
		{			
			bmp("../photo/100.bmp",600, 480, 0, 0);
			choice();
		}
	}
	
	return 0;
}

int huoyan()
{
	bmp("../photo/66.bmp", 800, 480, 0, 0);
	bmp("../photo/98.bmp", 200, 100, 600, 0);
	bmp("../photo/59.bmp", 200, 100, 600, 380);
	
	int x, y;
	
	while(1) {
		new_touch(&x ,&y);
		if (x>600 && x<800 && y>0 && y<100) {
			break;
		}
		
		if (x>600 && x<800 && y>380 && y<480) {
			choice();
		}
	}
	
	int F_fd;
	int F_ret;
	int F_mq2_flag=0; 
	//打开设备文件---/dev/mq2_drv
	F_fd = open("/dev/mq2_drv", O_RDWR); 
	if(F_fd < 0)
	{
		perror("open /dev/mq2_drv");
		return -1;	
	}
		// 打开蜂鸣器
	int F_beep_fd = open("/dev/beep", O_RDWR);
	if(F_beep_fd == -1)
	{
		perror("open:");
		exit(0);
	}
	
	while(1)
	{	
		F_ret = read(F_fd,&F_mq2_flag,4);		
		if (F_ret < 0)
		{
			perror("read /dev/key_drv error\n");
			return -1;
		}
		if(F_mq2_flag == 1)
		{
			// 打开蜂鸣器
			ioctl(F_beep_fd, ON, 1);		
		}
		usleep(300*1000);
		ioctl(F_beep_fd, OFF, 1);
		printf("mq2 flag = %d\n",F_mq2_flag);
			
	}
	close(F_fd);	
	return 0;
}

int choice()
{
	int x, y;
	
	bmp("../photo/2.bmp",800, 480, 0, 0);
	//bmp("../photo/57.bmp", 100, 100, 700, 0);
	
	while(1) {
		new_touch(&x , &y);
		if (x > 0 && x < 150 && y > 200 && y < 420) {
			 showpicture();
		}
		
		if (x > 150 && x < 320 && y > 200 && y < 420) {
			 videoplay();
		}
		
		if (x > 320 && x < 480 && y > 200 && y < 420) {
			musicplay();
		}
		
		if (x > 480 && x < 640 && y > 200 && y < 420) {
			huoyan();
		}
		
		if (x > 650 && x < 800 && y > 200 && y < 480) {
			//denglu();
			cgjb();
		}
	}
	
	return 0;
}














/*int main()
{
	char zhanghao[10];
	char mima[10];
	char zh_buf[10];
	char mm_buf[10];
	int a;
	int ret;
	bmp("1.bmp",800,480,0,0);
	K:
	//1、编写登录界面
	printf("----------------------------\n");
	printf("|                          |\n");
	printf("|                          |\n");
	printf("|        APP登录界面       |\n");
	printf("|                          |\n");
	printf("|                          |\n");
	printf("|                          |\n");
	printf("|                          |\n");
	printf("|   1、 登       录        |\n");
	printf("|                          |\n");
	printf("|                          |\n");
	printf("|   2、 注       册        |\n");
	printf("|                          |\n");
	printf("|                          |\n");
	printf("|   3、 退       出        |\n");
	printf("----------------------------\n");
	
	
	//打开账号和密码文本，并且把数据读取出来
	int zh_fd = open( "zhanghao.txt" , O_RDWR);
	if(zh_fd == -1)
	{
		printf("open zhanghao.txt error!\n");
		return -1;
	}
	int mm_fd = open( "mima.txt" , O_RDWR);
	if(mm_fd == -1)
	{
		printf("open mima.txt error!\n");
		return -1;
	}
		
	
	Z:
	//2、编写登录界面逻辑
	printf("请输入账号：");
	scanf("%s" , zhanghao);
	printf("请输入密码：");
	scanf("%s" , mima);
	printf("请输入你的选择：1、登录   2、注册   3、退出\n");
	scanf("%d" , &ret);
	if(ret == 1)//选择的是登录
	{
		C:
	    a = read(zh_fd ,zh_buf ,3);
		
		//1、账号正确，密码正确
	if(strncmp(zhanghao , zh_buf,3) == 0)//先判断账号   字符串判断函数：strncmp
	{
	read(mm_fd ,mm_buf ,3);
			
	if(strncmp(mima , mm_buf,3) == 0)//再判断密码
	{
	printf("登录成功！\n");    //登录成功进入主页面
	
	A:
	bmp("2.bmp",800,480,0,0);//主界面图片
	int x ,y ;
	while(1)
	{
		new_touch(&x ,&y);
		if(x>0 && x<150 && y>215 && y<420)//电子相册
		{
			char *tp_buf[] ={"3.bmp","4.bmp","13.bmp","14.bmp",};
			int x,y;
			int a = 0;
			//bmp("5.bmp",600,480,0,0);
			bmp("3.bmp",200,120,600,0); //上一张
			bmp("4.bmp",200,120,600,120);//下一张
			bmp("13.bmp",200,120,600,240); //自动播放
			bmp("14.bmp",200,120,600,360); //退出
	
	
		//2.编写当前界面的逻辑
			while(1)
			{
				new_touch(&x ,&y);
			if(x >600 && x<800 && y >0&& y<120)//上
			{
				a--;//上一张
				if(a == -1)
				{
					a = 4;
				}
				bmp(tp_buf[a%3],200,120,0,0);
				
			}
			
			if(x >600 && x<800 && y >120&& y<240)
			{
				a++;//下一张
				if(a == 5)
				{
					a = 0;
				}
				bmp(tp_buf[a%3],200,120,200,200);
				
			}
			
			if(x >600 && x<800 && y >240&& y<360)
			{
				for(int i = 0; i<3 ;i++)
				{
					a++;//自动播放
					bmp(tp_buf[a%3],200,120,240,360);
					sleep(1);
				}
			}
			if(x >600 && x<800 && y >360&& y<480)
			{
				//退出
				//bmp(tp_buf[a%5],600,480,0,0);
				system("killall -19 madplay");
				printf("退出\n");
				goto A;
				
			}
			}
		}	
		if(x>150 && x<320 && y>215 && y<420)//视频播放
		{
		int x,y;
		int a = 0; //控制暂停/播放
		int b = 0; //控制静音/恢复音量
		int c = 0; //控制切换视频
		mkfifo_init();
		char *sp_buf[] = {"mplayer -slave -quiet -input file=/tmp/fifo -geometry 0:0 -zoom -x 800 -y 380 ssjs.avi &",
		"mplayer -slave -quiet -input file=/tmp/fifo -geometry 0:0 -zoom -x 800 -y 380 Faded3.avi &",
		"mplayer -slave -quiet -input file=/tmp/fifo -geometry 0:0 -zoom -x 800 -y 380 lol.avi &"};
		bmp("fm.bmp",800,380,0,0);
		bmp("kj.bmp",120,100,0,380);
		bmp("zt.bmp",120,100,120,380); 
		bmp("kt.bmp",120,100,240,380);
		bmp("44.bmp",120,100,360,380); 
		bmp("55.bmp",200,100,480,380);
		bmp("66.bmp",120,100,680,380);
		
		//播放视频
		
		
		//通过触摸屏，改变播放状态
		while(1)
		{
			new_touch(&x , &y);
			if(x>0 && x<160 && y>450 && y<600)
			{
				system(sp_buf[2]);
				
				printf("播放\n");
				a = 2;
			}

			if(x>160 && x<320 && y>450 && y<600)
			{
				if(a == 0)
				{
					
					
					printf("开始播放\n");
					a = 2;
				}
				
				else if(a == 1)
				{
				write_fifo("pause\n");
				
				printf("继续播放\n");
				a = 2;
				}
				
				else if(a == 2)
				{
				write_fifo("pause\n");
				bmp("zt.bmp",120,100,120,380);
				a = 1;
				printf("暂停播放\n");
				}
			}
			if(x>320 && x<480 && y>450 && y<600)
			{
				write_fifo("seek 10\n");
				
				printf("快进\n");
				a = 2;
				
				}
			if(x>480 && x<640 && y>450 && y<600)
			{
				if(b == 0)
				{
				  write_fifo("mute\n");//静音
				 
				  b = 1;
				  a = 2;
				}
				else if(b == 1)
				{
				  write_fifo("mute\n");  //恢复音量
				  b = 0;
				  a = 2;
				}
			}
			if(x>640 && x<740 && y>450 && y<600)
			{
				
				write_fifo("volume -400\n"); //减音量
				printf("音量-\n");
				a = 2;
			}
			
			if(x>740 && x<840 && y>450 && y<600)
			{
				
				write_fifo("volume 400\n"); //加音量
				printf("音量+\n");
				a = 2;
			}
			
				if(x>0 && x<150 && y>200 && y<500)
			{
				sleep(1);
				if(a != 0 || a == 1 || a == 2)
				{
				write_fifo("quit 0\n");
				}
				c--;
				if(c == -1)
				{
					c = 2;
				}
				system(sp_buf[c%3]);
				
				printf("上一个视频\n");
				a = 2;
			}
			if(x>850 && x<1000 && y>200 && y<500)
			{
				sleep(1);
				if(a != 0 || a == 1 || a == 2)
				{
				write_fifo("quit 0\n");
				}
				c++;
				if(c == 3)
				{
					c = 0;
				}
				system(sp_buf[c%3]);
				
				printf("下一个视频\n");
				a = 2;
			}
			if(x>840 && x<1000 && y>450 && y<600)
			{
				write_fifo("quit 0\n"); //退出
				sleep(1);
				goto A;	
			}
		}
		}
		if(x>320 && x<480 && y>215 && y<420)//音乐播放
		{
			int i =0;
		int x,y;
		//让用户看到界面
		bmp("45.bmp" , 600,480,0,0);
		bmp("46.bmp" , 200,120,600,120);
		bmp("47.bmp" , 200,120,600,240);
		bmp("48.bmp" , 200,120,600,360);
		bmp("49.bmp" , 200,120,600,480);
		//int show_bmp(char *bmp_name , int bmp_w ,int bmp_h , int new_x ,int new_y)
		//数组保存图片
		//char *bmp_buf[5] = {"45.bmp","9.bmp","10.bmp","8.bmp","12.bmp"};
		char * music [3]={"mplayer faded.mp3 &","mplayer yy.mp3 &","mplayer Apologize.mp3 &"};
		int new_a = 1;
		
		//编写逻辑
		while(1)
		{		
		new_touch(&x ,&y);
		
		if(x >800 && x<1024 && y >0&& y<120)//上
		{	//new_a++;
			//printf("上");
			//show_bmp(bmp_buf[new_a%4] ,200,120,200,200);
			//system("mplayer Apologize.mp3 &");
			//i++;
			system("killall -19 mplayer");
		}
		if(x >800&& x<1024 && y >120&& y<240)//下
		{	new_a--;
			if(new_a == -1)//考虑临界点
			{
				new_a = 2;
			}
			//show_bmp(bmp_buf[new_a%4] ,200,120,0,0 );
			//system("mplayer yy.mp3 &");
			//system(music[i%3]);
			//printf("下");
			i +=2;
			i--;
			if (i == -1)
			{
				i = 2;
			}
			system(music[i%3]);
		}
		if(x > 800 && x<1024 && y>240&& y<360 )
		{	
			system("killall -18 mplayer");
		}
		if(x > 800 && x<1024 && y>360&& y<480)
		{
			printf("退出\n");
			system("killall -9 madplay");
			goto A;
			
		}
	}
	
	return 0;
		
		}
		
		
		if(x>480 && x<700 && y>215 && y<420)//火焰检测
		{
			
			pthread_t thread;
			pthread_create(&thread,NULL,one_thread,NULL);
			bmp("1.bmp",800,480,0,0);//蓝色背景
			bmp("r.bmp",200,120,600,360);//退出
			new_touch(&x ,&y);
			if(x>750 && x<1000 && y>450 && y<600)
			{
				pthread_cancel(thread);
				goto A;
			}
		}
	}
	}else  
		   {
				lseek(zh_fd , 0 , SEEK_SET );
				lseek(mm_fd , 0 , SEEK_SET );
				//2、账号正确，密码错误
				printf("账号或密码有误！\n");
				goto Z;
		   }
		
	}else
	{
	   lseek(mm_fd , 3 , SEEK_CUR);
		//3、账号错误				
		if(a == 0)
		{
			lseek(zh_fd , 0 , SEEK_SET );
			lseek(mm_fd , 0 , SEEK_SET );
			
			//清空数组函数：bzero
			bzero(zhanghao , 10);
			bzero(mima , 10);
			printf("账号或密码有误！\n");	
			goto Z;
		}
		goto C;
	}
	}
	else if(ret == 2)
	{
		printf("注册中......\n");
		lseek(zh_fd , 0 , SEEK_END );//让文本指针去到文件末尾
	    lseek(mm_fd , 0 , SEEK_END );
		
		write(zh_fd , zhanghao,3);//写入文本
		write(mm_fd , mima,3);
		
		lseek(zh_fd , 0 , SEEK_SET );//让文本指针回到文件开头
		lseek(mm_fd , 0 , SEEK_SET );
		sleep(1);
		printf(">>>注册成功<<<\n");
		goto Z;
	}
	
	
}*/