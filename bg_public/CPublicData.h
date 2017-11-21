/*
 * 功能描述:
 * 创建时间:2016/3/1 14:59:14
 * 编码人员:wangzhia
*/

#ifndef _CPUBLIC_DATA_H_
#define _CPUBLIC_DATA_H_

#include <algorithm>
#include <dirent.h>
#include <signal.h>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string.h>
#include <cstring>
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

#include "CPublicTool.h"

using namespace std;

class CPublicData
{
public:
	CPublicData();
	~CPublicData();
	virtual int init(ibear *ib,char *logName,int cur_thread_num,stMessage *stmsg,map<string,icfg_Map*> user_defined)=0;
	virtual int process(stDealTask *pTask)=0;
	virtual int fprocess(stDealTask *pTask)=0;
};

#endif
