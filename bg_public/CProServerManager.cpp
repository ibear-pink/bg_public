/**
 * describe:
 * create date:2013-10-28
 * author:wangzhia
 */
 
#include "CProServerManager.h"

CProServerManager::CProServerManager ()
{

}
 
CProServerManager::~CProServerManager()
{
	printf ("CProServerManager destructor.\n");
	if (m_iniCfg!=NULL)
	{
		delete m_iniCfg;
		m_iniCfg = NULL;
	}
	for (map<string, CDealDataMgr*>::iterator iter = m_DealDataMgr.begin(); iter != m_DealDataMgr.end(); iter++)
	{
		printf("CDealDataMgr is delete begin!!!!\n");
		CDealDataMgr *pData = iter->second;
		delete pData;
		pData = NULL;
	}
	m_DealDataMgr.clear();
	App_iLog::Instance()->ilog_Close();
	App_iLog::Close();
	if (m_CfgInfo!=NULL)
	{
		delete m_CfgInfo;
		m_CfgInfo = NULL;
	}
}

bool CProServerManager::Init (char *fileName,string input[])
{
	int iRet = 0;
	
	memset(m_Num,0,sizeof(m_Num));
	memset(m_CurNum,0,sizeof(m_CurNum));
	memset(m_BinName,0,sizeof(m_BinName));
#ifdef _ZK_CLIENT_
	getBinNameByPath((char *)input[0].c_str(), m_BinName);
	strcpy(m_HostId,input[1].c_str());
#else
	strcpy(m_Num,input[1].c_str());
	strcpy(m_CurNum,input[2].c_str());
#endif
	/*读配置文件*/
	m_iniCfg = new icfg_inifile();
	m_CfgInfo =new ilog_cfgserver();
	iRet = m_iniCfg->icfg_ReadFile(fileName);
	if (iRet != 0)
	{
		printf("读取配置文件失败[%d]\n",iRet);
		return false;
	}
	return true;
}

bool CProServerManager::Start (map<string,string> classMap)
{
	//加载日志服务配置
	strcpy(m_CfgInfo->log_name,m_iniCfg->icfg_Key_GetValue("LOG","log_name"));
	strcpy(m_CfgInfo->log_path,m_iniCfg->icfg_Key_GetValue("LOG","log_path"));
	m_CfgInfo->log_file_num = atoi(m_iniCfg->icfg_Key_GetValue("LOG","log_file_num"));
	m_CfgInfo->log_thread_num = atoi(m_iniCfg->icfg_Key_GetValue("LOG","log_thread_num"));
	for (int i = 0;i < m_CfgInfo->log_file_num ;i++)
	{
		char chname[128] = {0};
		sprintf(chname, "log_type%d",i);
		m_CfgInfo->logfile_infos[i].log_type = atoi(m_iniCfg->icfg_Key_GetValue("LOG",chname));

		sprintf(chname, "log_procmode%d",i);
		m_CfgInfo->logfile_infos[i].log_procmode = atoi(m_iniCfg->icfg_Key_GetValue("LOG",chname));
		
		sprintf(chname, "log_backup_type%d",i);
		m_CfgInfo->logfile_infos[i].log_backup_type = atoi(m_iniCfg->icfg_Key_GetValue("LOG",chname));
		
		sprintf(chname, "log_interval%d",i);
		m_CfgInfo->logfile_infos[i].log_interval = atoi(m_iniCfg->icfg_Key_GetValue("LOG",chname,"60"));
		
		sprintf(chname, "log_backup_size%d",i);
		m_CfgInfo->logfile_infos[i].log_backup_size = atoi(m_iniCfg->icfg_Key_GetValue("LOG",chname,"1"));
		
	}
	//加载业务服务器配置
	m_ProcessNum = atoi(m_iniCfg->icfg_Key_GetValue("PROC_SERVER","process_num","1"));
	
	stAppCfg_Zookeeper *zkCfgInfo = new stAppCfg_Zookeeper();
	int iOnlineFlag = 1;/*0; zk注册nodetype类型：抢占式、非抢占式*/
	char seriano[3+1] = {0};//序列开头
	char program_name[64+1] = {0};
	strcpy(program_name ,m_iniCfg->icfg_Key_GetValue("SYSTEM","program_name"));

#ifdef _ZK_CLIENT_
	strcpy (zkCfgInfo->chCluster, m_HostId);
	strcpy (zkCfgInfo->chAttr, program_name);
	strcpy (zkCfgInfo->chService, program_name);
	if (0 != ConnectZK(zkCfgInfo, iOnlineFlag, m_BinName))
	{
		printf ("FILE[%s]LINE[%d]ERR_MSG[ConnectZK is failed.]\n",  __FILE__, __LINE__);
		return -1;
	}
	printf("sInBuf:%s\n",zkCfgInfo->sInBuf);
	
	char sSegment[20] = {0};
	char szAddress[20]= {0};
	SplitAtPos(zkCfgInfo->sInBuf,sSegment,';',1);
	if (0 != GetValidIp(sSegment, MASK, szAddress))
	{
		printf ("FILE[%s]LINE[%d]ERR_MSG[GetValidIp is failed.]\n",  __FILE__, __LINE__);
		return -1;
	}
	
	m_iniCfg->icfg_SetKey((char *)"ZK_SERVER",(char *)"IP_ADDRESS",szAddress);
	char tmpStr[10+1] = {0};
	SplitAtPos(zkCfgInfo->sInBuf,seriano,';',7);
	SplitAtPos(zkCfgInfo->sInBuf,tmpStr ,';',8);
	SplitAtPos(tmpStr,m_Num,'-',1);
	SplitAtPos(tmpStr,m_CurNum,'-',2);
	printf("Zookeeper Register success [%s] [%s] [%s] [%s]!!!\n",szAddress,seriano,m_Num,m_CurNum);
	m_iniCfg->icfg_SetKey((char *)"ZK_SERVER",(char *)"HOST_ID",m_HostId);
	strreplace(m_CfgInfo->log_path,m_CfgInfo->log_path,(char *)ZK_HOST_ID,m_HostId);
#endif
	m_iniCfg->icfg_SetKey((char *)"ZK_SERVER",(char *)"NUM",m_Num);
	m_iniCfg->icfg_SetKey((char *)"ZK_SERVER",(char *)"CUR_NUM",m_CurNum);
	
	g_SerialNo.Init_SerialNo(seriano,seriano);
	//日志初始化
	if (m_ProcessNum <= 1)
	{
		if (!App_iLog::Instance()->ilog_Init(m_CfgInfo))
		{
			printf("加载日志服务失败\n");
			delete m_CfgInfo;
			m_CfgInfo = NULL;
			return false;
		}
	}
	else
	{
		for (int iLooper = 0; iLooper < m_ProcessNum; iLooper++)
		{
			if (!App_iLog::Instance()->ilog_Init(m_CfgInfo,iLooper))
			{
				printf("加载日志服务失败\n");
				delete m_CfgInfo;
				m_CfgInfo = NULL;
				return false;
			}
		}
	}
	//启动日志
	App_iLog::Instance()->ilog_Run();
	char server_name[64] = {0};
	int deal_type = atoi(m_iniCfg->icfg_Key_GetValue("PROC_SERVER","deal_type"));
	strcpy(server_name,m_iniCfg->icfg_Key_GetValue("PROC_SERVER","server_name"));
	int isgive = atoi(m_iniCfg->icfg_Key_GetValue("PROC_SERVER","is_give"));
	if (m_ProcessNum <= 1)
	{
		//初始化业务服务器
		CDealDataMgr *pMgr = new CDealDataMgr(classMap);
		if (pMgr->Init(m_iniCfg,0) != 0)
		{
			return false;
		}
		
		usleep (100);
		//启动业务服务器
		if (pMgr->Process() != 0)
		{
			return false;
		}

		if (deal_type == 1)
		{
			m_DealDataMgr.insert(pair<string, CDealDataMgr*>(string(server_name), pMgr));
		}
	}
	else
	{
		//多进程初始化
		for (int iLooper = 0; iLooper < m_ProcessNum; iLooper++)
		{
			int i,curMsgPos;
			pid_t pid; 
			if ((pid=fork())<0)
			{
				printf("初始化业务服务器失败 error!\n");
				exit(1);
			}
			else if (pid>0){
				printf("first parent pid[%d] ppid=[%d] 成功!\n",getpid(),getppid());
			}
			else if (pid==0)
			{
				//初始化业务服务器
				CDealDataMgr *pMgr = new CDealDataMgr(classMap);
				
				if (pMgr->Init(m_iniCfg,iLooper) != 0)
				{
					return false;
				}
				
				usleep (100);
				
				//启动业务服务器
				if (pMgr->Process() != 0)
				{
					return false;
				}
				if (deal_type == 1)
				{
					char server_name_temp[64] = {0};
					sprintf(server_name_temp, "%s_%d",server_name,iLooper);
					m_DealDataMgr.insert(pair<string, CDealDataMgr*>(string(server_name_temp), pMgr));
				}
			}
		}
	}
	return true;
}

#ifdef _ZK_CLIENT_
int CProServerManager::ConnectZK (stAppCfg_Zookeeper *zkInfo, int iOnlineFlag, const char * sProgramName)
{
	cZKClient *pZKClient;
	strZKNodeParm stNodeInfo;
	
	memset (&stNodeInfo, 0, sizeof(strZKNodeParm));
	
	char strBuf[2048] = {0};
	strcpy (strBuf, sProgramName);
	
	memset (&stNodeInfo, 0, sizeof(strZKNodeParm));
	
	char *p = strtok (strBuf, "/");
	while (p)
	{
		if (p == NULL)
		{
			break;
		}
		else
		{
			strcpy (stNodeInfo.szProcessName, p);
		}
		
		p = strtok (NULL, "/");
	}
	stNodeInfo.nNodeType = iOnlineFlag;
	strcpy (stNodeInfo.szCluster, zkInfo->chCluster);
	strcpy (stNodeInfo.szAttr, zkInfo->chAttr);
	strcpy (stNodeInfo.szService, zkInfo->chService);
	
	pZKClient= cZKClient::GetInstance();
	if( 0 != pZKClient->InitInfo(stNodeInfo))
	{
		printf("ZKClient InitClientInfo error!!!\n");
		return -1;
	}
	printf("InitInfo success!!!\n");
	string clientInfo;
	if( 0 != pZKClient->Register(clientInfo, 1))
	{
		printf("程序注册失败，即将退出...\n");
		return -1;
	}
	strcpy (zkInfo->sInBuf, clientInfo.c_str());
	
	return 0;
}
#endif


