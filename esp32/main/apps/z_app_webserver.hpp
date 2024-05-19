/*--------------------------------------------------------------------
@file            : z_app_webserver.hpp
@brief           : 
----------------------------------------------------------------------
@author          : zhangpeiwei02@gmail.com
 Release Version : 0.0.1
 Release Date    : 2024/05/18
----------------------------------------------------------------------
@attention       :
Copyright [2024] [copyright holder]     
     
Licensed under the Apache License, Version 2.0 (the "License");     
you may not use this file except in compliance with the License.     
You may obtain a copy of the License at     
  http://www.apache.org/licenses/LICENSE-2.0     
Unless required by applicable law or agreed to in writing, software     
distributed under the License is distributed on an "AS IS" BASIS,     
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.     
See the License for the specific language governing permissions and     
limitations under the License.     

--------------------------------------------------------------------*/

#ifndef __Z_APP_WEBSERVER_HPP__
#define __Z_APP_WEBSERVER_HPP__

#include "../units/z_units.hpp"
#include "esp_http_server.h"
#include "esp_vfs_fat.h"

#define URI_LIST_MAX    10

class zApp_WebSvr{
public:
    zApp_WebSvr(void);

    void Start(void);
    void Stop(void);

private:
    typedef struct{
        uint8_t *Data;
        size_t Len;
    }datapack_t;
    httpd_handle_t Svr_Handle;
    httpd_uri_t  URIList[URI_LIST_MAX];
    int URINum;
    bool Enable;
    void RegURI(void);
    
    static bool MatchURIEvent(const char *reference_uri,const char *uri_to_match,size_t match_upto);
    static void EventHandle(void* event_handler_arg,esp_event_base_t event_base,int32_t event_id,void* event_data);
    static esp_err_t AnyGetHandle(httpd_req_t *req); 
    static esp_err_t AnyPostHandle(httpd_req_t *req);
    static datapack_t ReadDataFromPath(char* _filepath);
    static void ReleasDataPack(datapack_t *_val);
};



#endif //__Z_APP_WEBSERVER_HPP__
