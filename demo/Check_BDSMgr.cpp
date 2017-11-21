
/*
 * 功能描述:一致性比对、修复
 * 创建时间:
 * 编码人员:自动生成
*/

#include "Check_BDSMgr.h"
#include "CDealData.h"

IMPLEMENT_CLASS(Check_BDSMgr)

Check_BDSMgr::Check_BDSMgr()
{
	memset(m_BusinessType, 0, sizeof(m_BusinessType));
}

Check_BDSMgr::~Check_BDSMgr()
{

}

int Check_BDSMgr::init(ibear *br, char *logName,int thread_num,map<string,icfg_Map*> user_defined)
{
	printf("Check_BDSMgr->init(%s|%d)\n",logName,thread_num);
	m_LogName = string(logName);
	m_ThreadNum = thread_num;
	m_Ibear = br;
	map<string,icfg_Map*>::iterator it = user_defined.find(string("business_type"));
	if (it!=user_defined.end())
	{
		strcpy(m_BusinessType,it->second->chValue);
	}
	return 0;
}

int Check_BDSMgr::process()
{
	printf("Check_BDSMgr->process[%s]\n",m_BusinessType);
	return 0;
}

int Check_BDSMgr::ProTimeList(char *sql_key)
{
	int iRet = 0;
	strcpy(sql_key,"SEL_INTERFACE");
	map<string,string> inParams;
	vector<vector<string> > list;
	//inParams.insert(pair<string, string>(string("PARAM_TYPE"),string(m_BusinessType)));
	iRet = m_Ibear->queryForList(sql_key,inParams);
	if (iRet != 0)
	{
		printf("ProTimeList ERRNO[%s]ERR_MSG[%s]\n",m_Ibear->no_error,m_Ibear->sql_error);
		return iRet;
	}

	return iRet;
}

int Check_BDSMgr::setTask(vector<string> v_row,stDealTask *pTask)
{
	for (vector<string>::iterator it = v_row.begin();it!=v_row.end();it++)
	{
		string text = *it;
		char data_Buff[1024] = {0};
		sprintf(data_Buff,"%s|%s",data_Buff,text.c_str());
		memcpy(pTask->sBuff,data_Buff,1024);
	}
	strcpy(pTask->dataSeq, v_row[0].c_str());
	strcpy(pTask->dataOnly,v_row[1].c_str());
	pTask->dataNum = atoi(pTask->dataOnly)%m_ThreadNum;

	return 0;
}

int Check_BDSMgr::ProInterface(vector<stDealTask*> *values)
{
	int iRet = 0;
	sleep(1);
	printf("Check_BDSMgr->ProInterface\n");
	map<string,string> inParams;
	vector<vector<string> > list;
	inParams.insert(pair<string, string>(string("PARAM_TYPE"),string(m_BusinessType)));
	if (0!=m_Ibear->queryForList("SEL_INTERFACE",inParams,&list))
	{
		printf("ProInterface ERRNO[%s]ERR_MSG[%s]\n",m_Ibear->no_error,m_Ibear->sql_error);
		WriteLog(m_LogName, LOG_SYS_ERROR, m_ThreadNum, "ProInterface[%s] ERRNO[%s]ERR_MSG[%s]\n",m_BusinessType,m_Ibear->no_error,m_Ibear->sql_error);
		return -1;
	}
	for(vector<vector<string> >::iterator it=list.begin();it!=list.end();it++)
	{
		vector<string> row = *it;
		stDealTask *pTask = new stDealTask();
		strcpy(pTask->dataSeq,row[0].c_str());
		strcpy(pTask->dataOnly,row[1].c_str());
		memcpy(pTask->sBuff,row[2].c_str(),1024);
		pTask->dataNum = atoi(pTask->dataOnly)%m_ThreadNum;
		(*values).push_back(pTask);
	}

	return iRet;
}

int Check_BDSMgr::UpdPoorStatus(stDealTask *pTask)
{
	int iRet = 0;
	printf("Check_BDSMgr->UpdPoorStatus[%s]\n",pTask->dataSeq);
	return iRet;
}




