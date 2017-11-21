/**************************
* ��������:JavaPub
* ��������:�����ļ�
* ����汾:V1.0
* ��������:2011/11/09
* ������Ա:
**************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "JavaPub.h"

#ifndef _DEBUG_
#define _DEBUG_
#endif 
char curSysDatetime[18];


static JavaVM *jvm;
static jobject obj_order;

int createJavaVM()
{
	JNIEnv *env;
	JavaVMOption options[6];
	JavaVMInitArgs vm_args;
	long status;
	jclass cls_order;
	
	char *p_env_abm = NULL;
	DIR *dir = NULL;
	dirent *ptr = NULL;
	char java_path[1024] = {0};
	char javalibs[10240] = {0};
	
	p_env_abm = getenv ("JAVA_LIB");
	if (NULL == p_env_abm)
	{
		printf ("FILE[%s]LINE[%d]ERR_MSG[ȡ��JAVA_LIB��������ʧ��\n", __FILE__, __LINE__);
		return -10;
	}
	sprintf (java_path, "%s", p_env_abm);
	//������ļ�Ŀ¼
	dir = opendir(java_path);
	sprintf(javalibs ,"-Djava.class.path=%s:%s/XmlSchema-1.4.7.jar:%s/acctMgrWeb.jar:%s/acctmgr_net.jar:%s/activation-1.1.jar",java_path,java_path,java_path,java_path,java_path);
	while ((ptr = readdir(dir)) != NULL)
	{
		sprintf(javalibs,"%s%s/%s:",javalibs,java_path, ptr->d_name);
	}
	closedir(dir);
	printf("%s\n",javalibs);
	options[0].optionString = "-Djava.compiler=NONE";
	/*�������*/
	//options[1].optionString = "-Djava.class.path=/acct1pkg/lib/jar/a.jar";
	/*��ʽ���*/
	//options[1].optionString = "-Djava.class.path=/acct1pkg/lib/jar/XmlSchema-1.4.7.jar:/acct1pkg/lib/jar/acctMgrWeb.jar:/acct1pkg/lib/jar/acctmgr_net.jar:/acct1pkg/lib/jar/activation-1.1.jar:/acct1pkg/lib/jar/aopalliance-1.0.jar:/acct1pkg/lib/jar/asm-3.3.1.jar:/acct1pkg/lib/jar/cglib-nodep-2.1_3.jar:/acct1pkg/lib/jar/brmAgent.jar:/acct1pkg/lib/jar/brmAgentWithOutExternalJar.jar:/acct1pkg/lib/jar/common.jar:/acct1pkg/lib/jar/commons-beanutils-1.8.3.jar:/acct1pkg/lib/jar/commons-codec-1.6.jar:/acct1pkg/lib/jar/commons-collections-3.2.1.jar:/acct1pkg/lib/jar/commons-configuration-1.10.jar:/acct1pkg/lib/jar/commons-dbcp-1.4.jar:/acct1pkg/lib/jar/commons-httpclient-3.1.jar:/acct1pkg/lib/jar/commons-dbutils-1.6.jar:/acct1pkg/lib/jar/commons-dbutils-1.5.jar:/acct1pkg/lib/jar/commons-lang-2.6.jar:/acct1pkg/lib/jar/commons-net-3.3.jar:/acct1pkg/lib/jar/commons-net-ftp-2.0.jar:/acct1pkg/lib/jar/commons-logging-1.1.3.jar:/acct1pkg/lib/jar/commons-pool-1.6.jar:/acct1pkg/lib/jar/commons-pool2-2.3.jar:/acct1pkg/lib/jar/curator-client-2.7.1.jar:/acct1pkg/lib/jar/cxf-api-2.7.0.jar:/acct1pkg/lib/jar/cxf-rt-bindings-soap-2.7.4.jar:/acct1pkg/lib/jar/curator-framework-2.7.1.jar:/acct1pkg/lib/jar/cxf-rt-databinding-jaxb-2.7.4.jar:/acct1pkg/lib/jar/cxf-rt-bindings-xml-2.7.4.jar:/acct1pkg/lib/jar/cxf-rt-core-2.3.3.jar:/acct1pkg/lib/jar/cxf-rt-frontend-jaxws-2.7.4.jar:/acct1pkg/lib/jar/cxf-rt-frontend-simple-2.7.4.jar:/acct1pkg/lib/jar/cxf-rt-transports-http-2.3.3.jar:/acct1pkg/lib/jar/cxf-rt-ws-policy-2.7.4.jar:/acct1pkg/lib/jar/cxf-rt-transports-common-2.3.3.jar:/acct1pkg/lib/jar/cxf-rt-ws-addr-2.7.4.jar:/acct1pkg/lib/jar/druid-0.2.26.jar:/acct1pkg/lib/jar/dubbo-2.5.3.jar:/acct1pkg/lib/jar/detailcfg.jar:/acct1pkg/lib/jar/dom4j-1.6.1.jar:/acct1pkg/lib/jar/eframework-2.1.1.jar:/acct1pkg/lib/jar/ezmorph-1.0.6.jar:/acct1pkg/lib/jar/fluent-hc-4.3.3.jar:/acct1pkg/lib/jar/geronimo-javamail_1.4_spec-1.7.1.jar:/acct1pkg/lib/jar/fastjson-1.2.5.jar:/acct1pkg/lib/jar/gson-2.3.1.jar:/acct1pkg/lib/jar/geronimo-jms_1.1_spec-1.1.1.jar:/acct1pkg/lib/jar/groovy-all-2.1.3.jar:/acct1pkg/lib/jar/guava-18.0.jar:/acct1pkg/lib/jar/hsf-2.0.2.jar:/acct1pkg/lib/jar/httpasyncclient-4.0.2.jar:/acct1pkg/lib/jar/httpcore-nio-4.3.2.jar:/acct1pkg/lib/jar/httpcore-4.3.2.jar:/acct1pkg/lib/jar/httpclient-4.3.5.jar:/acct1pkg/lib/jar/httpclient-osgi-4.3.3.jar:/acct1pkg/lib/jar/httpclient-cache-4.3.3.jar:/acct1pkg/lib/jar/iJCF-2.5.0.jar:/acct1pkg/lib/jar/idmm2-client-api-0.0.1-SNAPSHOT.jar:/acct1pkg/lib/jar/httpmime-4.3.5.jar:/acct1pkg/lib/jar/jackson-core-2.2.0.jar:/acct1pkg/lib/jar/iJCF-2.5.0-javadoc.jar:/acct1pkg/lib/jar/jackson-annotations-2.2.0.jar:/acct1pkg/lib/jar/ini4j-0.5.2.jar:/acct1pkg/lib/jar/javassist-3.12.1.GA.jar:/acct1pkg/lib/jar/javax.annotation-api-1.2-b02.jar:/acct1pkg/lib/jar/javax.ws.rs-api-2.0-rc3.jar:/acct1pkg/lib/jar/javax.servlet-api-3.0.1.jar:/acct1pkg/lib/jar/jackson-databind-2.2.0.jar:/acct1pkg/lib/jar/jedis-2.7.2.jar:/acct1pkg/lib/jar/jcip-annotations-1.0.jar:/acct1pkg/lib/jar/jaxrs-api-2.3.4.Final.jar:/acct1pkg/lib/jar/jcl-over-slf4j-1.7.7.jar:/acct1pkg/lib/jar/jaxb-impl-2.1.13.jar:/acct1pkg/lib/jar/jms-api-1.1-rev-1.jar:/acct1pkg/lib/jar/jline-0.9.94.jar:/acct1pkg/lib/jar/jetty-util-7.2.0.v20101020.jar:/acct1pkg/lib/jar/jettison-1.3.1.jar:/acct1pkg/lib/jar/json-path-0.8.0.jar:/acct1pkg/lib/jar/json-simple-1.1.1.jar:/acct1pkg/lib/jar/jolokia-client-java-1.2.3.jar:/acct1pkg/lib/jar/joda-time-2.3.jar:/acct1pkg/lib/jar/json-20140107.jar:/acct1pkg/lib/jar/json-smart-1.1.jar:/acct1pkg/lib/jar/jsr250-api-1.0.jar:/acct1pkg/lib/jar/jta-1.1.jar:/acct1pkg/lib/jar/junit-4.10.jar:/acct1pkg/lib/jar/netty-buffer-4.0.24.Final.jar:/acct1pkg/lib/jar/netty-3.2.5.Final.jar:/acct1pkg/lib/jar/log4j-1.2.17.jar:/acct1pkg/lib/jar/neethi-3.0.2.jar:/acct1pkg/lib/jar/mysql-connector-java-5.1.22.jar:/acct1pkg/lib/jar/netty-transport-4.0.24.Final.jar:/acct1pkg/lib/jar/netty-handler-4.0.24.Final.jar:/acct1pkg/lib/jar/netty-codec-4.0.24.Final.jar:/acct1pkg/lib/jar/netty-common-4.0.24.Final.jar:/acct1pkg/lib/jar/opensaml-2.5.1-1.jar:/acct1pkg/lib/jar/openws-1.4.2-1.jar:/acct1pkg/lib/jar/ojdbc6.jar:/acct1pkg/lib/jar/oro-2.0.8.jar:/acct1pkg/lib/jar/oscache-2.4.1.jar:/acct1pkg/lib/jar/protobuf-java-2.5.0.jar:/acct1pkg/lib/jar/resteasy-jaxrs-2.3.4.Final.jar:/acct1pkg/lib/jar/resteasy-jettison-provider-2.3.4.Final.jar:/acct1pkg/lib/jar/resteasy-jaxb-provider-2.3.4.Final.jar:/acct1pkg/lib/jar/slf4j-log4j12-1.7.7.jar:/acct1pkg/lib/jar/servlet-api-2.3.jar:/acct1pkg/lib/jar/scannotation-1.0.3.jar:/acct1pkg/lib/jar/resteasy-spring-2.3.4.Final.jar:/acct1pkg/lib/jar/slf4j-api-1.7.12.jar:/acct1pkg/lib/jar/spring-beans-3.2.0.RELEASE.jar:/acct1pkg/lib/jar/spring-aop-3.2.0.RELEASE.jar:/acct1pkg/lib/jar/spring-context-3.2.0.RELEASE.jar:/acct1pkg/lib/jar/slf4j-simple-1.7.12.jar:/acct1pkg/lib/jar/spring-context-support-3.2.0.RELEASE.jar:/acct1pkg/lib/jar/spring-core-3.2.0.RELEASE.jar:/acct1pkg/lib/jar/spring-expression-3.2.0.RELEASE.jar:/acct1pkg/lib/jar/spring-jdbc-3.2.0.RELEASE.jar:/acct1pkg/lib/jar/spring-orm-3.2.0.RELEASE.jar:/acct1pkg/lib/jar/spring-test-3.2.0.RELEASE.jar:/acct1pkg/lib/jar/spring-tx-3.2.0.RELEASE.jar:/acct1pkg/lib/jar/spring-web-3.0.5.RELEASE.jar:/acct1pkg/lib/jar/stax2-api-3.1.1.jar:/acct1pkg/lib/jar/wss4j-1.6.10.jar:/acct1pkg/lib/jar/unirest-java-1.3.26.jar:/acct1pkg/lib/jar/wsdl4j-1.6.3.jar:/acct1pkg/lib/jar/woodstox-core-asl-4.1.4.jar:/acct1pkg/lib/jar/xmemcached-2.0.0.jar:/acct1pkg/lib/jar/xml-apis-1.3.03.jar:/acct1pkg/lib/jar/xmlpull-1.1.3.1.jar:/acct1pkg/lib/jar/xercesImpl-2.8.0.jar:/acct1pkg/lib/jar/xalan-2.7.0.jar:/acct1pkg/lib/jar/xml-resolver-1.2.jar:/acct1pkg/lib/jar/xmlschema-core-2.0.3.jar:/acct1pkg/lib/jar/xmlsec-1.5.4.jar:/acct1pkg/lib/jar/xmltooling-1.3.2-1.jar:/acct1pkg/lib/jar/xroad-client-api-2.0.0.jar:/acct1pkg/lib/jar/zkclient-2.1.jar:/acct1pkg/lib/jar/xom-1.2.5.jar:/acct1pkg/lib/jar/xpp3_min-1.1.4c.jar:/acct1pkg/lib/jar/xstream-1.4.4.jar:/acct1pkg/lib/jar/zookeeper-3.4.6.jar:/acct1pkg/lib/jar/idmm-simple-api-0.0.11.jar:/acct1pkg/lib/jar/idmm-impl-0.0.11.jar:/acct1pkg/lib/jar/logback-core-1.1.1.jar:/acct1pkg/lib/jar/logback-classic-1.1.1.jar:/acct1pkg/lib/jar/curator-recipes-2.7.1.jar";
		//options[3].optionString = "-Xms256M";
	//options[4].optionString = "-Xmx1024M";
	//options[5].optionString = "-Xss256M";
	options[1].optionString = javalibs;
	options[2].optionString = "-verbose:jni";
	options[3].optionString = "-Xms512M";
	options[4].optionString = "-Xmx512M";
	options[5].optionString = "-Xss512M";
	
	memset(&vm_args, 0, sizeof(vm_args));
	vm_args.version = JNI_VERSION_1_6;
	vm_args.nOptions = 6;
	vm_args.options = options;
	vm_args.ignoreUnrecognized = JNI_TRUE;
	
	/*���� JVM ��������������֮�����ھ�׼������ JVM �ˡ�
	�ȴӵ��÷�����ʼ ����ɹ�����������������㣬
	��������޷����� JVM���򷵻�JNI_ERR��*/
	status = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
	if (status != JNI_ERR)
	{
		jobject obj;
		/*������*/
		//cls_order = env->FindClass("java/lang/String");
		/*��ʽ��*/
		cls_order = env->FindClass("com/sitech/acctmgrint/atom/busi/intface/PubInterCpp");
		if(cls_order != NULL)
		{
			jobject obj = env->AllocObject(cls_order);
			env->GetJavaVM(&jvm); //���浽ȫ�ֱ�����JVM
			obj_order = env->NewGlobalRef(obj);
			//printf("test1,cls_order=%ld,env=%ld...\n",cls_order,env);
			//printf("test1,jvm=%ld,obj_order=%ld...\n",jvm,obj_order);
			printf ("FILE[%s]LINE[%d]��ȡ�ɹ�.\n", __FILE__, __LINE__);
			return RET_OK;
		}
		else
		{
			printf ("FILE[%s]LINE[%d]��ȡjava��PubInterCppʧ��.\n", __FILE__, __LINE__);
			return RET_FAIL;
		}
	}
	else
	{
		printf ("FILE[%s]LINE[%d]JNI_CreateJavaVM ERR.", __FILE__, __LINE__);
		return RET_FAIL;
	}
}

/*
���÷���ͨ��
��Σ�
id_no 				:�û�
phone_no 			:�ֻ�����
LOGIN_NO			:��������
OP_CODE				:������ʶ
ORDER_ID			:ģ��
owner_flag 		:����������ʾ���ش�?'1'Ϊ������������'2'Ϊ����������,'3'Ϊ��������,'4'Ϊ��������
run_code 			:�µ�����״̬
open_flag 		:����ͨ��ʶ���ش�?0--ֻ����ͨ 1--ֻ�����û�״̬ 2--����ͨ�ָ����û�״̬
fyw_flag 			:��ҵ��ͣ������ʶ 0:��ͣSVC 1:�ָ�SVC 2:��ͣGPRS 3:��ͣWLAN 4:��ͣWIFI 5:4G
string_date 	:������: LOGIN_NO=xxxxx,CONTACT_ID=10001234,OP_TIME=20140101125959,OP_CODE=0000
db_label��		:���ݿ��ǩ����ʱ���ô����ַ�"DB_A1"
flag					:������� 1����  2�Ƿ���ͨ

������BusiMsgCPP  CPP_opPubOdrSndInter
CONTACT_ID		:ͳһ��ˮ
LOGIN_NO			:��������
OP_CODE				:������ʶ
ORDER_ID			:����ģ��
OWNER_FLAG		:1���������� 2���������� 3�������� 4��������
STRING_DATA		:�������� PHONE_NO=111,NEW_RUN=...
inDBLabel			:���ݿ��ǩ
�ֲ𹤵���SvcOrderCpp CPP_opArearStatuInter
lIdNo					:�û�
sPhoneNo			:�ֻ�����
sRunCode			:�µ�����״̬
sBlackFlag		:�Ƿ������
sOtherData 		:������ LOGIN_NO=xxxxx,CONTACT_ID=10001234,OP_TIME=20140101125959,OP_CODE=0000
inDBLabel			:���ݿ��ǩ
����ͨ��SvcOrderCpp CPP_opUserStatuInter
inIdNo 				:�û�
inPhoneNo 		:�ֻ�����
inOwnerFlag 	:����������ʾ���ش�?'1'Ϊ������������'2'Ϊ����������,'3'Ϊ��������,'4'Ϊ��������
inNewRun 			:�µ�����״̬
inOpenFlag 		:����ͨ��ʶ���ش�?0--ֻ����ͨ 1--ֻ�����û�״̬ 2--����ͨ�ָ����û�״̬
inFywFlag 		:��ҵ��ͣ������ʶ 0:��ͣSVC 1:�ָ�SVC 2:��ͣGPRS 3:��ͣWLAN 4:��ͣWIFI 5:4G
inSvcStr			:0����ͣSVC 1�ָ�SVC
sOtherData 		:������: LOGIN_NO=xxxxx,CONTACT_ID=10001234,OP_TIME=20140101125959,OP_CODE=0000
inDBLabel��		:���ݿ��ǩ����ʱ���ô����ַ�"DB_A1"

*/
int dealJavaInter(map<int,string> mResult,const char* serv_flag)
{
	char servName[100] = {0};
	char servIDs[1024] = {0};
	jboolean suc;
	long inIdNo = 0;
	jstring str[10];

	JNIEnv *env;
	jvm->AttachCurrentThread((void **)&env, NULL);
	jclass cls_order = env->GetObjectClass(obj_order);
	
#ifdef _DEBUG_INFO_
	//printf("test2,cls_order=%d,env=%d...\n",cls_order,env);
#endif
	/*1.����ͨ 3.���� 4.�ֲ𹤵�*/
	if (strcmp(serv_flag ,JAVA_INTERFACE_SERV_ORD) == 0)
	{
		strcpy(servName,"CPP_opUserStatuInter");
		/*��ʽǩ��*/
		strcpy(servIDs,"(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z");
		
		/*����*/
		//����ǩ��
		//strcpy(servIDs,"(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z");
		//220220200051455214,15043271018,1,C,0,,,LOGIN_NO=bossA,CONTACT_ID=100005237590,OP_TIME=20150205164513,OP_CODE=fwkt,DB_A1
		for(map<int ,string>::iterator iter = mResult.begin();iter != mResult.end();iter++)
		{
			string s_value = iter->second;
			char result_value[1024] = {0};
			char result_des[1024] = {0};
			int len = 1024;
			strcpy(result_value , s_value.c_str());
			BytesToUtf8(result_value,result_des,&len);
			
			int num = iter->first;
			str[iter->first] = env->NewStringUTF(result_des);
			if (iter->first == 0)
			{
				inIdNo = atol(result_value);
			}
		}
	}
	else if (strcmp(serv_flag ,JAVA_INTERFACE_ORDER) == 0)
	{
		strcpy(servName,"CPP_opPubOdrSndInter");
		/*��ʽǩ��*/
		strcpy(servIDs,"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z");
		/*����
		mResult.clear();
		mResult.insert(pair<int,string>(0,string("111111111111111111")));
		mResult.insert(pair<int,string>(1,string("system")));
		mResult.insert(pair<int,string>(2,string("C205")));
		mResult.insert(pair<int,string>(3,string("10004")));
		mResult.insert(pair<int,string>(4,string("1")));
		mResult.insert(pair<int,string>(5,string("LOGIN_NO=bossA,CONTACT_ID=100005237590,OP_TIME=20150205164513,OP_CODE=fwkt")));
		mResult.insert(pair<int,string>(6,string("DB_A1")));		*/
		for(map<int ,string>::iterator iter = mResult.begin();iter != mResult.end();iter++)
		{
			string s_value = iter->second;
			char result_value[1024] = {0};
			strcpy(result_value , s_value.c_str());
			int num = iter->first;
			//str[iter->first] = env->NewStringUTF(result_value);
			str[iter->first] = env->NewStringUTF(result_value);			
			#ifdef _DEBUG_INFO_
			printf("num=%d,result_value=%s\n",num,result_value);
			#endif
		}
	}	
	else if (strcmp(serv_flag ,JAVA_INTERFACE_B_ORDER) == 0)
	{
		memset(servName,0,sizeof(servName));
		memset(servIDs,0,sizeof(servIDs));
		strcpy(servName,"CPP_opArearStatuInter");		
		/*��ʽǩ��*/
		strcpy(servIDs,"(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z");
		/*����
		printf("string.max_size()=%ld\n",sizeof(string));
		mResult.clear();
		mResult.insert(pair<int,string>(0,string("220580200024951803")));
		mResult.insert(pair<int,string>(1,string("15844576780")));
		mResult.insert(pair<int,string>(2,string("b")));
		mResult.insert(pair<int,string>(3,string("1")));
		string a = "LOGIN_NO=system,OP_CODE=C205,LOGIN_ACCEPT=111111111111,CREATE_TIME=20150713161900,ID_NO=220580200024951803,REMARK=�ֲ�,CONTRACT_NO=220580200024951804,CHG_TIME=20150713161900,CHG_REASON=�ֲ�,BLACK_FLAG=1";
		//mResult.insert(pair<int,string>(4,string("LOGIN_NO=system,OP_CODE=C205,LOGIN_ACCEPT=111111111111,CREATE_TIME=20150713161900,ID_NO=220580200024951803,REMARK=�ֲ�,CONTRACT_NO=220580200024951804,CHG_TIME=20150713161900,CHG_REASON=�ֲ�,BLACK_FLAG=1")));
		mResult.insert(pair<int,string>(4,a));
		mResult.insert(pair<int,string>(5,string("DB_A1")));	*/	
		for(map<int ,string>::iterator iter = mResult.begin();iter != mResult.end();iter++)
		{
			string s_value = iter->second;
			char result_value[1024] = {0};
			strcpy(result_value , s_value.c_str());
			int num = iter->first;
			str[iter->first] = env->NewStringUTF(result_value);
			if (iter->first == 0)
			{
				inIdNo = atol(result_value);
			}		
			#ifdef _DEBUG_INFO_
			printf("num=%d,result_value=%s\n",num,result_value);
			#endif
		}
#ifdef _DEBUG_INFO_
		printf("servName=%s\n",servName);
		printf("servIDs=%s\n",servIDs);
#endif
	}	
	else
	{
		printf("�ӿ�δ���ã�����\n");
		return STOP_ERROR_NOR_STOP_JAVA_INTERFACE_FLAG_NOT_CONFIG;
	}
	jmethodID mid_order = env->GetMethodID(cls_order, servName, servIDs);
#ifdef _DEBUG_INFO_
	//printf("test1,mid_order=%d...\n",mid_order);
#endif
	if (mid_order == 0)
	{
		printf ("FILE[%s]LINE[%d]��ȡ����[%s]ʧ��.", __FILE__, __LINE__,servName);
		return RET_FAIL;
	}
	else
	{
		if (strcmp(serv_flag ,JAVA_INTERFACE_SERV_ORD) == 0)
		{
			suc = env->CallBooleanMethod(obj_order, mid_order, inIdNo,str[1],str[2],str[3],str[4],str[5],str[6],str[7],str[8]);
		}
		else if (strcmp(serv_flag ,JAVA_INTERFACE_ORDER) == 0)
		{
			suc = env->CallBooleanMethod(obj_order, mid_order,str[0],str[1],str[2],str[3],str[4],str[5],str[6]);
		}
		else if (strcmp(serv_flag ,JAVA_INTERFACE_B_ORDER) == 0)
		{
			suc = env->CallBooleanMethod(obj_order, mid_order, inIdNo,str[1],str[2],str[3],str[4],str[5]);
		}
	}
	
	if (suc == false)
	{
#ifdef _DEBUG_INFO_
	printf("����java�ӿڱ���\n");
#endif
		if(env->ExceptionCheck())
		{
			env->ExceptionDescribe();
			env->ExceptionClear();
		}
		jvm->DetachCurrentThread();
		return STOP_ERROR_NOR_STOP_JAVA_SEND_MSG_IS_ERROR;
	}
	jvm->DetachCurrentThread();
	
	return RET_OK;
}

int BytesToUtf8(char* src, char* dst, int* nout) {
	size_t n_in = strlen(src);
	size_t n_out = *nout;
	iconv_t c = iconv_open("UTF-8", "GB2312");
	if (c == (iconv_t)-1) {
		cerr << strerror(errno) << endl;
		return NULL;
	}
	char* inbuf = new char [n_in + 1];
	if (!inbuf) {
		iconv_close(c);
		return NULL;
	}
	
	strcpy(inbuf, src);
	memset(dst, 0, n_out);
	char* in = inbuf;
	char* out = dst;
	if (iconv(c, &in, &n_in, &out, &n_out) == (size_t)-1) {
		cerr << strerror(errno) << endl;
		out = NULL;
	}
	else {
		n_out = strlen(dst);
		out = dst;
	}
	iconv_close(c);
	*nout = n_out;
	delete[] inbuf;
	return 0;
}

