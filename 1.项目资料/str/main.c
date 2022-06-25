#include "li.h"

int main()
{
	//创建两个数组保存账号和密码
	char I_zhanghao[10]; //保存用户输入信息
	char I_mima[10];
	char I_zh_buf[10]; //保存文本
	char I_mm_buf[10];
	int I_a,I_ret;  //ret选择登录或注册  a判断账号密码
	bmp("../photo/denglu.bmp", 800, 480, 0, 0);
	
	//1、设计登录界面
	printf("-------------------\n");
	printf("|                 |\n");
	printf("|  账号：         |\n");
	printf("|                 |\n");
	printf("|  密码：         |\n");
	printf("|                 |\n");
	printf("|                 |\n");
	printf("|  1.  登    录   |\n");
	printf("|                 |\n");
	printf("|  2.  注    册   |\n");
    printf("|                 |\n");
	printf("|  3.  退    出   |\n");
	printf("|                 |\n");
	printf("-------------------\n");
	
	//打开账号和密码文本，并读取数据(动态)
	int I_zh_fd = open("../include/zhanghao.txt" ,O_RDWR );
	if(I_zh_fd == -1)
	{
		printf("open ../include/zhanghao.txt error!\n");
		return -1;
	}
	int I_mm_fd = open("../include/mima.txt" ,O_RDWR );
	if(I_mm_fd == -1)
	{
		printf("../include/open mima.txt error!\n");
		return -1;
	}
	
	Z:
	//2、编写登录界面逻辑
	printf("请输入账号：");
	scanf("%s" , I_zhanghao);
	printf("请输入密码：");
	scanf("%s" , I_mima);
	printf("请输入你的选择：1、登录    2、注册    3、退出\n");
	scanf("%d" , &I_ret);
	
	if(I_ret == 1)//选择登录
	{		
	    C:
	     I_a = read(I_zh_fd, I_zh_buf,3);
		 //printf("账号：%s\n",zh_buf);
		//1、账号和密码都是正确的  strcmp是字符串判断函数
		
		if(strncmp(I_zhanghao , I_zh_buf,3) == 0)//先判断账号
		{
			read(I_mm_fd, I_mm_buf,3);
			//printf("密码：%s\n",mm_buf);
			if(strncmp(I_mima , I_mm_buf,3) == 0)//再判断密码
			{
				printf("登录成功！\n");
				choice();
				
			}else
			{
				lseek(I_zh_fd, 0, SEEK_SET);
				lseek(I_mm_fd, 0, SEEK_SET);
				//2、密码是错误的
				printf("输入的账号或密码有误！\n");
				goto Z;//跳转语句
			}
			
		}else
		{
			lseek(I_mm_fd, 3, SEEK_CUR);//SEEK_CUR 文件保留在指针位置
			//3、账号错误的
			if(I_a == 0)
			{
				lseek(I_zh_fd, 0, SEEK_SET); //SEEK_SET让文件指针回到开头
				lseek(I_mm_fd, 0, SEEK_SET); 
				bzero(I_zhanghao , 10); //清空数组函数:bzero
				bzero(I_mima,10);
				printf("输入的账号或密码有误！\n");
			    goto Z;
			}
			goto C;
		}


		
	}else if(I_ret == 2)//选择注册
	{
		printf("注册中!....\n");
		lseek(I_zh_fd, 0, SEEK_END);//SEEK_END文件指针去到末尾
		lseek(I_mm_fd, 0, SEEK_END);
		
		write(I_zh_fd,I_zhanghao,3);//写入文本
		write(I_mm_fd,I_mima,3);
		
		lseek(I_zh_fd, 0, SEEK_SET); //文件回到开头
		lseek(I_mm_fd, 0, SEEK_SET);
		sleep(1);//延时一秒
		printf(">>>>注册成功<<<<\n");
		goto Z;
	}
	
	else if(I_ret == 3)//退出程序
	
	return 0;
}