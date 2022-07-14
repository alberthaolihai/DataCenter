#include "_public.h"
/*
 * 数据中心测试数据构造主函数
 * Author:Haoluchen
  */
// 日志类
CLogFile logfile;

// 方法声明，加载外部文件中的站点数据;
bool LoadSTCode(const char *inifile);
// 方法声明，构造测试数据
void CrtsurfData();
// 方法声明，把程序结果写入外部文件，参数：1.输出路径 2.输出文件格式
bool CrtSurfFile(const char *outpath,const char *datafmt);

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
// 结构体，存放观测数据
struct st_surfdata
{
	char obtid[11]; // 站点id
	char ddatetime[21]; // 观测时间:yyyymmddhh24miss
	int t; // 气温，单位:0.1摄氏度
	int p; // 气压，单位0.1百帕
	int u; //相对湿度,0-100之间的值;
	int wd;//风向，0-360之间的值;
	int wf;//风速,单位0.1m/s
	int r; //降雨量，单位0.1mm
	int vis;//能见度，单位0.1m
};

// 存放全国站点数据
vector<struct st_stcode> vstcode;
// 全局数组，存放观测数据
vector<struct st_surfdata> vsurfdata;
// 定义时间数组·
char strddatetime[21];
int main(int argc,char *argv[])
{
	if(argc!=5)
	{
	// 如果参数非法，给出参考文档；
	printf("Using: ./crtsuffdata1 inifile outoarh logfile datafmt \n");
	printf("Example:/project/idc1/bin/crtsurfdata4 /project/idc1/ini/stcode.ini /tmp/surfdata /log/idc/crtsurfdata4.log xml,json,csv\n\n"); 
	// 参数详解
	// Example:/project/idc1/bin/crtsurfdata1 /project/idc1/ini/stcode.ini /tmp/surfdata /log/idc/crtsurfdata1.log\n\n"
	printf("stcode.ini 全国气象站站点参数信息  \n");
	printf("outpath 输出数据存放位置，全国气象站站点观测数据  \n");
	printf("logfile 本程序运行的日志存放位置。 \n");
	printf("datafmt 生成数据文件的格式，支持xml、json和csv三种格式，中间用逗号分隔。\n\n");
	return -1;
	}
	// 打开文件日志，如果失败，则退出程序
	if(logfile.Open(argv[3])== false)
	{
	printf("logfile %s open filed",argv[3]);
	return -1;
	}
	logfile.Write("程序crtsuffdata4开始运行 \n");
	// 加载站点文件入容器;
	if (LoadSTCode(argv[1])==false) return -1;
	// 业务代码;
	CrtsurfData();	
	  // 把容器vsurfdata中的全国气象站点分钟观测数据写入文件。
	if (strstr(argv[4],"xml")!=0) CrtSurfFile(argv[2],"xml");
	if (strstr(argv[4],"json")!=0) CrtSurfFile(argv[2],"json");
	if (strstr(argv[4],"csv")!=0) CrtSurfFile(argv[2],"csv");
 	logfile.WriteEx("crtsurfdata4 运行结束。\n");
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
	vstcode.push_back(stcode);
	}
	// 测试日志
	for(int i =0; i< vstcode.size();i++)
	{
	logfile.Write("provname=%s,obtid=%s,obtname=%s,lat=%.2f,lon=%.2f,height=%.2f\n",\
		       vstcode[i].provname,vstcode[i].obtid,vstcode[i].obtname,vstcode[i].lat,\
		       vstcode[i].lon,vstcode[i].height);	
	}
	return true;
}

// 构造测试数据方法:
void CrtsurfData()
{
// 随机种子
srand(time(0));
// 生成结构体，存放观测数据
struct st_surfdata stsurfdata;
// 内存初始化
memset(strddatetime,0,sizeof(strddatetime));
// 获取当前时间
LocalTime(strddatetime,"yyyymmddhh24miss");
// 遍历气象站点数据
for(int i =0; i < vstcode.size();i++)
{
	// 给结构体初始化
	memset(&stsurfdata,0,sizeof(stsurfdata));
	// 给结构体赋值
	strncpy(stsurfdata.obtid,vstcode[i].obtid,10); //站点编码
	strncpy(stsurfdata.ddatetime,strddatetime,14); //数据时间 yyyymmddhh24miss
	stsurfdata.t = rand()%351;	// 	气温
	stsurfdata.p = rand()%264+1000; // 压强
	stsurfdata.wd=rand()%360;      // 风向，0-360之间的值。
        stsurfdata.wf=rand()%150;      // 风速：单位0.1m/s
        stsurfdata.r=rand()%16;        // 降雨量：0.1mm
        stsurfdata.vis=rand()%5001+100000;  // 能见度：0.1米
	// 结构体推入容器
	vsurfdata.push_back(stsurfdata);
}
}

// 观测数据写入外部文件;
bool CrtSurfFile(const char *outpath,const char *datafmt)
{
	// 文件类
	CFile File;
	// 拼接生成的文件名;
	char strFileName[301];
	sprintf(strFileName,"%s/SURF_ZH_%s_%d.%s",outpath,strddatetime,getpid(),datafmt); // 路径_时间_进程id_文件格式
	// 打开文件;
	if(File.OpenForRename(strFileName,"w") == false)
	{
		logfile.Write("File.OpenForRename(%s) failed. \n",strFileName);
		return false;
	}
	// 写入文件第一行
	if(strcmp(datafmt,"csv")==0) {File.Fprintf("站点代码,数据时间,气温,气压,相对湿度,风向,风速,降雨量,能见度\n");}
	// 遍历存放观测数据的容器
	for(int i = 0;i<vsurfdata.size();i++)
	{
		if(strcmp(datafmt,"csv")==0)
		{
		File.Fprintf("%s,%s,%.1f,%.1f,%d,%d,%.1f,%.1f,%.1f\n",\
			    vsurfdata[i].obtid,vsurfdata[i].ddatetime,vsurfdata[i].t/10.0,vsurfdata[i].p/10.0,vsurfdata[i].u,vsurfdata[i].wd,vsurfdata[i].wf/10.0,\
			    vsurfdata[i].r/10.0,vsurfdata[i].vis/10.0);
		}
	}
	File.CloseAndRename();
	logfile.Write("生成数据文件%s成功，数据时间%s,记录数%d. \n",strFileName,strddatetime,vsurfdata.size());
	return true;
}












