//
//  EsbWebPub.hpp
//  hlj_work
//
//  Created by wangzhia on 2017/3/17.
//  Copyright  2017wangzhia. All rights reserved.
//

#ifndef EsbWebPub_hpp
#define EsbWebPub_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <string.h>
#include "httpclient.h"
#include "cJSON.h"
#include "Define.h"

using namespace std;

//extern int PublicEsb(string url,string post,char *flag_out);
extern int PublicEsb(string url,string post,vector<string>& out_str);

#endif /* EsbWebPub_hpp */
