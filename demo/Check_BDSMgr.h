
/* 
 * 功能描述: 
 * 创建时间:
 * 编码人员:自动生成
*/
#ifndef _CHECK_BDS_MANAGER_H_
#define _CHECK_BDS_MANAGER_H_

#include "CPublicMgr.h"
#include "DynBase.h"

using namespace std;

class Check_BDSMgr:public CPublicMgr,public Object
{
	DECLARE_CLASS(Check_BDSMgr)
public:
	
	Check_BDSMgr();
	~Check_BDSMgr();
    
	int init(ibear *br,char *logName,int thread_num,map<string,icfg_Map*> user_defined);

	int process();
	
	int ProInterface(vector<stDealTask*> *values);
	
	int ProTimeList(char *sql_key);
	
	int setTask(vector<string> v_row,stDealTask *pTask);

	int UpdPoorStatus(stDealTask *pTask);
	
public:
	string m_LogName;
	ibear *m_Ibear;
	int m_ThreadNum;
	char m_BusinessType[1+1];
};
#endif


