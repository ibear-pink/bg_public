/**
 * description:数据备份文件
 * author:wangzhia
 * date:2017-07-26
 */
 
#ifndef _CFILE_BACKUP_H_
#define _CFILE_BACKUP_H_

#include "CPublicTool.h"

enum
{
	BACKUP_FILE_SIZE_TYPE			= 1,//文件大小
	BACKUP_FILE_INTERVAL_TIME_TYPE	= 2,//时间间隔
	BACKUP_FILE_END_FILE_TYPE		= 3,//入口文件处理完时备份
	BACKUP_FILE_IMMEDIATE_TYPE		= 4//即时备份
};

///////////////
//服务端处理模式
enum
{
	BACKUP_SYNC_MODE			= 1,//同步
	BACKUP_ASYNC_MODE			= 2//异步
};

typedef struct
{
	char bakDir[1024];			/*备份目录*/
	UnInt32 type;				/*备份模式是文件方式还是表方式*/
	UnInt32 writeMode;			/*写文件采用同步还是异步模式*/
	UnInt32 backupType;			/*移动文件类型*/
	UnInt32 interval;			/*文件间隔*/
	UnInt32 backupSize;
} stBakConfigInfo;

typedef struct
{
	bool fileEnd;
	char fileNameHead[12+1];		/*文件名头部*/
	char fileName[128+1];			/*文件名称*/
	char fileContent[1024+1];		/*文件内容*/
} stFileBakInfo;

using namespace std;

class CFileBackUp : public Thread
{
public:
	CFileBackUp (void);
	
	~CFileBackUp(void);
	
	void Init (stBakConfigInfo *pBakInfo);
	
	void Process (void);
	
	void run (void);
	
	Ret WriteFile (stFileBakInfo *tFileInfo);
	
	//备份文件
	void BackUp ();
	
private:
	//判断文件是否满足备份条件
	bool IsBackup (GDF_FILE *pFile);
	
private:
	map<string, GDF_FILE*> m_FileMgr;
	
	stBakConfigInfo *m_BakConfInfo;

	ilog_CMutexLock *m_MutexLocker;
	
	deque2<stFileBakInfo> m_FileContentQueue;
	
	bool isRunFlag;
	
	bool m_BackupFlag;
	
	char m_LastDay[8+1];
	
	bool m_IsSDFSBak;
};

#endif
