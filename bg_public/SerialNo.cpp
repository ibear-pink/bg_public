
#include "SerialNo.h"

SerialNo::SerialNo()
{
	i_serial_no	=	0	;
	i_unikey	=	0	;
	i_cluster	=	0	;
}

void SerialNo::Init_SerialNo(char * chProcEndFlag ,char * chCluster )
{
	i_serial_no = 0;		//序列号6位	从0开始	循环到999999
	i_unikey = 0;		//令牌中数字3位	取自g_stAppCfg.stZookeeper.chProcEndFlag
	i_cluster = 0;		//集群编码2位	取自g_stAppCfg.stZookeeper.chCluster
	if(strcmp(chProcEndFlag,"") ==0)
	{
		i_unikey = 100;
		i_cluster = 100;
	}
	else
	{
		i_unikey = atoi(chProcEndFlag);
		i_cluster = atoi(chCluster);
	}
}

string SerialNo::getSerialNo(int Thread_No)
{
	char	c_serial[18];				//最终序列码
	memset (c_serial, 0, sizeof(c_serial));
	
	string str_serial = "";
	
	time_t sec;
	struct tm t;
	sec = time(NULL);
	localtime_r(&sec,&t);
	//struct timeval tp;					//直接取微妙
	//gettimeofday(&tp,NULL);
	
	i_serial_no = i_serial_no + 1;		//序列计数
	if (i_serial_no > 999999)			//序列翻转
	{
		i_serial_no = 0;
	}
	sprintf (c_serial, "%03d%02d%02d%03d%02d%06d",
	 i_unikey, t.tm_hour,t.tm_min,t.tm_sec,Thread_No,i_serial_no);
	/*sprintf (c_serial, "%02d%%02d%06d",
	 i_unikey, Thread_No,i_serial_no);*/
	 
	str_serial = c_serial;
	
	return str_serial;
}

