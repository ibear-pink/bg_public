/*
 * ��������:
 * ����ʱ��:2016/3/1 14:59:14
 * ������Ա:wangzhia
*/

#ifndef _CPUBLIC_MANAGER_H_
#define _CPUBLIC_MANAGER_H_

#include <algorithm>
#include <signal.h>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <cstring>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdarg.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <errno.h>

#include "CDealData.h"

using namespace std;

class CPublicMgr
{
public:
	CPublicMgr();
	~CPublicMgr();
	
	//���Ի�-��ʼ��
	virtual int init(ibear *br,char *logName,int thread_num,map<string,icfg_Map*> user_defined)=0;
	//���Ի�-�ӿ����ʹ���
	virtual int process()=0;
	//���Ի�-��ȡ����Դ
	virtual int ProInterface(vector<stDealTask*> *values)=0;
	//�����α�ķ�ʽȡ����Դ
	virtual int ProTimeList(char *sql_key)=0;
	//�����α���Ϊ���ݽṹ�帳ֵ
	virtual int setTask(vector<string> v_row,stDealTask *pTask)=0;
	
};

#endif
