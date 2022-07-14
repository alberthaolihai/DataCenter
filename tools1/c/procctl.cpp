#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc,char *argv[])
{
	if(argc < 3)
	{
	printf("Using:./procctl timetvl program argv \n");
	printf("Example:/project/tools1/bin/procctl 5 /usr/bin/tar zcvf /usr/include\n");
	printf("本程序是服务程序调度程序，周期性服务程序或shell脚本\n");
	printf("timetvl 运行时间周期 单位：秒 每隔该参数的时间，应用被调用一次\n");
	printf("program 被调度的程序名，使用绝对路径\n");
	printf("argvs 被调度的程序的参数.\n");
	printf("Note: 调度程序只能被 kill -9 结束进程\n");
	return -1;
	}
	// 关闭Linux信号，和io;
	for(int i =0; i< 64; i++)
	{
	signal(i,SIG_IGN);
	close(i);
	}
	// 生成子进程，父进程退出，程序后台运行，由1号进程托管;
	if(fork()!=0) exit(0);
	// 进程信号接受设置，父进程可以wait子进程消息
	signal(SIGCHLD,SIG_DFL);
	// exec 函数参数获取
	char *pargv[argc];
	for(int i = 2;i < argc; i++)
	{
	pargv[i-2] = argv[i];
	}
	pargv[argc - 2] = NULL;
	while(true)
	{
	if(fork() == 0) // 子进程
		{
	 	 execv(argv[2],pargv);
	 	 exit(0);
		}	
	else
		{
		// 父进程
		int status;
		wait(&status);
		sleep(atoi(argv[1]));
		}
	}
	return 0;
}
