/*
 * ��������:
 * ����ʱ��:2016/4/27 9:37:05
 * ������Ա:wangzhia
*/

#ifndef _CPUBLIC_TOOL_H_
#define _CPUBLIC_TOOL_H_

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

#include "poor.h"
#include "DynBase.h"
#include "pubFunc.h"
#include "ilog.h"
#include "ibear.h"
#include "icfg_inifile.h"

using namespace std;

typedef struct stMessage
{
	UnInt32 state;				/*״̬*/
	char err_msg[128];			/*������Ϣ*/
	char err_code[10];			/*״̬��Ϣ*/
	int deal_num;				/*����������*/
} stMessage;

typedef struct
{
	void *sBuff;			//��������
	int length;				//������ʵ����
	int dataNum;			//���ݷַ���ʾ
	char dataSeq[40+1];		//��������(���ݸ��ֶβ�������Դ)
	char dataOnly[40+1];	//���ݸ��ֶ����أ����ݸ�ֵȡ��ַ���
	bool isEnd;				//�Ƿ��β
	int  code;				//�ɹ�ʧ�ܱ�ʾ(0,�ɹ�)
	char type[128+1];		//��������
} stDealTask;

class CPublicTool
{
public:
	CPublicTool();
	~CPublicTool();
	virtual int init(ibear *br,stMessage *stMsg,char *serverName)=0;

};

#endif
