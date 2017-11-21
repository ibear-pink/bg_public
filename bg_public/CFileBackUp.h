/**
 * description:���ݱ����ļ�
 * author:wangzhia
 * date:2017-07-26
 */
 
#ifndef _CFILE_BACKUP_H_
#define _CFILE_BACKUP_H_

#include "CPublicTool.h"

enum
{
	BACKUP_FILE_SIZE_TYPE			= 1,//�ļ���С
	BACKUP_FILE_INTERVAL_TIME_TYPE	= 2,//ʱ����
	BACKUP_FILE_END_FILE_TYPE		= 3,//����ļ�������ʱ����
	BACKUP_FILE_IMMEDIATE_TYPE		= 4//��ʱ����
};

///////////////
//����˴���ģʽ
enum
{
	BACKUP_SYNC_MODE			= 1,//ͬ��
	BACKUP_ASYNC_MODE			= 2//�첽
};

typedef struct
{
	char bakDir[1024];			/*����Ŀ¼*/
	UnInt32 type;				/*����ģʽ���ļ���ʽ���Ǳ�ʽ*/
	UnInt32 writeMode;			/*д�ļ�����ͬ�������첽ģʽ*/
	UnInt32 backupType;			/*�ƶ��ļ�����*/
	UnInt32 interval;			/*�ļ����*/
	UnInt32 backupSize;
} stBakConfigInfo;

typedef struct
{
	bool fileEnd;
	char fileNameHead[12+1];		/*�ļ���ͷ��*/
	char fileName[128+1];			/*�ļ�����*/
	char fileContent[1024+1];		/*�ļ�����*/
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
	
	//�����ļ�
	void BackUp ();
	
private:
	//�ж��ļ��Ƿ����㱸������
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
