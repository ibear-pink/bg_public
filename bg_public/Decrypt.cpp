//
//  Decrypt.cpp
//  hlj_work
//
//  Created by wangzhia on 2017/4/12.
//  Copyright  2017 wangzhia All rights reserved.
//

#include "Decrypt.h"

int data_decrypt(const char *sofile, AuthInfo *authInfo, ResInfo *resInfo){
	int retCode = -1;
	void *handle;
	int (*decryptData)(AuthInfo *, ResInfo *);
	handle = dlopen(sofile, RTLD_LAZY);
	if (!handle){
		fprintf(stderr, "[%s]%s\n",sofile, dlerror());
		return retCode;
	}
	dlerror();    /* Clear any existing error */
	decryptData = (int (*)(AuthInfo *, ResInfo *)) dlsym(handle, "decryptData_auth");
	if (decryptData != NULL) {
		char authFile[1024];
		memset(authFile, 0, sizeof(authFile));
		ssize_t len = readlink("/proc/self/exe", authFile, sizeof(authFile)-1);
		if (len != -1) {
			authFile[len] = '\0';
			authInfo->authFile = strdup(authFile);
			printf("authFile[%s]\n", authInfo->authFile);
			retCode = (*decryptData)(authInfo, resInfo);
		} else {
			fprintf(stderr, "readlink %s failed.\n", "/proc/self/exe");
		}
	} else {
		fprintf(stderr, "decryptData is null %s\n", dlerror());
	}
	dlclose(handle);
	return retCode;
}

void free_mem(AuthInfo *authInfo, ResInfo *resInfo) {
	free(authInfo->resTag);
	authInfo->resTag = NULL;
	free(authInfo->authFile);
	authInfo->authFile = NULL;
	free(authInfo->authTag);
	authInfo->authTag = NULL;
	
	free(resInfo->hostIP);
	resInfo->hostIP = NULL;
	free(resInfo->hostUser);
	resInfo->hostUser = NULL;
	free(resInfo->hostPasswd);
	resInfo->hostPasswd = NULL;
	free(resInfo->dbServ);
	resInfo->dbServ = NULL;
	free(resInfo->dbURL);
	resInfo->dbURL = NULL;
	free(resInfo->dbUser);
	resInfo->dbUser = NULL;
	free(resInfo->dbPasswd);
	resInfo->dbPasswd = NULL;
}
