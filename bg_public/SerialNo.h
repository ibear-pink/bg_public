/*
*	�ļ����� public.h
*
*	��Ȩ���� (C) 2012 ����˼������Ϣ�����ɷ����޹�˾
*
*	�����������������
*
*	���ߣ�Tom
*
*	�������ڣ�2006-07-10 
*
*	�汾��ver10.0
*
*	�޶���¼��
*	�޶�����    �޶���    							�޶�����
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
		int	i_serial_no	;	/*���к�6λ	��0��ʼ	ѭ����999999 */
		int	i_unikey	;	/*����������3λ	ȡ��g_stAppCfg.stZookeeper.chProcEndFlag */
		int	i_cluster	;	/*��Ⱥ����2λ	ȡ��g_stAppCfg.stZookeeper.chCluster */
};


#endif
