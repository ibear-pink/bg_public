
/*
 * ��������:
 * ����ʱ��:
 * ������Ա:�Զ�����
*/
#ifndef _CHECK_BDS_DATA_H_
#define _CHECK_BDS_DATA_H_

#include "CPublicData.h"
#include "DynBase.h"

using namespace std;

class Check_BDSData:public CPublicData,public Object
{
	DECLARE_CLASS(Check_BDSData)
public:

	Check_BDSData();
	~Check_BDSData();
	int init(ibear *br,char *logName,int cur_thread_num,stMessage *stmsg,map<string,icfg_Map*> user_defined);
	int process(stDealTask *pTask);
	
public:
	ibear *m_Ibear;
	stMessage *m_stMsg;
	string m_LogName;
	int m_CurThreadNum;
	char m_BusinessType[2+1];
};

#endif


