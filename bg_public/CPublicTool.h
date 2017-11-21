/*
 * 功能描述:
 * 创建时间:2016/4/27 9:37:05
 * 编码人员:wangzhia
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
	UnInt32 state;				/*状态*/
	char err_msg[128];			/*错误信息*/
	char err_code[10];			/*状态信息*/
	int deal_num;				/*处理数据量*/
} stMessage;

typedef struct
{
	void *sBuff;			//数据正文
	int length;				//数据真实长度
	int dataNum;			//数据分发标示
	char dataSeq[40+1];		//数据序列(根据该字段操作数据源)
	char dataOnly[40+1];	//根据该字段剔重（根据该值取余分发）
	bool isEnd;				//是否结尾
	int  code;				//成功失败标示(0,成功)
	char type[128+1];		//数据类型
} stDealTask;

class CPublicTool
{
public:
	CPublicTool();
	~CPublicTool();
	virtual int init(ibear *br,stMessage *stMsg,char *serverName)=0;

};

#endif
