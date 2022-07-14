#include "_public.h"
/*
 * 数据中心测试数据构造主函数
 * Author:Haoluchen
  */
// 日志类
CLogFile logfile;

// 方法声明，加载外部文件中的站点数据;
bool LoadSTCode(const char *inifile);
// 站点数据结构体
struct st_stcode
{
	char provname[31];// 省份
	char obtid[11];  // 站点Id
	char obtname[31];// 站点名
	double lat;  //纬度
	double lon; // 经度
	double height; // 海拔高度

};

// 结构体数组存放全国站点数据
vector<struct st_stcode> vstcode;


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
	logfile.Write("程序crtsuffdata2开始运行 \n");
	// 业务代码;
	
	if(LoadSTCode(argv[1])==false) return -1;
 	logfile.WriteEx("crtsurfdata2 运行结束。\n");
	return 0;

}

  // 打开文件。
  //   // filename：待打开的文件名，建议采用绝对路径的文件名。
  //     // openmode：打开文件的模式，与fopen库函数的打开模式相同。
  //       // bEnBuffer：是否启用缓冲，true-启用；false-不启用，缺省是启用。
  //         // 注意：如果待打开的文件的目录不存在，就会创建目录。
  //           bool Open(const char *filename,const char *openmode,bool bEnBuffer=true);
  //
// 加载文件的方法实现，常量指针，表示初始化文件位置;
bool LoadSTCode(const char *inifile)
{	// 封装好的文件类
	CFile File;
	// 判断是否成功打开文件
	if(File.Open(inifile,"r")== false)
	{
	logfile.Write("File.Open(%s) failed. \n",inifile);
	return false;
	} 
	// 封装好的字符串处理类
	CCmdStr CmdStr;
	// 缓冲数组，用来存储读取后的每行数据
	char strBuffer[301];
	// 结构体，存储每行数据
	struct st_stcode stcode;
	while(true)
	{
	 // 从站点文件中读取一，读完则break;
	 if(File.Fgets(strBuffer,300,true)==false){break; }
	// 处理读取到的字符串
	CmdStr.SplitToCmd(strBuffer,",",true);
	// 过滤无效数据
	if (CmdStr.CmdCount()!=6) continue;
	//对结构体初始化
	memset(&stcode,0,sizeof(struct st_stcode));
	CmdStr.GetValue(0,stcode.provname,30); // 省份

	CmdStr.GetValue(1,stcode.obtid,30); // 站点id

	CmdStr.GetValue(2,stcode.obtname,30); // 站点名

	CmdStr.GetValue(3,&stcode.lat); // 经度 
	
	CmdStr.GetValue(4,&stcode.lon); //纬度 
		
	CmdStr.GetValue(5,&stcode.height); // 高度 
	}
	vstcode.push_back(stcode);
	// 测试日志
	for(int i =0; i< vstcode.size();i++)
	{
	logfile.Write("provname=%s,obtid=%s,obtname=%s,lat=%.2f,lon=%.2f,height=%.2f\n",\
		       vstcode[i].provname,vstcode[i].obtid,vstcode[i].obtname,vstcode[i].lat,\
		       vstcode[i].lon,vstcode[i].height);	
	}
	return true;
}
