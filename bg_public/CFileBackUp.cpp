
#include "CFileBackUp.h"

CFileBackUp::CFileBackUp()
{
	m_FileMgr.clear ();
	
	m_MutexLocker = new ilog_CMutexLock();
	
	m_BackupFlag = false;
	
	memset (m_LastDay, 0, sizeof(m_LastDay));
}

CFileBackUp::~CFileBackUp (void)
{
	m_MutexLocker->Lock();
	
	map<string, GDF_FILE *>::iterator iter;
	for (iter = m_FileMgr.begin(); iter != m_FileMgr.end(); iter++)
	{
		GDF_FILE *pFile = iter->second;
		
		delete pFile;
		
		pFile = NULL;
	}
	
	m_FileMgr.clear();
	
	m_MutexLocker->UnLock();
	
	if (NULL != m_MutexLocker)
	{
		delete m_MutexLocker;
		m_MutexLocker = NULL;
	}
	delete m_BakConfInfo;
	m_BakConfInfo = NULL;
#ifdef _SDFS_BAK_
	ifile_sdfs_destory(&sp);
#endif
	printf ("CFileBackUp destructor over.\n");
}

void CFileBackUp::Init (stBakConfigInfo *pBakInfo)
{
	m_BakConfInfo = pBakInfo;
	
	isRunFlag = false;
	
#ifdef _SDFS_BAK_
	m_IsSDFSBak = true;
	if (0 != ifile_sdfs_init(&sp))
	{
		return;
	}
#else
	m_IsSDFSBak = false;
#endif
}

void CFileBackUp::Process(void)
{
	if (!isRunFlag)
	{
		isRunFlag = true;
		if (BACKUP_ASYNC_MODE == m_BakConfInfo->writeMode)
		{
			m_FileContentQueue.setMaxNum(MAX_DEQUE_NUM);
			m_FileContentQueue.setDelayTime(1);
			this->start();
		}
	}
	else
	{
		printf ("文件备份线程已经启动.\n");
	}
}

void CFileBackUp::run(void)
{
	stFileBakInfo *pData = NULL;
	
	while (getThreadState() == THREAD_STAT_RUNNING)
	{
		//备份文件
		BackUp();
		
		//写入临时文件
		pData = m_FileContentQueue.pop();
		if (NULL == pData)
		{
			usleep (50);
			continue;
		}
		
		if (pData->fileEnd)
		{
			delete pData;
			pData = NULL;
			continue;
		}

		GDF_FILE *pFile = NULL;
		
		m_MutexLocker->Lock();
		
		map<string, GDF_FILE *>::iterator iter = m_FileMgr.find (string(pData->fileNameHead));
		if (iter == m_FileMgr.end())
		{
			char tmpFile[128+1] = {0};
			char sysdate[14+1] = {0};
			char curDay[8+1] = {0};
 
			getCurDateTime(sysdate);
			strncpy(curDay,sysdate,8);

			if (strncmp (curDay, m_LastDay,8) != 0)
			{
				strncpy (m_LastDay, curDay,8);
			}
			strcpy (tmpFile, pData->fileName);
			
			if (m_IsSDFSBak)
			{
#ifdef _SDFS_BAK_
				pFile = new SDFS_GDF_FILE (&sp, m_BakConfInfo->bakDir, tmpFile);
#endif
				if (pFile == NULL || 0 != pFile->Open ("a+"))
				{
					m_IsSDFSBak = false;
					if (pFile!=NULL)
					{
						delete pFile;
						pFile = NULL;
					}
					pFile = new SYSTEM_GDF_FILE (m_BakConfInfo->bakDir, tmpFile);
					if (0 != pFile->Open ("a+"))
					{
						printf ("FILE[%s]LINE[%d]ERR_MSG[Open File Is Failed.]\n", __FILE__, __LINE__);
						m_MutexLocker->UnLock();
						continue;
					}
				}
			}
			else
			{
				pFile = new SYSTEM_GDF_FILE (m_BakConfInfo->bakDir, tmpFile);
				if (0 != pFile->Open ("a+"))
				{
					printf ("FILE[%s]LINE[%d]ERR_MSG[Open File Is Failed.]\n", __FILE__, __LINE__);
					m_MutexLocker->UnLock();
					continue;
				}
			}
			m_FileMgr.insert(pair<string, GDF_FILE *>(string(pData->fileNameHead), pFile));
		}
		else
		{
			pFile = iter->second;
		}
		
		pFile->Write (pData->fileContent, strlen(pData->fileContent), 1);
		pFile->Fflush();
		m_MutexLocker->UnLock();
	}
}

Ret CFileBackUp::WriteFile (stFileBakInfo *tFileInfo)
{
	if (m_BakConfInfo->writeMode == BACKUP_SYNC_MODE)
	{
		GDF_FILE *pFile = NULL;
		
		m_MutexLocker->Lock();
		
		map<string, GDF_FILE *>::iterator iter = m_FileMgr.find (string(tFileInfo->fileNameHead));
		if (iter == m_FileMgr.end())
		{
			//替换字符串中@@@@@@@为流水
			char tmpFile[1024] = {0};
			char curDay[8+1] = {0};
			
			getCurDateTime (curDay, "yyyymmdd");
			if (strcmp (curDay, m_LastDay) != 0)
			{
				strcpy (m_LastDay, curDay);
			}
			strcpy (tmpFile, tFileInfo->fileName);
			
#ifdef _DEBUG_INFO_
	printf ("writefile-出口文件名: %s \n", tmpFile);
#endif
			
			if (m_IsSDFSBak)
			{
#ifdef _SDFS_BAK_
				pFile = new SDFS_GDF_FILE (&sp,m_BakConfInfo->bakDir, tmpFile);
#endif
				if (pFile == NULL || 0 != pFile->Open ("a+"))
				{
					m_IsSDFSBak = false;
					if (pFile!=NULL)
					{
						delete pFile;
						pFile = NULL;
					}
					pFile = new SYSTEM_GDF_FILE (m_BakConfInfo->bakDir, tmpFile);
					if (0 != pFile->Open ("a+"))
					{
						printf ("FILE[%s]LINE[%d]ERR_MSG[Open File Is Failed.]\n", __FILE__, __LINE__);
						m_MutexLocker->UnLock();
						return RET_FAIL;
					}
				}
			}
			else
			{
				pFile = new SYSTEM_GDF_FILE (m_BakConfInfo->bakDir, tmpFile);
				if (0 != pFile->Open ("a+"))
				{
					printf ("FILE[%s]LINE[%d]ERR_MSG[Open File Is Failed.]\n", __FILE__, __LINE__);
					m_MutexLocker->UnLock();
					return RET_FAIL;
				}
			}
			m_FileMgr.insert(pair<string, GDF_FILE *>(string(tFileInfo->fileNameHead), pFile));
		}
		else
		{
			pFile = iter->second;
		}
		
		char tmpbuff[4096] = {0};
		sprintf (tmpbuff, "%s", tFileInfo->fileContent);
		
		pFile->Write (tmpbuff, strlen(tmpbuff), 1);
		
		pFile->Fflush();
		
		m_MutexLocker->UnLock();
	}
	else if (m_BakConfInfo->writeMode == BACKUP_ASYNC_MODE)
	{
		stFileBakInfo *pData = new stFileBakInfo();
		if (pData == NULL)
		{
			return RET_FAIL;
		}
		m_FileContentQueue.push_first (tFileInfo);
	}
	else
	{
		printf ("FILE[%s]LINE[%d]ERR_MSG[m_BakConfInfo->writeMode<%d> Is Error.]\n", __FILE__, __LINE__, m_BakConfInfo->writeMode);
		return RET_FAIL;
	}
	
	return RET_OK;
}

void CFileBackUp::BackUp (void)
{
	char backupFile[128] = {0};
	GDF_FILE *pFile = NULL;

	char sysdate[20+1] = {0};
	switch (m_BakConfInfo->backupType)
	{
		case BACKUP_FILE_SIZE_TYPE:
		case BACKUP_FILE_INTERVAL_TIME_TYPE:
		{
			for (map<string, GDF_FILE *>::iterator iter = m_FileMgr.begin(); iter != m_FileMgr.end();)
			{
				pFile = iter->second;
				
				getCurDateTime(sysdate);
				memset (backupFile, 0, sizeof(backupFile));
				sprintf (backupFile, "%s/%s.%s", m_BakConfInfo->bakDir, pFile->GetFileName(),sysdate);
				if (IsBackup(pFile) || m_BackupFlag)
				{
					m_MutexLocker->Lock();
				
					pFile->Close ();
					pFile->Rename (backupFile);
					
					delete pFile;
					
					m_FileMgr.erase(iter++);
					
					m_MutexLocker->UnLock();
				}
				else
					++iter;
			}
			
			break;
		}
		case BACKUP_FILE_END_FILE_TYPE:
		case BACKUP_FILE_IMMEDIATE_TYPE:
		{
			if (m_BackupFlag)
			{
				while (m_FileContentQueue.sum() != 0)
				{
					if (m_FileContentQueue.sum() == 0)
					{
						break;
					}
					
					usleep(50);
				}
		
				for (map<string, GDF_FILE *>::iterator iter = m_FileMgr.begin(); iter != m_FileMgr.end();)
				{
					pFile = iter->second;
					
					getCurDateTime(sysdate,"yyyymmddhhmiss");
					memset (backupFile, 0, sizeof(backupFile));
					sprintf (backupFile, "%s/%s.%s", m_BakConfInfo->bakDir, pFile->GetFileName(),sysdate);
					
					m_MutexLocker->Lock();
					
					pFile->Close ();

					pFile->Rename (backupFile);
					
					delete pFile;
					
					m_FileMgr.erase(iter++);
					
					m_MutexLocker->UnLock();
				}
			}
			
			break;
		}
		default:
			break;
	}
	
	
	m_BackupFlag = false;
	
	return;
}

bool CFileBackUp::IsBackup (GDF_FILE *pFileBackUp)
{
	switch (m_BakConfInfo->backupType)
	{
		case BACKUP_FILE_SIZE_TYPE:
		{
			break;
		}
		case BACKUP_FILE_INTERVAL_TIME_TYPE:
		{
			time_t tmpT = time(NULL);
			if (tmpT/m_BakConfInfo->interval != pFileBackUp->GetFileStartTime()/m_BakConfInfo->interval)
			{
				return true;
			}
			
			break;
		}
		case BACKUP_FILE_END_FILE_TYPE:
		{
			break;
		}
		case BACKUP_FILE_IMMEDIATE_TYPE:
		{
			return true;
		}
		default:
			return false;
	}
	
	return false;
}
