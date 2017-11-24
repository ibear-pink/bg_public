#ifndef _DEFINE_H_
#define _DEFINE_H_

#include <pthread.h>
#include <string.h>

/* DATA TYPE DEFINATION */
#ifdef _32BIT_SERVER
typedef unsigned long long	UnInt64;
typedef unsigned int		UnInt32;
typedef int					Int32;
typedef long long			Int64;
#elif _64BIT_SERVER
typedef unsigned long		UnInt64;
typedef unsigned int		UnInt32;
typedef int					Int32;
typedef long				Int64;
#else
typedef unsigned long long	UnInt64;
typedef unsigned int		UnInt32;
typedef int					Int32;
typedef long long			Int64;
#endif

#define MAX_DEQUE_NUM		10000
#define BG_MAX_BUFFERLEN	10240
#define MAX_BUSINESS_THREAD	40
#define MAX_PARSE_FILE_NUM	16
#define MAX_PARSE_FIEL_NUM	32
#define IFCONF_LEN 			1024
#define MASK				"0.0.0.0"
#define BUFSIZE				1024

/*socket 相关信息*/
#define MAXDATASIZE			1000
#define MAX_LOG_LENGTH		204800
#define MAX_BUFLEN 			4096
#define PRO_LENGTH			120 //进程名称最大字符数

typedef enum
{
	RET_FAIL = -1,
	RET_OK = 0,
	RET_QUIT = 1
} Ret;

#define	SUCCESS		"Y"
#define	FAIL		"N"

#define ERROR		-1
#define INFO		2
#define DEBUG		3

#define	OK			 0
#define NOTFOUND	1403

#define FILEPATH_LEN				128

/*断点定义*/
#define BREAK_POINT_0   '0'  				/*正常终止*/
#define BREAK_POINT_1   '1'  				/*处理记录中断*/
#define BREAK_POINT_2   '2'  				/*移错单文件中断*/
#define BREAK_POINT_3   '3'  				/*移入口到备份中断*/
#define	BREAK_POINT_4	'4'					/*移入口到错误目录中断*/
#define	BREAK_POINT_5	'5'					/*将出口文件从临时目录移到正式目录*/

/*协议转换接口类型*/
enum
{
	INTERFACE_TYPE_FILE		= 1,	//文件接口
	INTERFACE_TYPE_TABLE	= 2,	//接口表
	INTERFACE_TYPE_FEDX		= 3,	//fedx接口
	INTERFACE_TYPE_API		= 4		//API
};

/*备份模式（1：文件方式，2：表方式）*/
enum
{
	NO_BACKUP = 0,  //不做备份
	BACKUP_TYPE_FILE	= 1,//文件接口
	BACKUP_TYPE_TABLE	= 2	//接口表
};

enum
{
	MESSAGE_STATE_OVER = 1 //子线程处理结束
};

#define ZK_HOST_ID "{host_id}"


#endif



