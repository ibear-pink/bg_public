/*
*	文件名： public.h
*
*	版权所有 (C) 2012 北京思特奇信息技术股份有限公司
*
*	描述：公共函数相关
*
*	作者：Tom
*
*	创建日期：2006-07-10 
*
*	版本：ver10.0
*
*	修订记录：
*	修订日期    修订人    							修订内容
*
*/

#ifndef _SERIALNO_H_
#define _SERIALNO_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/time.h>
#include <string>

using namespace std;

#define ZK_THREAD_NO_POS 2

class SerialNo
{
	public:
		SerialNo();
		virtual ~SerialNo(){};
	public:
		void Init_SerialNo(char * ,char * );
		string getSerialNo( int );
	private:
		int	i_serial_no	;	/*序列号6位	从0开始	循环到999999 */
		int	i_unikey	;	/*令牌中数字3位	取自g_stAppCfg.stZookeeper.chProcEndFlag */
		int	i_cluster	;	/*集群编码2位	取自g_stAppCfg.stZookeeper.chCluster */
};


#endif
