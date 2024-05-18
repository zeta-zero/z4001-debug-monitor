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

class zApp_WebSvrProc{
public:
    static esp_err_t RootGetHandle(httpd_req_t *req){
        char path[128];
        uint8_t *data = NULL;
        UINT len = 0;
        esp_err_t res = ESP_OK;
        sprintf(path,"%s/Web/Home/index.html",LocalUnits.SDPath);
        data = ReadData(path,&len);
        if(data == NULL){
            httpd_resp_send_404(req);
            ESP_LOGE("WEBProc","Root Fail");
            goto end;
        }
        httpd_resp_set_type(req, HTTPD_TYPE_TEXT);
        httpd_resp_send(req,(char*)data,len);

    end:
        Release(data);
        return res;
    }

    static esp_err_t HomeStyleGetHandle(httpd_req_t *req){
        char path[128];
        uint8_t *data = NULL;
        UINT len = 0;
        esp_err_t res = ESP_OK;
        sprintf(path,"%s/Web/Home/homestyle.css",LocalUnits.SDPath);
        data = ReadData(path,&len);
        if(data == NULL){
            httpd_resp_send_404(req);
            ESP_LOGE("WEBProc","home style Fail");
            goto end;
        }
        httpd_resp_set_type(req, "text/css");
        httpd_resp_send(req,(char*)data,len);

    end:
        Release(data);
        return res;
    }

    static esp_err_t FaviconGetHandle(httpd_req_t *req){
        char path[128];
        uint8_t *data = NULL;
        UINT len = 0;
        esp_err_t res = ESP_OK;
        sprintf(path,"%s/Web/Res/ticon.ico",LocalUnits.SDPath);
        data = ReadData(path,&len);
        if(data == NULL){
            httpd_resp_send_404(req);
            ESP_LOGE("WEBProc","Favicon Fail",);
            res = ESP_FAIL;
            goto end;
        }
        httpd_resp_set_type(req, "image/x-icon");
        httpd_resp_send(req,(char*)data,len);
        Release(data);
    end:
        return res;
    }

    static esp_err_t AnyGetHanlde(httpd_req_t *req){
        esp_err_t res = ESP_OK;
        char buf[100];
        int len = 0;
        /* Read the data for the request */
        len = httpd_req_recv(req, buf,100);
        
        ESP_LOGI("WEBProc", "=========== RECEIVED DATA ==========");
        ESP_LOGI("WEBProc", "%.*s", len, buf);
        ESP_LOGI("WEBProc", "====================================");

        return res;
    }

private:
    static uint8_t *ReadData(char* _filepath,UINT *_len){
        uint8_t *res=NULL;
        FILE *file_content = NULL;
        FRESULT frst = FR_OK;
        int datasize = 0;
        file_content = fopen(_filepath,"r");
        if(file_content == NULL){
            ESP_LOGW("WebProc","Open Fail %s",_filepath);
            goto end;
        }
        fseek(file_content,0,SEEK_END);
        datasize = ftell(file_content);
        fseek(file_content,0,SEEK_SET);
        res = (uint8_t*)malloc(datasize);
        *_len = fread(res,1,datasize,file_content);
        if(*_len == 0){
            ESP_LOGW("WebProc","Read Fail %s",_filepath);
            free(res);
            res = NULL;
            goto end;
        }
    end:
        if(file_content){fclose(file_content);}
        return res;
    }

    static void Release(uint8_t* _data){
        if(_data != NULL){
            free(_data);
        }
    }


};

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
    httpd_uri_t  URIList;
    int URINum;
    bool Enable;
    zApp_WebSvrProc Proc;

    void RegURI(void);
    
    static bool MatchURIEvent(const char *reference_uri,const char *uri_to_match,size_t match_upto);
    static void EventHandle(void* event_handler_arg,esp_event_base_t event_base,int32_t event_id,void* event_data);
    static esp_err_t AnyGetHandle(httpd_req_t *req); 
    static datapack_t ReadDataFromPath(char* _filepath);
    static void ReleasDataPack(datapack_t *_val);
};



#endif //__Z_APP_WEBSERVER_HPP__
