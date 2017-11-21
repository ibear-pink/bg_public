/*
 * 功能描述:
 * 创建时间:2016/3/1 14:59:14
 * 编码人员:wangzhia
*/

#ifndef _CDEAL_DATA_H_
#define _CDEAL_DATA_H_

#include "CPublicData.h"

using namespace std;

class CDealData : public Thread
{
public:
	CDealData(char *className,char *className_deal);
	
	~CDealData();
	
	void run(void);
	
	int Init(char *logname,int thread_num,Poor<stDealTask> *pDataPoor,icfg_inifile *inicfg);
	
	int DealRecord (stDealTask *pTask);
	
	stMessage * getMessage ();
	
	void addDealNum_Message();
	
	Poor<stDealTask> * getDataPoor();
	
	void PutTask (stDealTask *pTask) {m_DealQueue.push_first (pTask);}
	
	stDealTask *GetTask () {return m_DealQueue.pop();}
	
	int GetQueueNum () {return m_DealQueue.sum();};
	
private:
	int m_ThreadNum;//线程数量
	int m_CurThreadNum;//当前线程号
	int m_DealType;//1.实时,2.定时
	int m_GiveType;//分发方式
	char m_ServerName[64];
	int m_InterfaceType;//1 文件接口，2 接口表

	ibear *m_Ibear;
	icfg_inifile *m_iniCfg;
	Poor<stDealTask> *m_Data_Poor;
	deque2<stDealTask> m_DealQueue;

	CPublicData *m_PublicData;
	stMessage *m_stMsg;
	bear_db * pInfo;
};

#endif
