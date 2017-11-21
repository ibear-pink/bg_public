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
	char chCluster[32+1];	/*������Ⱥ*/
	char chAttr[16+1];		/*�ڵ�����*/
	char chService[16+1];	/*�ڵ����*/
	int iServNum;				/*�ڵ�����*/
	
	/*buf���߰�����IP���Σ��˿ںţ����̱�ʶ*/
	char sInBuf[4096];		/*�����ַ���*/
	char sInterfaceTable[128];	/*�ӿڱ���*/
	char chProcEndFlag[32+1];   /*¼id,�������ֽ��̵�,Ҳ������Ϊ�ļ���Ψһ��ʶ*/
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
	int m_ProcessNum;//��������
	map<string, CDealDataMgr*> m_DealDataMgr;
	char m_Num[2+1];//�ܽ�����
	char m_CurNum[2+1];//��ǰ���̺�
	char m_BinName[64];//ִ�еĳ�������
	char m_HostId[64];//���������еļ�Ⱥ����

};

typedef GDF_Singleton<CProServerManager> App_ProServerManager;

#endif

