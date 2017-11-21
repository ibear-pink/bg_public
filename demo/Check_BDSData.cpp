
/*
 * 功能描述:
 * 创建时间:
 * 编码人员:自动生成
*/
#include "Check_BDSData.h"
#include "DynBase.h"

IMPLEMENT_CLASS(Check_BDSData)

Check_BDSData::Check_BDSData()
{
	memset(m_BusinessType, 0, sizeof(m_BusinessType));
}

Check_BDSData::~Check_BDSData()
{

}

int Check_BDSData::init(ibear *br,char *logName,int cur_thread_num,stMessage *stmsg,map<string,icfg_Map*> user_defined)
{
	printf("Check_BDSData::init(%s)\n",logName);
	m_LogName = string(logName);
	m_CurThreadNum = cur_thread_num;
	m_Ibear = br;
	m_stMsg=stmsg;
	map<string,icfg_Map*>::iterator it = user_defined.find(string("business_type"));
	if (it!=user_defined.end())
	{
		strcpy(m_BusinessType,it->second->chValue);
	}
	else
	{
		return RET_FAIL;
	}
	
	return RET_OK;
}

int Check_BDSData::process(stDealTask *pTask)
{
	int iRet = 0;
	printf("[%d]Check_BDSData::process[%s][%s]\n",m_CurThreadNum,m_BusinessType,(char*)pTask->sBuff);
	WriteLog(m_LogName, LOG_SYS_RUNNING,m_CurThreadNum, "[%d]Check_BDSData::process[%s]\n",m_CurThreadNum,(char*)pTask->sBuff);
	return iRet;
}


