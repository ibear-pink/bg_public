#include "pubFunc.h"

SerialNo g_SerialNo;

/* 功能：
	1970年1月1日到现在的微秒数
 * 返回值：
	微秒数
 */
long getUTime()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (((unsigned long)tv.tv_sec) * 1000*1000 + ((unsigned long)tv.tv_usec));
}

/* 功能：
	获得当前时间，精确到微秒
 * 出参：
	时间yyyymmddhh24miss.usec
 */
void getCurUTime( char *sDateTime )
{
	struct tm tDateTime;
	struct timeval tuDateTime;
	time_t lSeconds;
	time( &lSeconds );
	localtime_r( &lSeconds, &tDateTime );
	gettimeofday( &tuDateTime, NULL );
	localtime_r( &tuDateTime.tv_sec, &tDateTime );
	
	sprintf( sDateTime, "%04d%02d%02d%02d%02d%02d.%06d", tDateTime.tm_year + 1900, tDateTime.tm_mon + 1, tDateTime.tm_mday,	tDateTime.tm_hour, tDateTime.tm_min, tDateTime.tm_sec, tuDateTime.tv_usec );
}

char *GetSysTime ()
{
	static char sysDate[20+1];
	time_t sec;
	struct tm t;

	memset(sysDate, 0, sizeof(sysDate));
	sec = time(NULL);
	localtime_r(&sec,&t);
	sprintf (sysDate,"%04d-%02d-%02d %02d:%02d:%02d",\
		t.tm_year+1900,t.tm_mon+1,t.tm_mday,\
		t.tm_hour,t.tm_min,t.tm_sec);
	
	sysDate[20] = '\0';
	return sysDate;
}

void getCurDateTime (char *sDateTime)
{
	struct tm tDateTime;
	time_t lSeconds;
	time( &lSeconds );
	localtime_r( &lSeconds, &tDateTime );
	
	sprintf (sDateTime, "%04d%02d%02d%02d%02d%02d", tDateTime.tm_year + 1900, tDateTime.tm_mon + 1, tDateTime.tm_mday,\
 			tDateTime.tm_hour, tDateTime.tm_min, tDateTime.tm_sec);
 			
 	sDateTime[14] = '\0';
}

void getCurDateTime( char *sDateTime, const char *pattern )
{
	char year[5] = { 0 };
	char month[3] = { 0 };
	char day[3] = { 0 };
	char hour[3] = { 0 };
	char minute[3] = { 0 };
	char second[3] = { 0 };
	char week[1] = { 0 };
	
	struct tm tDateTime;
	time_t lSeconds;
	time( &lSeconds );
	localtime_r( &lSeconds, &tDateTime );
	
	sprintf( year, "%04d", tDateTime.tm_year + 1900);
	sprintf( month, "%02d", tDateTime.tm_mon + 1 );
	sprintf( day, "%02d", tDateTime.tm_mday );
	sprintf( hour, "%02d", tDateTime.tm_hour );
	sprintf( minute, "%02d", tDateTime.tm_min );
	sprintf( second, "%02d", tDateTime.tm_sec );
	sprintf( week, "%d", tDateTime.tm_wday );
	
	char *loc;
	char systime[50] = { 0 };
	strcpy( systime, pattern );
	
	loc = strstr( systime, "yyyy" );
	if( loc )
		memcpy( loc, year, 4 );

	loc = strstr( systime, "YY" );
	if( loc ) 	        
		memcpy( loc, year + 2, 2 );

	loc = strstr( systime, "mm" );
	if( loc )
		memcpy( loc, month, 2 );  	        

	loc = strstr( systime, "dd" );
	if( loc )
		memcpy( loc, day, 2 );	        
	        
	loc = strstr( systime, "hh" );
	if( loc )
		memcpy( loc, hour, 2 );	        

	loc = strstr( systime, "mi" );
	if( loc )
		memcpy( loc, minute, 2 );

	loc = strstr( systime, "ss" );
	if( loc )
		memcpy( loc, second, 2 );

	loc = strstr( systime, "w" );
	if( loc )
		memcpy( loc, week, 1 );

	strcpy( sDateTime, systime );
}

void GetCurDateTime(string& date_time)
{	
	struct tm tDateTime;
	time_t lSeconds;
	time( &lSeconds );
	localtime_r( &lSeconds, &tDateTime );
	
	char sDateTime[14+1];
	memset(sDateTime, 0, sizeof(sDateTime));
	
	sprintf (sDateTime, "%04d%02d%02d%02d%02d%02d", tDateTime.tm_year + 1900, tDateTime.tm_mon + 1, tDateTime.tm_mday,\
 			tDateTime.tm_hour, tDateTime.tm_min, tDateTime.tm_sec);
 			
 	date_time = string(sDateTime);
}

/*
 输入日期偏移addmonth个月
 yearmonth:YYYYMM
 return:YYYYMM
 */
int AddMonth(int yearmonth,int addmonth)
{
	int i = 0;
	int one_ym = 0;
	one_ym = yearmonth;
	if (addmonth >= 0)
	{
		for(i=0;i<addmonth;i++)
		{
			one_ym = one_ym + 1;
			if (one_ym%100==13)
				one_ym = (one_ym/100+1)*100+1;
		}
	}
	else
	{
		for(i=0;i>addmonth;i--)
		{
			one_ym = one_ym - 1;
			if (one_ym%100==0)
				one_ym = (one_ym/100-1)*100+12;
		}
	}
	return one_ym;
	
}

string toString(char *c)
{
	return string(c);
}
string toString(const char *c)
{
	return string(c);
}

string toString(int i)
{
	char c[10+1] = {0};
	sprintf(c,"%d",i);
	return string(c);
}

string toString(long l)
{
	char c[20+1] = {0};
	sprintf(c,"%ld",l);
	return string(c);
}

/**
 *@name ChkProgNum
 *@description 用于获取操作时间,
 *@inparam p_prog 程序名称
 *@return  整型值，0：无进程
 */
int ChkProgNum(char *p_prog)
{
	int num=0;
	char bufStr[PRO_LENGTH]="\0";
	char bufCmd[PRO_LENGTH]="\0";
	FILE *fRead;
	
	sprintf(bufStr,"ps -ef|grep -v grep|grep -v sh|grep -v vi|grep -v tail|grep -v log|grep -v LOG|grep %s |wc -l",p_prog);
	
	if((fRead=popen(bufStr,"r"))!= NULL)
	{
		while(fgets(bufCmd,PRO_LENGTH,fRead)!=NULL)
		{
			if(atoi(bufCmd)>0) break;
		}
		pclose(fRead);
	}
	num=atoi(bufCmd);
	
	return(num);
}

/**
 *@name NeedDeal
 *@description 判断该号码是否该在该进程中处理,
 *@inparam phone_no 电话号码
 *@inparam proc_no  当前进程序号
 *@inparam proc_num 启动进程总数
 *@return  整型值，0：处理
 */
int NeedDeal(char* phone_no, int proc_no, int proc_num)
{
	long phone = atol(phone_no); 

	if( proc_no != ( (phone%10) % proc_num ) )
	{
		return RET_FAIL;
	}

	return RET_OK;
}

/**
 *@name touppers
 *@description 字符串转大写
 *@inparam  in_src      输入字符串
 *@return  转换后字符串
 */
char* touppers(char *in_src,char *out_src)
{
	strcpy(out_src,in_src);
	for(int i = 0; i < sizeof(in_src); i++)
	{
		out_src[i] = toupper(in_src[i]);
	}
	return out_src;
}

/******************* zhangdg add 20130703 ************************\
 *  函 数 名  :         *CurrSysDate
 *  程序功能  :         通过内部函数获取当前时间的不同展现形式
 *  输    入  :         表达式选项option
 *  输    出  :         成功:1 失败:<0
 *  返    回  :         日期字符串
 \*****************************************************************/
char* CurrSysDate(int option)
{
	struct tm timeptr;
	time_t now;
	static char now_date[50];
	static char now_time[50];
	static char now_datetime[100];
	char temptime[15];
	
	memset(now_date,0,sizeof(now_date));
	memset(now_time,0,sizeof(now_time));
	memset(now_datetime,0,sizeof(now_datetime));
	
	time(&now);
	timeptr=*localtime(&now);
	if(option==0) /*YYYY-MM-DD*/
	{
		sprintf(now_date,"%04d-%02d-%02d",1900+timeptr.tm_year,timeptr.tm_mon+1,timeptr.tm_mday);
		return now_date;
	}
	if(option==1) /*YYYY/MM/DD*/
	{
		sprintf(now_date,"%04d/%02d/%02d",1900+timeptr.tm_year,timeptr.tm_mon+1,timeptr.tm_mday);
		return now_date;
	}
	if(option==2) /*YYYY.MM.DD*/
	{
		sprintf(now_date,"%04d.%02d.%02d",1900+timeptr.tm_year,timeptr.tm_mon+1,timeptr.tm_mday);
		return now_date;
	}
	
	if(option==3) /*HH:MM:SS*/
	{
		sprintf(now_time,"%02d:%02d:%02d",timeptr.tm_hour,timeptr.tm_min,timeptr.tm_sec);
		return now_time;
	}
	if(option==4) /*HHMMSS*/
	{
		sprintf(now_time,"%02d%02d%02d",timeptr.tm_hour,timeptr.tm_min,timeptr.tm_sec);
		return now_time;
	}
	if(option==5) /*YYYY-MM-DD HH:MM:SS*/
	{
		sprintf(now_datetime,"%04d-%02d-%02d %02d:%02d:%02d",1900+timeptr.tm_year,timeptr.tm_mon+1,timeptr.tm_mday,timeptr.tm_hour,timeptr.tm_min,timeptr.tm_sec);
		return now_datetime;
	}
	if(option==13) /*YYYY/MM/DD HH:MM:SS*/
	{
		sprintf(now_datetime,"%04d/%02d/%02d %02d:%02d:%02d",1900+timeptr.tm_year,timeptr.tm_mon+1,timeptr.tm_mday,timeptr.tm_hour,timeptr.tm_min,timeptr.tm_sec);
		return now_datetime;
	}
	if(option==14) /*YYYY-MM-DDHHMMSS*/
	{
		memset(temptime,0,15);
		strcpy(temptime,CurrSysDate(0));
		strcat(temptime,CurrSysDate(4));
		strcpy(now_datetime,temptime);
	}
	if(option==15) /*YYYYMMDD*/
	{
		sprintf(now_datetime,"%04d%02d%02d",1900+timeptr.tm_year,timeptr.tm_mon+1,timeptr.tm_mday);
	}
	if(option==16) /*YYYYMMDDHHMMSS*/
	{
		sprintf(now_datetime,"%04d%02d%02d",1900+timeptr.tm_year,timeptr.tm_mon+1,timeptr.tm_mday);
		memset(temptime,0,15);
		strcpy(temptime,CurrSysDate(15));
		strcat(temptime,CurrSysDate(4));
		strcpy(now_datetime,temptime);
	}
	if(option==17)
	{
		sprintf(now_time,"%02d",timeptr.tm_hour);
		return now_time;
	}
	return now_datetime;
}

int SplitStringByTipForTwo (const char *buff, char *tStr1, char *tStr2)
{
	int size = 0;
	
	char tmpBuf[1024];
	
	memset (tmpBuf, 0, sizeof(tmpBuf));
	
	strcpy (tmpBuf, buff);
	
	int len = strlen(tmpBuf);
	
	tmpBuf[len] = '^';
	tmpBuf[len+1] = '\0';
	
	int i = 0, end = 0, begin = 0;
	
	while (tmpBuf[i] != '\0' && i <= len)
	{
		if (tmpBuf[i] == '^')
		{
			end = i;
			i++;
		}
		else
		{
			i++;
			continue;
		}
		
		size++;
		
		if (size == 1)
		{
			memcpy (tStr1, tmpBuf+begin, end - begin);
			tStr1[end-begin] = '\0';
		}
		
		if (size == 2)
		{
			memcpy (tStr2, tmpBuf+begin, end - begin);
			tStr2[end-begin] = '\0';
		}
		
		begin = i;
	}
	
	return 0;
}

void addday (int year, int month, int day, int spareDay, int *d_year, int *d_month, int *d_day)
{
	*d_year = year;
	*d_month = month;
	*d_day = day;
	
	int spare = 0;
	
	int v[13]={0,31,29,31,30,31,30,31,31,30,31,30,31};
	int u[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
	
	int checkYear = 0;
	
	if ((year % 4 == 0 && year % 100 != 0 ) || year % 400 == 0)
	{
		checkYear = 1;
	}
	
	if (spareDay >= 0)
	{
		if (checkYear == 1)
		{
			spare = day + spareDay - v[*d_month];
			if (spare <= 0)
			{
				*d_day = day + spareDay;
				return;
			}
			else
			{
				month++;
				day = 0;
				if (month > 12)
				{
					year++;
					month = 1;
				}
				*d_year = year;
				*d_month = month;
				
				addday (year, month, day, spare, d_year, d_month, d_day);
			}
		}
		else
		{
			spare = day + spareDay - u[*d_month];
			if (spare <= 0)
			{
				*d_day = day + spareDay;
				return;
			}
			else
			{
				month++;
				day = 0;
				if (month > 12)
				{
					year++;
					month = 1;
				}
				*d_year = year;
				*d_month = month;
				
				addday (year, month, day, spare, d_year, d_month, d_day);
			}
		}
	}
	else
	{
		if (checkYear == 1)
		{
			spare = day + spareDay;
			if (spare > 0)
			{
				*d_day = spare;
				return;
			}
			else if (spare == 0)
			{
				month--;
				if (month <= 0)
				{
					year--;
					month = 12;
				}
				
				*d_year = year;
				*d_month = month;
				*d_day = v[month];
				return;
			}
			else
			{
				month--;
				day = v[month];
				if (month <= 0)
				{
					year--;
					month = 12;
				}
				
				addday (year, month, day, spare, d_year, d_month, d_day);
			}
		}
		else
		{
			spare = day + spareDay;
			if (spare > 0)
			{
				*d_day = spare;
				return;
			}
			else if (spare == 0)
			{
				month--;
				day = u[month];
				if (month <= 0)
				{
					year--;
					month = 12;
				}
				
				*d_year = year;
				*d_month = month;
				*d_day = u[month];
				return;
			}
			else
			{
				month--;
				day = u[month];
				if (month <= 0)
				{
					year--;
					month = 12;
				}
				
				addday (year, month, day, spare, d_year, d_month, d_day);
			}
		}
	}
}

void addDays (char *src, int days, char *des)
{
	int year = 0, month = 0, day = 0;
	int d_year = 0, d_month = 0, d_day = 0;
	char s_year[4+1] = {0};
	char s_month[2+1] = {0};
	char s_day[2+1] = {0};
	
	strncpy (s_year, src, 4);
	s_year[4] = '\0';
	strncpy (s_month, src+4, 2);
	s_month[2] = '\0';
	strncpy (s_day, src+6, 2);
	s_day[2] = '\0';
	
	year = atoi (s_year);
	month = atoi (s_month);
	day = atoi (s_day);
	
	addday (year, month, day, days, &d_year, &d_month, &d_day);
	
	long tmpdate = d_year*10000 + d_month*100 + d_day;
	sprintf (des, "%ld", tmpdate);
	
	return;
}

int SplitString (const char *buff, char (*out_values)[1024])
{
	int iRet = 0;
	int size = 0;
	char str[1024] = {0};
	char content[1024];
	
	memset (content, 0, sizeof(content));
	strcpy (content, buff);
	
	long len = strlen(content);
	
	content[len] = '\0';
	
	int i = 0, end = 0, begin = 0;
	while (i < len)
	{
		if (content[i] == '|')
		{
			end = i;
		}
		else
		{
			i++;
			continue;
		}
		
		memset(str,0,sizeof(str));
		strncpy (str, content+begin, end - begin);
		strcpy(out_values[size] ,str);
		i++;
		begin = i;
		size++;
	}
	
	return iRet;
}


int replaceTemplate(char *tpl_content,char *out_content,map<string,string> bindParams)
{
	int iRet = 0;
	char *p=NULL;
	char out_content_tmp[1024] = {0};
	
	strcpy(out_content_tmp,tpl_content);
	
	for (map<string,string>::iterator iter = bindParams.begin();iter != bindParams.end();iter++)
	{
		string key = iter->first;
		string value = iter->second;
		char repedStr[64] = {0};
		sprintf(repedStr,"${%s}",key.c_str());
		p = strstr(out_content_tmp, repedStr);
		if (p == NULL)
		{
			continue;
		}
		
		if( NULL==strreplace((char *)out_content_tmp,(char *)out_content_tmp,repedStr,(char *)value.c_str()))
		{
			return -1;
		}
	}
	strcpy(out_content,out_content_tmp);
	
	return 0;
}

// 替换字符串中特征字符串为指定字符串
char * strreplace(char *pInput,char *pOutput,char *pSrc,char *pDst)
{
	char *p;//*p1;
	char p1[1024] = {0};
	int len;
	
	if (pInput == NULL || pSrc == NULL || pDst == NULL)
		return NULL;
	
	p = strstr(pInput, pSrc);       //返回字符串第一次出现的地址,否则返回NULL
	if (p == NULL)  /*not found*/
		return NULL;
	
	len = strlen(pInput) + strlen(pDst) - strlen(pSrc);
	
	strncpy(p1, pInput, p-pInput);
	strcat(p1, pDst);
	
	p += strlen(pSrc);
	strcat(p1, p);
	strcpy(pOutput,p1);
	
	return pOutput;
}

void SplitAtPos (const char *buff, char *dest, char split, int pos)
{
	int tmp_pos = pos;
	char tmpBuf[1024];
	
	memset (tmpBuf, 0, sizeof(tmpBuf));
	strcpy (tmpBuf, buff);
	int len = strlen(tmpBuf);
	
	tmpBuf[len] = split;
	tmpBuf[len+1] = '\0';
	
	int i = 0, end = 0, begin = 0;
	while (tmpBuf[i] != '\0' && i <= len)
	{
		if (tmpBuf[i] == split)
		{
			tmp_pos --;
			if (1 == tmp_pos && tmpBuf[i + 1] != '\0')
			{
				begin = i + 1;
			} else if (0 == tmp_pos) {
				end = i;
				break;
			}
		}
		i++;
		continue;
	}
	
	if (end != 0)
	{
		memcpy (dest, tmpBuf + begin, end - begin);
		dest[end-begin] = '\0';
	}
	
	return ;
}

int getBinNameByPath(char *fileName,char *binName)
{
	char fileName_temp[1024] = {0};
	char *p;
	strcpy(fileName_temp,fileName);
	while (true)
	{
		p = strstr(fileName_temp,"/");
		if (p == NULL)
		{
			strcpy(binName,fileName_temp);
			return 0;
		}
		memcpy(fileName_temp,p+1,strlen(fileName_temp));
	}
}

//获取地址
//返回IP地址字符串
//返回：0=成功，1=失败
unsigned int getlocalip( vector<string> &IPVec )
{
	int i=0;
	int sockfd;
	struct ifconf ifconf;
	char buf[IFCONF_LEN*4+1];
	struct ifreq *ifreq;
	char* ip;
	
	//初始化ifconf
	ifconf.ifc_len = IFCONF_LEN*4;
	ifconf.ifc_buf = buf;
	
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0))<0)
	{
		return 1;
	}
	ioctl(sockfd, SIOCGIFCONF, &ifconf);    //获取所有接口信息
	close(sockfd);
	
	//接下来一个一个的获取IP地址
	ifreq = (struct ifreq*)buf;
	for( i = (ifconf.ifc_len/sizeof(struct ifreq)); i > 0; i--)
	{
		ip = inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_addr))->sin_addr);
		
		if(strcmp(ip,"127.0.0.1")==0)  //排除127.0.0.1，继续下一个
		{
			ifreq++;
			continue;
		}
		ifreq++;
		IPVec.push_back(ip);
	}
	return 0;
}

int GetValidIp(const char *sSegment,const char *sMask,char *szAddress)
{
	unsigned int c_ip;
	unsigned int mask;
	unsigned int l_ip;
	vector<string>  IPVec;
	vector<string>::iterator iter;
	bool bFind = false;
	
	c_ip = inet_addr(sSegment);
	mask = inet_addr(sMask);
	
	getlocalip(IPVec);
	for( iter = IPVec.begin(); iter != IPVec.end(); iter++ )
	{
		
		l_ip = inet_addr((char*)iter->c_str());
		
		if( (l_ip & mask) == (c_ip & mask) )
		{
			bFind = true;
			break;
		}
	}
	
	if( true == bFind )
	{
		strcpy(szAddress, iter->c_str());
		IPVec.clear();
		return 0;
	}
	else
	{
		IPVec.clear();
		return -1;
	}
}

int gbk2utf(char *buf, size_t len,char *outstr)
{
	return codeswitch("GBK","UTF-8",buf,len,outstr);
}

int codeswitch(const char *src_code,const char *desc_code,char *buf, size_t len,char *outstr)
{
	iconv_t cd = iconv_open(src_code,desc_code);
	if (cd == (iconv_t)-1)
	{
		printf("获取字符转换描述符失败！\n");
		return -1;
	}
	size_t sz = BUFSIZE * BUFSIZE;
	char *tmp_str = (char *)malloc(sz);
	// 不要将原始的指针传进去，那样会改变原始指针的
	size_t inlen = len;
	size_t outlen = sz;
	char *in = buf;
	char *out = tmp_str;
	if (tmp_str == NULL)
	{
		iconv_close(cd);
		printf("分配内存失败！\n");
		return -1;
	}
	if (iconv(cd, &in, &inlen, &out, &outlen) == (size_t)-1)
	{
		printf("iconv 失败！\n");
		iconv_close(cd);
		return -1;
	}
	iconv_close(cd);
	strcpy(outstr,tmp_str);
	return 0;
}
