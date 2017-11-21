
/*********************
* ��������:��ں���
* ��������:
* ������Ա:�Զ�����
*********************/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include "CProServerManager.h"

using namespace std;

int g_runflag;//0: run  1��stop

/**********************************************************
Function:		static void sig_quit(int signo)
Description:	�źŴ�����
Input:			int signo, �źű��
Output:			��
Return: 		��
Others:			������ת
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
	
	/*��������в����ĺϷ��ԣ�����ȡ�Ϸ������ļ���*/
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

