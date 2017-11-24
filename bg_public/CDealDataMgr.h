/*
 * ��������:
 * ����ʱ��:2016/3/1 14:59:14
 * ������Ա:wangzhia
*/

#ifndef _DEAL_DATA_MGR_H_
#define _DEAL_DATA_MGR_H_

#include "CPublicMgr.h"
#include "CFileBackUp.h"

using namespace std;

class CDealDataMgr : public Thread
{
public:
	
	CDealDataMgr(map<string,string> classMap);
	
	~CDealDataMgr();
	
	void run (void);
	
	void time_run (void);
	
	void other_run(void);
	
	int Process ();
	
	int Init(icfg_inifile *inicfg,int cur_processNum);

	int Init(icfg_inifile *inicfg);

	int InitDealData();
	
	int DealFileList();
	
	int dealFile(char *filename);
	int dealFedx(char *filename);
	Ret writeBreakLog(const char *strFileName, const char chStepFlag);
private:
	void DestroyDealData();

public:
	//ҵ����ṹ��
	CDealData **m_DealData;
	Poor<stDealTask> *m_Data_Poor;
protected:
	
	friend int get_ascii_value_by_pos (char *buff, int pos, char *value);
private:
	CPublicMgr *m_PublicMgr;
	ibear *m_Ibear;
	char m_FreshTime[6+1];
	char m_LogName[64];
	int m_ProcessNum;
	icfg_inifile *m_Inicfg;
	bear_db * pInfo;
	SDFS sp;
	
	int m_ThreadNum;		//�߳���
	int m_DealType;			//1.ʵʱ�ӿ�, 2.��ʱ�ӿ�
	int m_InterfaceType;	//1 �ļ��ӿڣ�2 �ӿڱ�
	int m_GiveType;			//���ݷַ���ʽ
	int m_TabType;			//��������0:������ ,1���ļ���ʽ��2����ʽ
	int m_logThreadNum;		//��־�߳���
	char m_Indir[1024];		//����ļ�·��
	char m_Bakdir[1024];	//��ڱ���·��
	char m_Errdir[1024];	//�����ļ�·��
	char m_Breakdir[1024];	//�ϵ��ļ�·��
	char m_ServerName[1024];//ҵ���������
	
	map<int ,int> m_ParseSize;
	map<int ,int> m_ParseSeq;
	map<int ,int> m_ParseMain;
	
	vector<stDealTask*> m_interfaceTable;

	bool m_ServerExitFlag;

	CFileBackUp *m_FileBackUp;
	GDF_FILE *m_breakFile;//�ϵ��ļ�
	ilog_CMutexLock *m_MutexLocker;
	char m_DataName[64];
	char m_DealName[64];
};

#endif
