/**
 * describe:
 * create date:2013-10-28
 * author:wangzhia
 */
#ifndef _CPRO_SERVER_MANAGER_H_
#define _CPRO_SERVER_MANAGER_H_

#include <stdio.h>
#include <string>
#include <iostream>

#include "CDealDataMgr.h"
#ifdef _ZK_CLIENT_
#include "ZKClient.h"
#endif

using namespace std;

typedef struct
{
	char chCluster[32+1];	/*归属集群*/
	char chAttr[16+1];		/*节点属性*/
	char chService[16+1];	/*节点服务*/
	int iServNum;				/*节点总数*/
	
	/*buf离线包括：IP网段；端口号；进程标识*/
	char sInBuf[4096];		/*返回字符串*/
	char sInterfaceTable[128];	/*接口表名*/
	char chProcEndFlag[32+1];   /*录id,用于区分进程的,也用于作为文件名唯一标识*/
}stAppCfg_Zookeeper;

class CProServerManager
{
public:
	CProServerManager (void);
	
	~CProServerManager (void);
	
	bool Init (char *fileName,string input[]);
	
	bool Start (map<string,string> className);
	
	int ConnectZK (stAppCfg_Zookeeper *zkInfo, int iOnlineFlag, const char * sProgramName);
	
	int GetServerSize () {return m_DealDataMgr.size();}
private:
	
	char m_InputStr[1024];
	icfg_inifile *m_iniCfg;
	ilog_cfgserver *m_CfgInfo;
	int m_ProcessNum;//进程数量
	map<string, CDealDataMgr*> m_DealDataMgr;
	char m_Num[2+1];//总进程数
	char m_CurNum[2+1];//当前进程号
	char m_BinName[64];//执行的程序名称
	char m_HostId[64];//程序名称中的集群名称

};

typedef GDF_Singleton<CProServerManager> App_ProServerManager;

#endif

