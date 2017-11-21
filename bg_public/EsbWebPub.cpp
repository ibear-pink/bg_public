//
//  EsbWebPub.cpp
//  hlj_work
//
//  Created by wangzhia on 2017/3/17.
//  Copyright  2017 wangzhia. All rights reserved.
//

#include "EsbWebPub.h"

int PublicEsb(string url,string post,vector<string>& out_str)
{
	/*c++调用esb测试代码*/
	CHttpClient *a = new CHttpClient();//程序加载时，只需初始化一次，后续可直接使用
	//string strUrl = string("http://172.21.3.96:51000/esbWS/rest/com_sitech_res_inter_outinter_ISChkGoodPhoneSvc_checkGoodPhone");//入参地址
	//string strPost = string("{\"ROOT\":{\"HEADER\":{\"POOL_ID\":\"11\",\"DB_ID\":\"\",\"ENV_ID\":\"\",\"ROUTING\":{\"ROUTE_KEY\":\"\",\"ROUTE_VALUE\":\"\"}},\"BODY\":{\"LOGIN_NO\":\"crm123456\",\"PHONE_NO\": \"13944161918\",\"REGION_CODE\": \"2201\",\"RULE_TYPE\": \"\"}}}");//入参json
	string strUrl = url;
	string strPost = post;
	string strResponse;//返回值json
	
	if( a->Post(strUrl, strPost, strResponse) < 0)
	{
		if ( NULL!=a )
		{
			delete(a);
		}
		printf("POST ERROR!!!!\n");
		return RET_FAIL;
	}
	if ( NULL!=a )
	{
		delete(a);
	}
#ifdef _DEBUG_INFO_
	//printf("strResponse=[%s]\n",strResponse.c_str());
#endif
	char jsonString[1024] = {0};

	string flag;
	string msg;
	//char* out;
	memset(jsonString,0,sizeof(jsonString));
	sprintf(jsonString,"%s",strResponse.c_str());
	printf("jsonString=%s\n",jsonString);
	cJSON* pArray = NULL;
	cJSON* pRoot = cJSON_Parse ( jsonString );
	if ( pRoot )
	{
		//printf("pRoot=%s\n",cJSON_Print(pRoot));
		cJSON* pArrayItem = NULL;
		//cJSON* objJsonItem = NULL;
		cJSON* pArrayItem_out = NULL;
		//cJSON* objJsonItem_out = NULL;
		pArray = cJSON_GetObjectItem ( pRoot, "ROOT" );
		//printf("pArray->type%d\n",pArray->type);
		//printf("cJSON_Print(pArray)=%s\n",cJSON_Print(pArray));
		//int nCount = cJSON_GetArraySize ( pArray ); //获取pArray数组的大小
		//printf("nCount=%d\n",nCount);
		/*for( int i = 1; i < nCount-1; i++)
		 {
			 pArrayItem = cJSON_GetArrayItem(pArray, i);
			 out = cJSON_Print( pArrayItem );    //将pArrayItem的值以字串的形式打印到char型buffer上，cJSON_Print()会自动分配内存空间，用完需要释放内存。
			 printf( "array item %d: %s\n", i, out);
			 objJsonItem = cJSON_GetObjectItem(pArrayItem, "OUT_DATA");
			 if (objJsonItem)
			 {
				 pArrayItem = cJSON_GetArrayItem(objJsonItem, 0);
				 out = cJSON_Print( pArrayItem );
				 printf( "array item %d: %s\n", i, out);
			 }
			 free( out );
		 } */
		pArrayItem_out = cJSON_GetArrayItem(pArray, 1);  // 0 Header; 1 Body
		
		if(NULL == pArrayItem_out)
		{
			printf("pArrayItem_out is null!!!\n");
			return RET_FAIL;
		}
		//objJsonItem_out = cJSON_GetObjectItem(pArrayItem_out, "BODY");
		//printf("pArrayItem_out=%s\n", cJSON_Print(pArrayItem_out));
		//printf("objJsonItem_out=%s\n", cJSON_Print(objJsonItem_out));
		
		pArrayItem = cJSON_GetArrayItem(pArrayItem_out, 0);
		
		if(NULL == pArrayItem)
		{
			printf("pArrayItem is null!!!\n");
			return RET_FAIL;
		}
		
		//printf("objJsonItem=%s\npArrayItem=%s\n",cJSON_Print(objJsonItem),cJSON_Print(pArrayItem));
		
		cJSON *json_flag=NULL;
		cJSON *json_msg=NULL;
		
		json_flag = cJSON_GetObjectItem(pArrayItem, "FLAG");
		json_msg  = cJSON_GetObjectItem(pArrayItem, "MSG");
		
		flag = string(json_flag->valuestring);
		msg	 = string(json_msg->valuestring);
		
		printf("flag=%s, msg=%s\n",json_flag->valuestring, json_msg->valuestring);
		
		/*if (objJsonItem)
		 {
			 pArrayItem = cJSON_GetArrayItem(objJsonItem, 0);
			 char *out = cJSON_Print( pArrayItem );
			 #ifdef _DEBUG_INFO_
			 printf( "array item %d: %s\n", i, out);
			 #endif
			 strncpy(flag,out+1,1);
			 #ifdef _DEBUG_INFO_
			 printf("flag=%s\n",flag);
			 #endif
			 free( out );
		 }*/
		cJSON_Delete ( pRoot );
	}
	out_str.push_back(flag);
	out_str.push_back(msg);
	return RET_OK;
}
