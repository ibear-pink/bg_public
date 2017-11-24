/*
 * 功能描述:
 * 创建时间:2016/3/1 14:59:14
 * 编码人员:wangzhia
*/

#include "CDealData.h"

CDealData::CDealData(char *className,char *className_deal)
{
	memset (m_ServerName, 0, sizeof(m_ServerName));
	
	m_InterfaceType = 0;
	
	m_stMsg = new stMessage();
	//根据反射实现工厂类
	Object* poObject = Object::CreateObject(className);
	m_PublicData = dynamic_cast<CPublicData*>(poObject);
	
	Object* dealObject = Object::CreateObject(className_deal);
}

CDealData::~CDealData()
{
	printf ("CDealData[%d] destructor.\n",m_ThreadNum);
	
	delete m_stMsg;
	m_stMsg = NULL;
	
	if (NULL != m_PublicData)
	{
		delete  m_PublicData;
		m_PublicData = NULL;
	}
	m_Ibear->Quit();
	delete m_Ibear;
	m_Ibear = NULL;
	delete pInfo;
	pInfo = NULL;
	printf ("CDealData[%d] destructor. end .\n",m_ThreadNum);
}

int CDealData::Init (char *logname,int thread_num,Poor<stDealTask> *pDataPoor,icfg_inifile *inicfg)
{
	int iRet = 0;
	
	m_iniCfg = inicfg;
	//解析配置文件
	m_DealType = atoi(m_iniCfg->icfg_Key_GetValue("PROC_SERVER","deal_type"));
	m_InterfaceType = atoi(m_iniCfg->icfg_Key_GetValue("PROC_SERVER","interface_type"));
	m_ThreadNum = atoi(m_iniCfg->icfg_Key_GetValue("PROC_SERVER","thread_num","1"));
	m_GiveType = atoi(m_iniCfg->icfg_Key_GetValue("PROC_SERVER","is_give"));

	strcpy(m_ServerName,logname);
	m_Data_Poor = pDataPoor;
	m_CurThreadNum = thread_num;
	
	printf("初始线程[%d]的数据库连接\n",m_CurThreadNum);
	//加载ibear
	pInfo = new bear_db();
	int db_num = atoi(m_iniCfg->icfg_Key_GetValue("SYSTEM","db_num"));
	for (int i = 0;i<db_num;i++)
	{
		char chname[128] = {0};
		sprintf(chname, "db_id_%d",i);
		pInfo->db_id[i] = atoi(m_iniCfg->icfg_Key_GetValue("SYSTEM",chname));
	}
	m_Ibear = new ibear(m_iniCfg->icfg_Key_GetValue("SYSTEM","ibear_name"),pInfo);
	
	//调用个性化init();
	map<string,icfg_Map*> user_defined = m_iniCfg->icfg_Group_GetKeys("USER_DEFINED");
	m_PublicData->init(m_Ibear, m_ServerName, m_CurThreadNum,m_stMsg,user_defined);
	m_DealQueue.setMaxNum (MAX_DEQUE_NUM);
	m_DealQueue.setDelayTime(1);
	printf("加载完毕!!!\n");
	
	return 0;
}

void CDealData::run(void)
{
	stDealTask *pTask = NULL;
	
	while (getThreadState() != THREAD_STAT_EXIT)
	{
		if (m_GiveType == 1)
		{
			pTask = m_Data_Poor->GetRecord();
		}
		else if (m_GiveType == 2)
		{
			pTask = GetTask();
		}
		if (pTask == NULL)
		{
			usleep (10);
			continue;
		}
		if (pTask->isEnd)
		{
			m_stMsg->state = MESSAGE_STATE_OVER;
			if (pTask->sBuff != NULL)
			{
				free(pTask->sBuff);
				pTask->sBuff = NULL;
			}
			delete pTask;
			pTask = NULL;
			return;
		}
		else
		{
			DealRecord(pTask);
		}
		if (pTask->sBuff != NULL)
		{
			free(pTask->sBuff);
			pTask->sBuff = NULL;
		}
		delete pTask;
		pTask = NULL;
	}
	return;
}

int CDealData::DealRecord (stDealTask *pTask)
{
	int iRet = 0;
	do
	{
		//业务逻辑处理
		switch (m_InterfaceType)
		{
			case INTERFACE_TYPE_FILE:
			{
				//调用个性化Process
				iRet = m_PublicData->fprocess(pTask);
				addDealNum_Message();
				break;
			}
			case INTERFACE_TYPE_TABLE:
			{
				//调用个性化Process
				iRet = m_PublicData->process(pTask);
				addDealNum_Message();
				break;
			}
			default:
				WriteLog(m_ServerName, LOG_SYS_ERROR, m_CurThreadNum, "FILE[%s]LINE[%d]ERR_MSG[m_InterfaceType<%d> is failed.]\n",  __FILE__, __LINE__, m_InterfaceType);
				iRet = -1;
				break;
		}
	} while (0);
	
	if (0 != iRet)
	{
		WriteLog(m_ServerName, LOG_SYS_ERROR, m_CurThreadNum, "FILE[%s]LINE[%d]ERR_MSG[Err_Code<%d>Err_msg<%s>]\n",  __FILE__, __LINE__, m_stMsg->state,m_stMsg->err_msg);
	}
	return iRet;
}

stMessage * CDealData::getMessage ()
{
	return m_stMsg;
}

void CDealData::addDealNum_Message()
{
	m_stMsg->deal_num++;
}

Poor<stDealTask> * CDealData::getDataPoor()
{
	return m_Data_Poor;
}







