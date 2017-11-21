/*
 * ��������:
 * ����ʱ��:2016/3/1 14:59:14
 * ������Ա:wangzhia
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
	int m_ThreadNum;//�߳�����
	int m_CurThreadNum;//��ǰ�̺߳�
	int m_DealType;//1.ʵʱ,2.��ʱ
	int m_GiveType;//�ַ���ʽ
	char m_ServerName[64];
	int m_InterfaceType;//1 �ļ��ӿڣ�2 �ӿڱ�

	ibear *m_Ibear;
	icfg_inifile *m_iniCfg;
	Poor<stDealTask> *m_Data_Poor;
	deque2<stDealTask> m_DealQueue;

	CPublicData *m_PublicData;
	stMessage *m_stMsg;
	bear_db * pInfo;
};

#endif
