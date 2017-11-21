/**************************
 * 服务名称:JavaPub
 * 功能描述:公共头文件
 * 程序版本:V1.0
 * 编码日期:2014/10/20 11:23:53
 * 编码人员:
 **************************/
#ifndef _JAVA_PUB_FUNC_H_
#define _JAVA_PUB_FUNC_H_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <map>
#include <vector>
#include <iconv.h>
#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/types.h>
#include <strings.h>
#include <dirent.h>

#include "Define.h"
#include "jni.h"

using namespace std;

/*标识*/
#define DIRNAME 128
#define INTER_MAX_REGEX_LEN 1024
#define BEST_BUF			8192		/*文件系统最佳缓存大小*/
#define STATUS 83

//工单，服开，处理函数
extern int createJavaVM();
extern int dealJavaInter(map<int,string> mResult,const char* serv_flag);
extern int BytesToUtf8(char* src, char* dst, int* nout);
#endif
