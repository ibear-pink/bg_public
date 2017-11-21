/*
 * 功能描述:
 * 创建时间:2016/3/1 14:59:14
 * 编码人员:wangzhia
*/

#include "CDealDataMgr.h"
#include "CPublicMgr.h"
#include "DynBase.h"

CDealDataMgr::CDealDataMgr (map<string,string> classMap)
{
	m_ThreadNum = 0;
	
	m_InterfaceType = 0;
	
	m_ServerExitFlag = false;
	
	m_breakFile = NULL;
	
	memset(m_FreshTime, 0, sizeof(m_FreshTime));
	memset(m_ServerName, 0, sizeof(m_ServerName));
	memset(m_LogName, 0, sizeof(m_LogName));
	memset(m_Indir, 0, sizeof(m_Indir));
	memset(m_Errdir, 0, sizeof(m_Errdir));
	memset(m_Bakdir, 0, sizeof(m_Bakdir));
	memset(m_Breakdir, 0, sizeof(m_Breakdir));
	memset(m_DataName, 0, sizeof(m_DataName));
	memset(m_DealName, 0, sizeof(m_DealName));

	//根据反射实现工厂类
	map<string,string>::iterator iter = classMap.find(string("Mgr"));
	if (iter != classMap.end())
	{
		char className[64] = {0};
		strcpy(className,iter->second.c_str());
		printf("MRG|value:%s\n",className);
		
		Object* poObject = Object::CreateObject(className);
		m_PublicMgr = dynamic_cast<CPublicMgr*>(poObject);
	}
	
	map<string,string>::iterator iter1 = classMap.find(string("Data"));
	if (iter1 != classMap.end())
	{
		strcpy(m_DataName,iter1->second.c_str());
		printf("DATA|value:%s\n",m_DataName);
	}
}

CDealDataMgr::~CDealDataMgr()
{
	printf ("CDealDataMgr destructor.\n");
	while(1)
	{
		if (m_ServerExitFlag)
		{
			DestroyDealData ();
			
			break;
		}
		else
		{
			setThreadState(THREAD_STAT_EXIT);
			this->join();
		}
	}
	if (m_PublicMgr != NULL)
	{
		delete m_PublicMgr;
		m_PublicMgr = NULL;
	}
	
	m_Ibear->Quit();
	delete m_Ibear;
	m_Ibear=NULL;
	delete pInfo;
	pInfo = NULL;
	clearDataList(m_interfaceTable);
	
	if (NULL != m_MutexLocker)
	{
		delete m_MutexLocker;
		m_MutexLocker = NULL;
	}
	
	if (NULL != m_breakFile)
	{
		m_breakFile->Close();
		delete m_breakFile;
		m_breakFile = NULL;
	}
	Destruction();
}

int CDealDataMgr::InitDealData()
{
	Ret iRet = RET_OK;
	
	int iErr = 0;
	
	if (m_ThreadNum < 1 || m_ThreadNum > MAX_BUSINESS_THREAD)
	{
		return RET_FAIL;
	}
	
	CDealData *pDealData = NULL;
	
	m_DealData = (CDealData**)operator new (sizeof(CDealData*) * m_ThreadNum);
	
	//创建数据池
	m_Data_Poor = new Poor<stDealTask>();
	m_Data_Poor->PoorInit();
	for(int i = 0; i < m_ThreadNum; i++)
	{
		m_DealData[i] = new CDealData(m_DataName,m_DealName);
		pDealData = m_DealData[i];
		
		if (0 != pDealData->Init(m_ServerName,i,m_Data_Poor,m_Inicfg))
		{
			printf ("FILE[%s]LINE[%d]ERR_MSG[CDealData Init is failed.]", __FILE__, __LINE__);
			
			break;
		}
		
		if (0 != pDealData->start())
		{
			printf ("FILE[%s]LINE[%d]ERR_MSG[CDealData start is failed.]", __FILE__, __LINE__);
			
			break;
		}
	}
	
	if (0 != iRet)
	{
		for (int i = 0; i < m_ThreadNum; i++)
		{
			CDealData *pDealData = m_DealData[i];
			
			if (pDealData != NULL)
			{
				delete pDealData;
				pDealData = NULL;
			}
		}
	}
	
	return iRet;
}

void CDealDataMgr::DestroyDealData()
{
	printf ("DestroyDealData CDealDataMgr destructor.\n");
	if (m_GiveType == 0 || m_DealType == 3)
		return;
	for (int i = 0; i < m_ThreadNum; i++)
	{
		CDealData *pDealData = m_DealData[i];
		
		if (pDealData != NULL)
		{
			if (pDealData->getDataPoor()->GetRecordNum() <= 0)
			{
				pDealData->setThreadState(THREAD_STAT_EXIT);
				printf("CDealData-wait!\n");
				pDealData->join();
				printf("CDealData-join!\n");
				
				delete pDealData;
				pDealData = NULL;
			}
		}
	}
	delete m_DealData;
	m_DealData = NULL;
	
	if (m_Data_Poor->GetRecordNum()!=0)
	{
		m_Data_Poor->ClearRecord();
	}
	delete m_Data_Poor;
	m_Data_Poor = NULL;
}

int CDealDataMgr::Init(icfg_inifile *inicfg,int cur_processNum)
{
	int ret = 0;
	m_Inicfg = inicfg;
	printf("初始化数据库\n");
	//加载ibear
	pInfo = new bear_db();
	int db_num = atoi(m_Inicfg->icfg_Key_GetValue("SYSTEM","db_num"));
	for (int i = 0;i<db_num;i++)
	{
		char chname[9+1] = {0};
		sprintf(chname, "db_id_%d",i);
		pInfo->db_id[i] = atoi(m_Inicfg->icfg_Key_GetValue("SYSTEM",chname));
	}
	m_Ibear = new ibear(m_Inicfg->icfg_Key_GetValue("SYSTEM","ibear_name"),pInfo);
	strcpy(m_LogName,m_Inicfg->icfg_Key_GetValue("LOG","log_name"));

	//解析配置文件
	m_ProcessNum = atoi(m_Inicfg->icfg_Key_GetValue("PROC_SERVER","process_num","1"));
	m_DealType = atoi(m_Inicfg->icfg_Key_GetValue("PROC_SERVER","deal_type"));
	m_InterfaceType = atoi(m_Inicfg->icfg_Key_GetValue("PROC_SERVER","interface_type"));
	m_ThreadNum = atoi(m_Inicfg->icfg_Key_GetValue("PROC_SERVER","thread_num"));
	m_TabType = atoi(m_Inicfg->icfg_Key_GetValue("PROC_SERVER","tab_type"));
	strcpy(m_Indir,m_Inicfg->icfg_Key_GetValue("PROC_SERVER","indir",""));
	strcpy(m_Bakdir,m_Inicfg->icfg_Key_GetValue("PROC_SERVER","bakdir",""));
	strcpy(m_Errdir,m_Inicfg->icfg_Key_GetValue("PROC_SERVER","errdir",""));
	strcpy(m_Breakdir,m_Inicfg->icfg_Key_GetValue("PROC_SERVER","breakdir",""));
	m_GiveType = atoi(m_Inicfg->icfg_Key_GetValue("PROC_SERVER","is_give"));
	printf("(%s)总进程数:%d,当前进程号:%d\n",m_LogName,m_ProcessNum,cur_processNum);
	if (m_ProcessNum <= 1)
	{
		sprintf(m_ServerName,"%s", m_LogName);
	}
	else
	{
		sprintf(m_ServerName,"%s_%d", m_LogName,cur_processNum);
	}
	
	//调用个性化init();
	map<string,icfg_Map*> user_defined = m_Inicfg->icfg_Group_GetKeys("USER_DEFINED");
	
	icfg_Map *im_num = new icfg_Map();
	strcpy(im_num->chName,"NUM");
	strcpy(im_num->chValue,m_Inicfg->icfg_Key_GetValue("ZK_SERVER","NUM","1"));
	user_defined.insert(pair<string,icfg_Map*>(string("NUM"),im_num));
	
	icfg_Map *im_curnum = new icfg_Map();
	strcpy(im_curnum->chName,"CUR_NUM");
	strcpy(im_curnum->chValue, m_Inicfg->icfg_Key_GetValue("ZK_SERVER","CUR_NUM","0"));
	user_defined.insert(pair<string,icfg_Map*>(string("CUR_NUM"),im_curnum));
	
	icfg_Map *im_intertype= new icfg_Map();
	strcpy(im_intertype->chName,"CUR_NUM");
	sprintf(im_intertype->chValue, "%d", m_InterfaceType);
	user_defined.insert(pair<string,icfg_Map*>(string("interface_type"),im_intertype));

	if (0 != m_PublicMgr->init(m_Ibear,m_ServerName,m_ThreadNum,user_defined))
	{
		return -1;
	}
	
	/*if (m_CfgInfo->java_vm == 1)
	{
		//初始化java虚拟机
		if (0 != createJavaVM())
		{
			printf ("FILE[%s]LINE[%d]ERR_MSG[createJavaVM is failed.]", __FILE__, __LINE__);
			return -1;
		}
	}*/
	
	/*初始化业务处理线程*/
	if (m_GiveType != 0 && m_DealType != 3)
	{
		printf("初始化业务处理线程\n");
		if (InitDealData())
		{
			WriteLog(m_ServerName, LOG_SYS_ERROR, m_ThreadNum, "FILE[%s]LINE[%d]InitDealData error.\n",  __FILE__, __LINE__ );
			return -1;
		}
	}
	
	char m_HostId[64] = {0};
	if (m_Inicfg->icfg_Key_GetValue("ZK_SERVER","HOST_ID") != NULL)
	{
		strcpy(m_HostId,m_Inicfg->icfg_Key_GetValue("ZK_SERVER","HOST_ID"));
		strreplace(m_Indir,m_Indir,(char *)ZK_HOST_ID,m_HostId);
		strreplace(m_Bakdir,m_Bakdir,(char *)ZK_HOST_ID,m_HostId);
		strreplace(m_Errdir,m_Errdir,(char *)ZK_HOST_ID,m_HostId);
		strreplace(m_Breakdir,m_Breakdir,(char *)ZK_HOST_ID,m_HostId);
	}

	switch(m_TabType)
	{
		case BACKUP_TYPE_FILE:
		{
			char tab_path[128] = {0};
			if (m_Inicfg->icfg_Key_GetValue("PROC_SERVER","tab_path") != NULL)
			{
				strcpy(tab_path ,m_Inicfg->icfg_Key_GetValue("PROC_SERVER","tab_path"));
			}
			if (m_Inicfg->icfg_Key_GetValue("ZK_SERVER","HOST_ID") != NULL)
			{
				strreplace(tab_path,tab_path,(char *)ZK_HOST_ID,m_HostId);
			}
			int tab_procmode = 0;
			if (m_Inicfg->icfg_Key_GetValue("PROC_SERVER","tab_procmode") != NULL)
			{
				tab_procmode = atoi(m_Inicfg->icfg_Key_GetValue("PROC_SERVER","tab_procmode"));
			}
			
			int tab_backup_type = 0;
			if (m_Inicfg->icfg_Key_GetValue("PROC_SERVER","tab_backup_type") != NULL)
			{
				tab_backup_type = atoi(m_Inicfg->icfg_Key_GetValue("PROC_SERVER","tab_backup_type"));
			}
			
			int tab_interval = 0;
			if (m_Inicfg->icfg_Key_GetValue("PROC_SERVER","tab_interval") != NULL)
			{
				tab_interval = atoi(m_Inicfg->icfg_Key_GetValue("PROC_SERVER","tab_interval"));
			}
			m_FileBackUp = new CFileBackUp();
			stBakConfigInfo *pBakInfo= new stBakConfigInfo();
			
			pBakInfo->type = m_TabType;
			pBakInfo->backupType = tab_backup_type;
			strcpy(pBakInfo->bakDir,tab_path);
			pBakInfo->interval = tab_interval;
			pBakInfo->writeMode = tab_procmode;
			m_FileBackUp->Init(pBakInfo);
			m_FileBackUp->Process();
			break;
		}
		case BACKUP_TYPE_TABLE:
			break;
		case NO_BACKUP:
			break;
		default:
		{
			WriteLog(m_ServerName, LOG_SYS_ERROR, m_ThreadNum, "FILE[%s]LINE[%d]ERR_MSG[backup_type<%d> is failed.]\n",  __FILE__, __LINE__, m_TabType);
			break;
		}
	}

	return 0;
}

int CDealDataMgr::Process ()
{
	int iRet = RET_OK;
	
	switch (m_InterfaceType)
	{
		case INTERFACE_TYPE_FILE:
		{
			m_MutexLocker = new ilog_CMutexLock();
			
			if(m_DealType == 1)
			{
				iRet = this->start();
			}
			else if (m_DealType == 2)
			{
				this->time_run();
			}
			else 
			{
				this->other_run();
			}
			break;
		}
		case INTERFACE_TYPE_TABLE:
		{
			if(m_DealType == 1)
			{
				iRet = this->start();
			}
			else if (m_DealType == 2)
			{
				this->time_run();
			}
			else 
			{
				this->other_run();
			}
			break;
		}
		case INTERFACE_TYPE_FEDX:
		{
			m_MutexLocker = new ilog_CMutexLock();
			
			if(m_DealType == 1)
			{
				iRet = this->start();
			}
			else if (m_DealType == 2)
			{
				this->time_run();
			}
			else
			{
				this->other_run();
			}
			break;
		}
		default:
			WriteLog(m_ServerName, LOG_SYS_ERROR, m_ThreadNum, "FILE[%s]LINE[%d]MSG[m_InterfaceType<%d> is failed.]\n",  __FILE__, __LINE__, m_InterfaceType);
			break;
	}
	return iRet;
}

int CDealDataMgr::DealFileList()
{
	int iRet = 0;
	char src_file[1024] = {0};
	char des_file[1024] = {0};
#ifdef _SDFS_BAK_
	GDF_FILE *file = new SDFS_GDF_FILE();
#else
	GDF_FILE *file = new SYSTEM_GDF_FILE();
#endif
	if (0 != file->getFileList(m_Indir))
	{
		printf("getFileList error\n");
		return -1;
	}
	
	if ((file->mFileList).size() == 0)
	{
		sleep(1);
	}
	
	for (vector<stFileInfo *>::iterator it = file->mFileList.begin();it!=file->mFileList.end();it++)
	{
		stFileInfo *sf = *it;
#ifdef _DEBUG_INFO_
		printf("%s,%s\n",sf->sFileName,sf->sModifyTime);
#endif
		//断点文件初始化(每次重新打开)
		m_breakFile = new SYSTEM_GDF_FILE(m_Breakdir,"breakpoint.log");
		m_breakFile->Open("w+");
		
		//开始处理
		switch (m_InterfaceType)
		{
			case INTERFACE_TYPE_FILE:
			{
				iRet = dealFile(sf->sFileName);
				break;
			}
			case INTERFACE_TYPE_FEDX:
			{
				iRet = dealFedx(sf->sFileName);
				break;
			}
			default:
			{
				WriteLog(m_ServerName, LOG_SYS_ERROR, m_ThreadNum, "FILE[%s]LINE[%d]ERR_MSG[interface_type<%d> is failed.]\n",  __FILE__, __LINE__, m_InterfaceType);
				setThreadState(THREAD_STAT_EXIT);
				break;
			}
		}
		if (0 != iRet)
		{
			//文件处理失败，挪入errdir下
			writeBreakLog(sf->sFileName,BREAK_POINT_4);
			sprintf(src_file, "%s/%s",m_Indir,sf->sFileName);
			sprintf(des_file, "%s/%s",m_Errdir,sf->sFileName);
		}
		else
		{
			//文件处理成功，挪入bakdir下
			writeBreakLog(sf->sFileName,BREAK_POINT_3);
			sprintf(src_file, "%s/%s",m_Indir,sf->sFileName);
			sprintf(des_file, "%s/%s",m_Bakdir,sf->sFileName);
			
		}
		if (0 != file->Move(src_file, des_file))
		{
			printf("Move[%s]到[%s]失败！！！\n",src_file, des_file);
			continue;
		}
		writeBreakLog(sf->sFileName,BREAK_POINT_0);
	}
	
	if ((file->mFileList).size()>0)
	{
		clearDataList(file->mFileList);
	}
	delete file;
	file = NULL;
	return 0;
}

int CDealDataMgr::dealFile(char *filename)
{
	int iRet = RET_OK;
	stDealTask *pTask = NULL;
	char *strBuff;
	char stBuffs[BG_MAX_BUFFERLEN];

	writeBreakLog(filename,BREAK_POINT_1);
#ifdef _SDFS_BAK_
	GDF_FILE *file = new SDFS_GDF_FILE(m_Indir,filename);
#else
	GDF_FILE *file = new SYSTEM_GDF_FILE(m_Indir,filename);
#endif
	if (0!=file->Open("r"))
	{
		return -1;
	}
	while(!file->IsFileEnd())
	{
		memset (stBuffs, 0, sizeof(stBuffs));
		file->Gets(stBuffs,BG_MAX_BUFFERLEN);
		if (NULL == stBuffs || strlen(stBuffs) <= 0)
		{
			break;
		}
		strBuff = (char*)malloc(BG_MAX_BUFFERLEN*sizeof(char));
		sprintf(strBuff, "%s", stBuffs);
		if (getThreadState() == THREAD_STAT_EXIT)
		{
			break;
		}
		pTask = new stDealTask();
		if (pTask == NULL)
		{
			iRet = RET_FAIL;
			break;
		}
		pTask->length = strlen(strBuff);
		pTask->sBuff = strBuff;
		sprintf(pTask->type,"%s",filename);
		pTask->isEnd = false;

		if (m_GiveType == 1)
		{
			//将处理数据放入池中
			m_Data_Poor->PutRecord(pTask);
		}
		else if (m_GiveType == 2)
		{
			//数据按分发标示进行分发
			pTask->dataNum = 0;
			m_DealData[pTask->dataNum]->PutTask(pTask);
		}
		else
		{
			pTask->code = m_PublicMgr->process();
		}
	}
	delete file;
	file = NULL;
	
	return iRet ;
}

int CDealDataMgr::dealFedx(char *filename)
{
	int iRet = RET_OK;
	stDealTask *pTask = NULL;
	char stBuffs[BG_MAX_BUFFERLEN];
	int len = 0;
	short real_len = 0;
	
	writeBreakLog(filename,BREAK_POINT_1);
	SDFS_GDF_FILE *file = new SDFS_GDF_FILE(m_Indir,filename);
	if (0!=file->Open("rb"))
	{
		return -1;
	}
	while(file->Read(stBuffs,1,4) >= 0)
	{
		memcpy( &real_len, stBuffs+2, 2);
		real_len = ntohs(real_len);
		if( real_len != file->Read( stBuffs+4, 1, real_len))
		{
			return -1;
		}
		len = real_len+4;
		if (getThreadState() == THREAD_STAT_EXIT)
		{
			break;
		}
		pTask = new stDealTask();
		if (pTask == NULL)
		{
			iRet = RET_FAIL;
			break;
		}

		pTask->sBuff = stBuffs;
		
		pTask->length = len;
		sprintf(pTask->type,"%s",filename);
		pTask->isEnd = false;
		
		if (m_GiveType == 1)
		{
			//将处理数据放入池中
			m_Data_Poor->PutRecord(pTask);
		}
		else if (m_GiveType == 2)
		{
			//数据按分发标示进行分发
			pTask->dataNum = 0;
			m_DealData[pTask->dataNum]->PutTask(pTask);
		}
		else
		{
			pTask->code = m_PublicMgr->process();
		}
	}
	delete file;
	file = NULL;
	
	return iRet ;
}

void CDealDataMgr::run()
{
	int iRet = 0, iFlag = 0;
	Ret ret = RET_OK;
	
	char fileName[1024] = {0};
	CDealData *pDealData = NULL;
	stDealTask *pTask = NULL;
	
	while (getThreadState() == THREAD_STAT_RUNNING)
	{
		if (strcmp(CurrSysDate(4),m_FreshTime) == 0)
		{
			for (int i = 0; i < m_ThreadNum; i++)
			{
#ifdef _DEBUG_INFO_
	printf("刷新%d个线程的第[%d]个信控配置%s,%s\n",m_ThreadNum,i,CurrSysDate(4),m_FreshTime);
#endif
				pDealData = m_DealData[i];
				printf("[%d]刷新配置完成!!!\n",i);
			}
			sleep(3);
		}
		if (m_GiveType == 1)
		{
			if (m_Data_Poor->GetRecordNum() > 100)
			{
				usleep(1000);
				continue;
			}
		}
		switch (m_InterfaceType)
		{
			case INTERFACE_TYPE_FILE:
			{
				if (0 != DealFileList())
				{
					break;
				}
				break;
			}
			case INTERFACE_TYPE_TABLE:
			{
				//取数据到内存中
				m_PublicMgr->ProInterface(&m_interfaceTable);
				if (m_interfaceTable.size() == 0)
				{
					usleep (1000);
					continue;
				}
				
				//数据处理
				for (vector<stDealTask*>::iterator dataIter = m_interfaceTable.begin(); dataIter != m_interfaceTable.end(); dataIter++)
				{
					if (getThreadState() == THREAD_STAT_EXIT)
					{
						break;
					}
					pTask = new stDealTask();
					memcpy(pTask, *dataIter, sizeof(stDealTask));
					if (pTask == NULL)
					{
						ret = RET_FAIL;
						break;
					}
					pTask->isEnd = false;
					
					if (m_GiveType == 1)
					{
						//将处理数据放入池中
						m_Data_Poor->PutRecord(pTask);
					}
					else if (m_GiveType == 2)
					{
						while(1)
						{
							if (m_DealData[pTask->dataNum]->GetQueueNum() == 0)
							{
								break;
							}
							sleep(1);
						}
						m_DealData[pTask->dataNum]->PutTask(pTask);
					}
					else
					{
						pTask->code = m_PublicMgr->process();
					}
				}
				
				switch(m_TabType)
				{
					case BACKUP_TYPE_FILE:
					{
						for (vector<stDealTask*>::iterator iter = m_interfaceTable.begin(); iter != m_interfaceTable.end(); iter++)
						{
							stDealTask *dt = *iter;
							stFileBakInfo *tFileInfo = new stFileBakInfo();
							tFileInfo->fileEnd = false;
							strcpy(tFileInfo->fileNameHead,"bill");
							sprintf(tFileInfo->fileName,"%s",tFileInfo->fileNameHead);
							sprintf(tFileInfo->fileContent,"%s\n",(char*)dt->sBuff);
							m_FileBackUp->WriteFile(tFileInfo);
						}
						break;
					}
					case BACKUP_TYPE_TABLE:
						break;
					case NO_BACKUP:
						break;
					default:
					{
						WriteLog(m_ServerName, LOG_SYS_ERROR, m_ThreadNum, "FILE[%s]LINE[%d]ERR_MSG[backup_type<%d> is failed.]\n",  __FILE__, __LINE__, m_InterfaceType);
						
						iFlag = THREAD_STAT_EXIT;
						break;
					}
				}
				
				clearDataList(m_interfaceTable);
				break;
			}
			case INTERFACE_TYPE_FEDX:
			{
				if (0 != DealFileList())
				{
					break;
				}
				break;
			}
			default:
			{
				WriteLog(m_ServerName, LOG_SYS_ERROR, m_ThreadNum, "FILE[%s]LINE[%d]ERR_MSG[interface_type<%d> is failed.]\n",  __FILE__, __LINE__, m_InterfaceType);
				
				setThreadState(THREAD_STAT_EXIT);
				
				break;
			}
		}
		
		if (getThreadState() == THREAD_STAT_EXIT)
		{
			m_ServerExitFlag = true;
				
			break;
		}
	}
	
	if (getThreadState() == THREAD_STAT_EXIT)
	{
		m_ServerExitFlag = true;
	}
	
	return;
}

void CDealDataMgr::time_run()
{
	int iRet = 0, iFlag = 0;
	Ret ret = RET_OK;
	char fileName[1024] = {0};

	CDealData *pDealData = NULL;
	stDealTask *pTask = NULL;
	
	switch (m_InterfaceType)
	{
		case INTERFACE_TYPE_FILE:
		{
			printf("定时无文件接口方式！！！\n");
			break;
		}
		case INTERFACE_TYPE_TABLE:
		{
			char sql_key[64] = {0};
			iRet = m_PublicMgr->ProTimeList(sql_key);
			if (0 < iRet)
			{
				//查不到数据请返回0或大于0的值，否则会一直打日志
				WriteLog(m_ServerName, LOG_SYS_ERROR, m_ThreadNum, "FILE[%s]LINE[%d]MSG[ProcInterfaceTable return value is <%d>]\n",  __FILE__, __LINE__, iRet);
				break;
			}
			//数据处理
			vector<string> v_row;
			while (1)
			{
				if (m_GiveType == 1)
				{
					if (m_Data_Poor->GetRecordNum() > 100)
					{
						usleep(1000);
						continue;
					}
				}
				iRet = m_Ibear->fetchRow(sql_key,&v_row);
				if (iRet < 0)
				{
					printf("m_Ibear->fetchRow[%s] ERRNO[%s]ERR_MSG[%s]\n",sql_key,m_Ibear->no_error,m_Ibear->sql_error);
					break;
				}
				else if (iRet == 1)
				{
					break;
				}
				pTask = new stDealTask();
				pTask->isEnd = false;
				pTask->dataNum = -1;
				iRet = m_PublicMgr->setTask(v_row,pTask);
				if (iRet < 0)
				{
					printf("m_PublicMgr->setTask ERRNO[%s]ERR_MSG[%s]\n",m_Ibear->no_error,m_Ibear->sql_error);
					break;
				}
				
				if (m_GiveType == 1)
				{
					//将处理数据放入池中
					m_Data_Poor->PutRecord(pTask);
					if (iRet != RET_OK)
					{
						printf("UpdPoor_Status is error %d \n",iRet);
						break;
					}
				}
				else if (m_GiveType == 2)
				{
					if (pTask->dataNum == -1)
					{
						pTask->dataNum = atoi(pTask->dataOnly)%m_ThreadNum;
					}
					while(1)
					{
						if (m_DealData[pTask->dataNum]->GetQueueNum() < 5000)
						{
							break;
						}
						sleep(1);
					}
					m_DealData[pTask->dataNum]->PutTask(pTask);
				}
				else
				{
					pTask->code = m_PublicMgr->process();
				}
			}
			break;
		}
		case INTERFACE_TYPE_FEDX:
		{
			printf("定时无文件接口方式！！！\n");
			break;
		}
		default:
		{
			WriteLog(m_ServerName, LOG_SYS_ERROR, m_ThreadNum, "FILE[%s]LINE[%d]ERR_MSG[interface_type<%d> is failed.]\n",  __FILE__, __LINE__, m_InterfaceType);
			
			break;
		}
	}
	
	//等待子线程处理完毕,结束主线程
	if (m_GiveType == 1)
	{
		for(int i = 0; i < m_ThreadNum; i++)
		{
			pTask = new stDealTask();
			pTask->isEnd = true;
			m_Data_Poor->PutRecord(pTask);
		}
		while (1)
		{
			if (m_Data_Poor->GetRecordNum() == 0)
			{
				CDealData *pDealData = NULL;
				for(int i = 0; i < m_ThreadNum; i++)
				{
					pDealData = m_DealData[i];
					stMessage *stmsg = pDealData->getMessage();
					if (stmsg->state != MESSAGE_STATE_OVER)
					{
						m_ServerExitFlag = false;
						break;
					}
					else
					{
						m_ServerExitFlag = true;
					}
				}
				if (m_ServerExitFlag)
				{
					break;
				}
			}
			sleep(1);
		}
	}
	else if (m_GiveType == 2)
	{
		for(int i = 0; i < m_ThreadNum; i++)
		{
			pTask = new stDealTask();
			pTask->isEnd = true;
			m_DealData[i]->PutTask(pTask);
		}
		int dealNum[MAX_BUSINESS_THREAD];
		CDealData *pDealData = NULL;
		while (1)
		{
			for(int i = 0; i < m_ThreadNum; i++)
			{
				pDealData = m_DealData[i];
				stMessage *stmsg = pDealData->getMessage();

				if (stmsg->state == MESSAGE_STATE_OVER)
				{
					dealNum[i] = 1;
				}
			}
			
			bool dealEnd = true;
			for(int j = 0; j < m_ThreadNum; j++)
			{
				if (dealNum[j] != 1)
				{
					dealEnd = false;
					break;
				}
			}
			if (dealEnd)
			{
				break;
			}
			else
			{
				usleep (10);
			}
		}
	}
}

void CDealDataMgr::other_run()
{
	int iRet = 0;
	stDealTask *pTask = NULL;
	switch (m_InterfaceType)
	{
		case INTERFACE_TYPE_FILE:
		{
			iRet = m_PublicMgr->process();
			break;
		}
		case INTERFACE_TYPE_TABLE:
		{
			iRet = m_PublicMgr->process();
			break;
		}
		case INTERFACE_TYPE_FEDX:
		{
			iRet = m_PublicMgr->process();
			break;
		}
		default:
		{
			WriteLog(m_ServerName, LOG_SYS_ERROR, m_ThreadNum, "FILE[%s]LINE[%d]ERR_MSG[interface_type<%d> is failed.]\n",  __FILE__, __LINE__, m_InterfaceType);
			
			break;
		}
	}
	if (0 != iRet)
	{
		WriteLog(m_ServerName, LOG_SYS_ERROR, m_ThreadNum, "FILE[%s]LINE[%d]MSG[process return value is <%d>]\n",  __FILE__, __LINE__, iRet);
	}
	m_ServerExitFlag = true;
}

Ret CDealDataMgr::writeBreakLog(const char *strFileName, const char chStepFlag)
{
	Ret iRet = RET_OK;
	char chTmp[1024] = {0};
	int flag = 0;
	char fileName_tmp[1024] = {0};
	
	memset (chTmp, 0, sizeof(chTmp));
	
	sprintf(fileName_tmp, "%s/breakpoint.log",m_Breakdir);
	sprintf (chTmp, "%c#%s#\n", chStepFlag, strFileName);
	
	m_MutexLocker->Lock();
	if (iRet != m_breakFile->Truncate(fileName_tmp,sizeof(chTmp)))
	{
		return iRet ;
	}
	m_breakFile->Seek(0, SEEK_SET);
	m_breakFile->Write(chTmp, sizeof(chTmp), 1);
	m_breakFile->Fflush();
	m_MutexLocker->UnLock();
	return iRet;
}
