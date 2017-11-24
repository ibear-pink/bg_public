/*
 * 功能描述:
 * 创建时间:2016/3/1 14:59:14
 * 编码人员:wangzhia
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
	//业务处理结构体
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
	
	int m_ThreadNum;		//线程数
	int m_DealType;			//1.实时接口, 2.定时接口
	int m_InterfaceType;	//1 文件接口，2 接口表
	int m_GiveType;			//数据分发方式
	int m_TabType;			//备份类型0:不备份 ,1：文件方式，2：表方式
	int m_logThreadNum;		//日志线程数
	char m_Indir[1024];		//入口文件路径
	char m_Bakdir[1024];	//入口备份路径
	char m_Errdir[1024];	//错误文件路径
	char m_Breakdir[1024];	//断电文件路径
	char m_ServerName[1024];//业务服务器名
	
	map<int ,int> m_ParseSize;
	map<int ,int> m_ParseSeq;
	map<int ,int> m_ParseMain;
	
	vector<stDealTask*> m_interfaceTable;

	bool m_ServerExitFlag;

	CFileBackUp *m_FileBackUp;
	GDF_FILE *m_breakFile;//断点文件
	ilog_CMutexLock *m_MutexLocker;
	char m_DataName[64];
	char m_DealName[64];
};

#endif
