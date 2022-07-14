#include "_public.h"
/*
 * 数据中心测试数据构造主函数
 * Author:Haoluchen
  */

CLogFile logfile;
int main(int argc,char *argv[])
{
	if(argc!=4)
	{
	// 如果参数非法，给出参考文档；
	printf("Using: ./crtsuffdata1 inifile outoarh logfile\n");
	printf("Example:/project/idc1/bin/crtsurfdata1 /project/idc1/ini/stcode.ini /tmp/surfdata /log/idc/crtsurfdata1.log\n\n"); 
	// 参数详解
	// Example:/project/idc1/bin/crtsurfdata1 /project/idc1/ini/stcode.ini /tmp/surfdata /log/idc/crtsurfdata1.log\n\n"
	printf("stcode.ini 全国气象站站点参数信息  \n");
	printf("outpath 输出数据存放位置，全国气象站站点观测数据  \n");
	printf("logfile 本程序运行的日志存放位置。 \n");
	return -1;
	}
	// 打开文件日志，如果失败，则退出程序
	if(logfile.Open(argv[3])== false)
	{
	printf("logfile %s open filed",argv[3]);
	return -1;
	}
	logfile.Write("程序crtsuffdata1开始运行 \n");
	// 业务代码;
	logfile.Write("程序crtsuffdata1结束运行 \n");
	return 0;

}
