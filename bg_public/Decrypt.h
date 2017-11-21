//
//  Decrypt.cpp
//  hlj_work
//
//  Created by wangzhia on 2017/4/12.
//  Copyright  2017 wangzhia All rights reserved.
//


#ifndef Decrypt_h
#define Decrypt_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <unistd.h>

#ifdef  __cplusplus
extern "C" {
#endif
	
	typedef struct auth_info_st {
		char *resTag;
		char *authTag;
		char *authFile;
	} AuthInfo;
	typedef struct res_info_st {
		int resType;
		char *hostIP;
		char *hostUser;
		char *hostPasswd;
		char *dbServ;
		char *dbURL;
		char *dbUser;
		char *dbPasswd;
	} ResInfo;
	
	typedef enum return_code_em {
		process_successful = 0,
		process_failed = 1,
		passwdfile_load_failed = 2,
		passwdfile_bad_format = 3,
		argument_is_illegal = 4,
		tag_is_invalid = 5,
		auth_FILES_failed = 6,
		auth_HOSTIP_failed = 7,
		auth_KEY_expired = 8
	} return_code;
	
	typedef enum resource_type_em {
		HOST = 1,
		DB = 2
	} resource_type;
	
	/*返回值：
	 0 - 处理成功；
	 1 - 处理失败；
	 2 - 密码文件不存在或无读权限；
	 3 - 密码文件格式不正确；
	 4 - 不合法的参数；
	 5 - 无效的标签值；
	 6 - 授权文件认证失败；
	 7 - 授权IP地址认证失败；
	 8 - 授权密钥已过期。
	 */
	int data_decrypt(const char *sofile, AuthInfo *authInfo, ResInfo *resInfo);
	void free_mem(AuthInfo *authInfo, ResInfo *resInfo);
	
#ifdef  __cplusplus
}
#endif
#endif /* Decrypt_h */
