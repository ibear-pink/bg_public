
/*********************
* 功能描述:入口函数
* 编码日期:
* 编码人员:自动生成
*********************/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include "CProServerManager.h"

using namespace std;

int g_runflag;//0: run  1：stop

/**********************************************************
Function:		static void sig_quit(int signo)
Description:	信号处理函数
Input:			int signo, 信号编号
Output:			无
Return: 		无
Others:			产生跳转
**********************************************************/
static void sig_quit(int signo)
{
	g_runflag = 1;
}

int main (int argc,char *argv[])
{
	printf("%s\t############# RealTimeStop Program Started ############\n",GetSysTime());	
	int ret=0;
	char opr_Start[1024] = {0};
	char fileName[1024] = {0};
	char processName[1024] = {0};
	map<string,string> classMap;
	
	classMap.insert(pair<string,string>(string("Mgr"),string("Check_BDSMgr")));
	classMap.insert(pair<string,string>(string("Data"),string("Check_BDSData")));
	
	/*检查命令行参数的合法性，并获取合法配置文件名*/
	string inputs[5];
	strcpy(fileName,"/ocsapp/work/wangzhia/ibf/bg_public/demo/chkbds.conf");
	if (!App_ProServerManager::Instance()->Init (fileName,inputs))
	{
		printf ("App_ProServerManager Init Is Failed.\n");
		return RET_FAIL;
	}
	
	if (!App_ProServerManager::Instance()->Start(classMap))
	{
		printf ("App_ProServerManager Start Is Exit.Please Check Log.\n");
		return RET_FAIL;
	}
	
	signal(SIGTERM, sig_quit);
	signal(SIGINT, sig_quit);
	signal(SIGQUIT, sig_quit);
	signal(SIGPIPE, SIG_IGN);

	while (g_runflag == 0)
	{
		if (App_ProServerManager::Instance()->GetServerSize() == 0)
		{
			break;
		}

		sleep(5);
	}
	
	App_ProServerManager::Close();
	classMap.clear();
	return RET_OK;
}

