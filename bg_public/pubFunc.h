#ifndef _CPUB_FUNC_H_
#define _CPUB_FUNC_H_

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <string>
#include <map>
#include <vector>
#include <sys/types.h>
#include <list>
#include <time.h>
#include <sys/time.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socketvar.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <errno.h>
#include <signal.h>
#include <assert.h>
#include <iconv.h>
#include <stdlib.h>

#include "Define.h"
#include "SerialNo.h"

extern SerialNo g_SerialNo;

using namespace std;

//日期函数
char *GetSysTime();
long getUTime();
char* CurrSysDate(int oper);
void getCurUTime( char *sDateTime );
void getCurDateTime(char *sDateTime);
void GetCurDateTime(string& date_time);
void getCurDateTime(char *sDateTime, const char *pattern);
int AddMonth(int yearmonth,int addmonth);
void addday (int year, int month, int day, int spareDay, int *d_year, int *d_month, int *d_day);
void addDays (char *src, int days, char *des);

//转换string
string toString(char *c);
string toString(const char *c);
string toString(int c);
string toString(long c);

//字符串处理
int replaceTemplate(char *tpl_content,char *out_content,map<string,string> bindParams);
char * strreplace(char *pInput,char *pOutput,char *pSrc,char *pDst);
char* touppers(char *in_src,char *out_src);
int codeswitch(const char *src_code,const char *desc_code,char *buf, size_t len,char *outstr);
int gbk2utf(char *buf, size_t len,char *outstr);

//拆分字符串
int SplitString (const char *buff, char (*out_values)[1024]);
int SplitStringByTipForTwo (const char *buff, char *tStr1, char *tStr2);
void SplitAtPos (const char *buff, char *dest, char split, int pos);
int getBinNameByPath(char *fileName,char *binName);

//其他
int ChkProgNum(char *p_prog);
int NeedDeal(char* phone_no, int proc_no, int proc_num);
unsigned int getlocalip( vector<string> &IPVec );
int GetValidIp(const char *sSegment,const char *sMask,char *szAddress);

//清除数据内存
template<class D> extern void clearDataList (vector<D*> &vList);
template<class K, class V> extern void clearDataList (map<K, V*> &mList);
template<class K, class V> extern void clearDataList (multimap<K, V*> &mList);
template<class K, class V> extern void clearDataList (map<K, vector<V*> > &mList);

template<class D>
void clearDataList (vector<D*> &vList)
{
	if (vList.size() == 0)
	{
		return;
	}
	
	typename vector<D*>::iterator iter;
	for (iter = vList.begin(); iter != vList.end(); iter++)
	{
		D *pData = *iter;
		delete pData;
		pData = NULL;
	}
	
	vList.clear ();
	
	return;
}

template<typename K, typename V>
void clearDataList (map<K, V*> &mList)
{
	if (mList.size() == 0)
	{
		return;
	}
	
	typename map<K, V*>::iterator iter;
	for (iter = mList.begin(); iter != mList.end(); iter++)
	{
		V *pData = iter->second;
		delete pData;
		pData = NULL;
	}
	
	mList.clear ();
}

template<typename K, typename V>
void clearDataList (multimap<K, V*> &mList)
{
	if (mList.size() == 0)
	{
		return;
	}
	
	typename multimap<K, V*>::iterator iter;
	for (iter = mList.begin(); iter != mList.end(); iter++)
	{
		V *pData = iter->second;
		delete pData;
		pData = NULL;
	}
	
	mList.clear ();
}

template<typename K, typename V>
void clearDataList (map<K, vector<V*> > &mList)
{
	if (mList.size() == 0)
	{
		return;
	}
	
	vector<V*> vList;
	typename vector<V*>::iterator vIter;
	typename map<K, vector<V*> >::iterator iter;
	
	//内存释放
	for (iter = mList.begin(); iter != mList.end(); iter++)
	{
		vList = iter->second;
		
		for (vIter = vList.begin(); vIter != vList.end(); vIter++)
		{
			V *pNode = *vIter;
			delete pNode;
			pNode = NULL;
		}
		
		vList.clear();
	}
	
	mList.clear();
}


#endif

