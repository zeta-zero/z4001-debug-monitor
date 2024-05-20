/*--------------------------------------------------------------------
@file            : z_app_webserver.cpp
@brief           : 
----------------------------------------------------------------------
@author          : zhangpeiwei02@gmail.com
 Release Version : 0.0.1
 Release Date    : 2024/5/18
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

#include "z_app_webserver.hpp"
#include "z_app_aisle.hpp"
#include "esp_log.h"
#include "esp_err.h"
#include "../comm/z_comm_base64.hpp"


// DEFINE ------------------------------------------------------------
#define TAG  "APP_WEBSVR"

#define CONST_RES_PATH(_VAL_) "/sd/Web" # _VAL_

#define URI_MATCH(_URI_,_BUF_,_LEN_) ((sizeof(_URI_) - 1) == _LEN_ && memcmp(_URI_,_BUF_,_LEN_) == 0)
#define STRING_MATCH(_URI_,_BUF_,_LEN_) ((sizeof(_URI_) - 1) <= _LEN_ && memcmp(_URI_,_BUF_,(sizeof(_URI_) - 1)) == 0)

#define OCT_BASE64_FLAG  "enB3B-Base64"


#define SPEC_URI_HOMW   "/home"

#define TEXT_CONTENT_TYPE  "text/plain"

#define POST_MAX_RECV_LEN    4096

// TYPE ------------------------------------------------------------

// VALUE ------------------------------------------------------------
static const char* AnyURIsName = "/*";

static const char* ContentTypeMap[][2] = {
    {".html",HTTPD_TYPE_TEXT},
    {".js","application/javascript"},
    {".css","text/css"},
    {".json",HTTPD_TYPE_JSON},
    {".png","image/png"},
    {".jpe","image/jpeg"},
    {".jpep","image/jpeg"},
    {".jpg","image/jpeg"},
    {".gif","image/gif"},
    {".ico","image/x-icon"},
    {".*",HTTPD_TYPE_OCTET},
};

/**-------------------------------------------------------------------
 * @fn     : zApp_WebSvr
 * @brief  : 
 * @param  : none
 * @return : res
 */
zApp_WebSvr::zApp_WebSvr()
{
    Enable = false;
    RegURI();
}

/**-------------------------------------------------------------------
 * @fn     : RegURI
 * @brief  : 
 * @param  : none
 * @return : res
 */
void zApp_WebSvr::RegURI(void)
{
    URINum = 0;

    URIList[URINum].uri = AnyURIsName;
    URIList[URINum].method = HTTP_GET;
    URIList[URINum].handler = AnyGetHandle;
    URINum++;
    
    URIList[URINum].uri = AnyURIsName;
    URIList[URINum].method = HTTP_POST;
    URIList[URINum].handler = AnyPostHandle;
    URINum++;
}



/**-------------------------------------------------------------------
 * @fn     : Start
 * @brief  : 
 * @param  : none
 * @return : res
 */
void zApp_WebSvr::Start()
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;
    config.uri_match_fn = MatchURIEvent;

    if (httpd_start(&Svr_Handle, &config) != ESP_OK) {
        ESP_LOGI(TAG, "Error starting server!");
        goto end;
    }
    esp_event_handler_register(ESP_HTTP_SERVER_EVENT,ESP_EVENT_ANY_ID,EventHandle,this);
    // Set URI handlers
    for(int i = 0;i<URINum;i++){
        httpd_register_uri_handler(Svr_Handle, &URIList[i]);
    }
    ESP_LOGI(TAG, "Registering URI handlers");

    Enable = true;
    
end:
    return;
}

/**-------------------------------------------------------------------
 * @fn     : Stop
 * @brief  : 
 * @param  : none
 * @return : res
 */
void zApp_WebSvr::Stop()
{
    if(Enable == false){
        return;
    }
    httpd_stop(Svr_Handle);
    esp_event_handler_unregister(ESP_HTTP_SERVER_EVENT,ESP_EVENT_ANY_ID,EventHandle);
    Enable = false;
}

/**-------------------------------------------------------------------
 * @fn     : MatchURIEvent
 * @brief  : 
 * @param  : none
 * @return : res
 */
bool zApp_WebSvr::MatchURIEvent(const char *reference_uri,const char *uri_to_match,size_t match_upto)
{
    bool res = false;
    if(memcmp(reference_uri,"/*",2) == 0){
        res = true;
        goto end;
    }
    else{
        res = memcmp(reference_uri,uri_to_match,match_upto) == 0;
    }

end:
    return res;
}

/**-------------------------------------------------------------------
 * @fn     : EventHandle
 * @brief  : 
 * @param  : none
 * @return : res
 */
void zApp_WebSvr::EventHandle(void* event_handler_arg,esp_event_base_t event_base,int32_t event_id,void* event_data)
{
    zApp_WebSvr *local_web_svr = (zApp_WebSvr*)event_data;
    if(event_base != ESP_HTTP_SERVER_EVENT || local_web_svr == NULL){
        goto end;
    }
    switch(event_id){
        case HTTP_SERVER_EVENT_ERROR:{
            
        }break;
        case HTTP_SERVER_EVENT_START:{
            
        }break;
        case HTTP_SERVER_EVENT_ON_CONNECTED:{
            
        }break;
        case HTTP_SERVER_EVENT_ON_HEADER:{
            
        }break;
        case HTTP_SERVER_EVENT_HEADERS_SENT:{
            
        }break;
        case HTTP_SERVER_EVENT_ON_DATA:{
            
        }break;
        case HTTP_SERVER_EVENT_SENT_DATA:{
            
        }break;
        case HTTP_SERVER_EVENT_DISCONNECTED:{
            
        }break;
        case HTTP_SERVER_EVENT_STOP:{
            
        }break;
        default:break;
    }
end:
    return;
}

/**-------------------------------------------------------------------
 * @fn     : AnyGetHanlde
 * @brief  : 
 * @param  : none
 * @return : res
 */
esp_err_t zApp_WebSvr::AnyGetHandle(httpd_req_t *req)
{
    esp_err_t res = ESP_OK;
    size_t len = 0;
    size_t remain = 0,pos = 0;
    const char* uribuf = NULL,*query = NULL,*fragment = NULL,*suffix = NULL;
    size_t query_len = 0,fragment_len = 0,path_len = 0,offset = 0;
    char path[256];
    datapack_t data={NULL,0};
    len = strlen(req->uri);
    uribuf = req->uri;
    if(uribuf == NULL || len == 0){
        ESP_LOGI(TAG, "Fail %.*s", len, uribuf);
        httpd_resp_send_404(req);
        goto end;
    }
    if(URI_MATCH("/",uribuf,len) || URI_MATCH("/home",uribuf,len)){
        // data = ReadDataFromPath(path);
        httpd_resp_set_type(req, HTTPD_TYPE_TEXT);
        sprintf(path,"%s/Web/index.html",LocalUnits.SDPath);
        SendFile(req,path);
        
    }
    else if(URI_MATCH("/favicon.ico",uribuf,len)){
        // data = ReadDataFromPath(path);
        httpd_resp_set_type(req, "image/x-icon");
        sprintf(path,"%s/Web/Res/ticon.ico",LocalUnits.SDPath);
        SendFile(req,path);
    }
    else{
        query = (const char*)memchr(uribuf+1,'?',len - 1) + 1;
        fragment = (const char*)memchr(uribuf+1,'#',len - 1) + 1;
        if(fragment != NULL){
            fragment_len = len - ((size_t)fragment - (size_t)uribuf);
            offset++;
        }
        if(query != NULL){
            query_len = len - ((size_t)query - (size_t)uribuf) - fragment_len - offset;
            offset++;
        }
        path_len = len - fragment_len - query_len - offset;
        suffix = (const char*)memchr(uribuf,'.',path_len);
        if(suffix != NULL){
            for(int i = 0;i<sizeof(ContentTypeMap)/(sizeof(char*) * 2);i++){
                if(strstr(uribuf,ContentTypeMap[i][0]) != NULL){
                    httpd_resp_set_type(req, ContentTypeMap[i][1]);
                    break;
                }
            }
            sprintf(path,"%s/Web%.*s",LocalUnits.SDPath,path_len,uribuf);
            SendFile(req,path);
            // data = ReadDataFromPath(path);

        }
        else{

        } 
    }

    // if(data.Data == NULL || data.Len == 0){
    //     httpd_resp_send_404(req);
    // }
    // else{
    //     if(len < 512){
    //         httpd_resp_send(req,(char*)data.Data,data.Len);
    //     }
    //     else{
    //         remain = data.Len;
    //         while(remain > 512){
    //             httpd_resp_send_chunk(req,(char*)&data.Data[pos],512);
    //             pos += 512;
    //             remain -= 512;
    //         }
    //         httpd_resp_send_chunk(req,(char*)&data.Data[pos],remain);
    //         httpd_resp_send_chunk(req,NULL,0);
    //     }
    // }

end:
    ReleasDataPack(&data);
    return res;
}


/**-------------------------------------------------------------------
 * @fn     : AnyPostHandle
 * @brief  : 
 * @param  : none
 * @return : res
 */
esp_err_t zApp_WebSvr::AnyPostHandle(httpd_req_t *req)
{
    /* 定义 HTTP POST 请求数据的目标缓存区
     * httpd_req_recv() 只接收 char* 数据，但也可以是任意二进制数据（需要类型转换）
     * 对于字符串数据，null 终止符会被省略，content_len 会给出字符串的长度 */
    char *content = NULL,*senddata = NULL;
    uint8_t* rawdata = NULL;
    uint16_t rawlen = 0,aislelen = 0,sendlen = 0;
    char contenttype[64];
    size_t typelen = 0;
    esp_err_t res = ESP_FAIL;
    int timeoutcount = 0;
    size_t remainlen = 0,procnum = 0;
    int readlen = 0;

    size_t recv_size = MIN(req->content_len, POST_MAX_RECV_LEN);
    content = (char*)calloc(recv_size,1);

    typelen = httpd_req_get_hdr_value_len(req,"Content-Type");
    httpd_req_get_hdr_value_str(req,"Content-Type",contenttype,64);

    remainlen = req->content_len;
    while(remainlen > 0){
        readlen = recv_size;
        readlen = httpd_req_recv(req, content, readlen);
        if (readlen < 0) {  /* 返回 0 表示连接已关闭 */
            /* 检查是否超时 */
            if (readlen == HTTPD_SOCK_ERR_TIMEOUT) {
                timeoutcount++;
                if(timeoutcount > 3){
                    httpd_resp_send_408(req);
                }
                continue;
            }
            else{
                httpd_resp_send_404(req);
            }
            goto end;
        }
        else if(readlen == 0){
            break;
        }
        else{
            timeoutcount = 0;
            if(STRING_MATCH(TEXT_CONTENT_TYPE,contenttype,typelen)){
                if(procnum == 0){
                    if(STRING_MATCH(OCT_BASE64_FLAG,content,readlen)){
                        if(req->content_len > POST_MAX_RECV_LEN){
                            httpd_resp_send(req, OCT_BASE64_FLAG" Fail,Data Over 4096", sizeof(OCT_BASE64_FLAG" Fail,Data Over 4096")); 
                            goto end;
                        }
                        else{
                            rawlen = readlen - sizeof(OCT_BASE64_FLAG) + 1;
                            rawdata = (uint8_t*)calloc(rawlen,1);
                            rawlen = zCommBase64::Decode((uint8_t*)&content[sizeof(OCT_BASE64_FLAG)-1],readlen - sizeof(OCT_BASE64_FLAG) + 1,rawdata,rawlen);
                            if(zAppAisle::Send(zAppAisle::Ch_HTTP_Post,zAppAisle::Ch_Center,rawdata,rawlen) != ESP_OK){
                                httpd_resp_send(req, OCT_BASE64_FLAG" Fail,Dev No MEM", sizeof(OCT_BASE64_FLAG" Fail,Dev No MEM")); 
                                goto end;
                            }
                            httpd_resp_set_status(req, "200 Ok");
                            httpd_resp_set_type(req, HTTPD_TYPE_TEXT);
                            httpd_resp_send(req,"",0);

                            if(zAppAisle::Recv(zAppAisle::Ch_HTTP_Post,NULL,rawdata,rawlen,&aislelen,100) != ESP_OK){
                                if(httpd_resp_send(req, OCT_BASE64_FLAG" Fail,Dev No Resp", sizeof(OCT_BASE64_FLAG" Fail,Dev No Resp")) != ESP_OK){
                                    ESP_LOGW(TAG,"Http Send Fail");
                                }
                                goto end;
                            }
                            if(aislelen  == 0){
                                goto end;
                            }
                            sendlen = sizeof(OCT_BASE64_FLAG) + 4 * ((aislelen + 2) / 3);
                            senddata = (char*)calloc(sizeof(OCT_BASE64_FLAG) + 4 * ((aislelen + 2) / 3),1);
                            memcpy(senddata,OCT_BASE64_FLAG,sizeof(OCT_BASE64_FLAG) - 1);
                            sendlen = zCommBase64::Encode(rawdata,aislelen,(uint8_t*)&senddata[sizeof(OCT_BASE64_FLAG)-1],sendlen);
                            if(sendlen != 0){
                                httpd_resp_set_type(req,TEXT_CONTENT_TYPE);
                                httpd_resp_send(req, senddata, sendlen); 
                            }
                        }
                    }
                }
                else{

                }
            }
            else if(STRING_MATCH(HTTPD_TYPE_OCTET,contenttype,typelen)){

            }
            else{

            }
        }
        procnum++;
        remainlen -= readlen;
    }
    res = ESP_OK;
end:
    if(content != NULL){free(content);}
    if(senddata != NULL){free(senddata);}
    if(rawdata != NULL){free(rawdata);}
    return res;
}

/**-------------------------------------------------------------------
 * @fn     : ReadDataFromPath
 * @brief  : 
 * @param  : none
 * @return : res
 */
zApp_WebSvr::datapack_t zApp_WebSvr::ReadDataFromPath(char* _filepath)
{
    datapack_t res={NULL,0};
    FILE *file_content = NULL;
    file_content = fopen(_filepath,"r");
    if(file_content == NULL){
        ESP_LOGW(TAG,"Open Fail %s",_filepath);
        goto end;
    }
    fseek(file_content,0,SEEK_END);
    res.Len = ftell(file_content);
    fseek(file_content,0,SEEK_SET);
    res.Data = (uint8_t*)malloc(res.Len);
    res.Len = fread(res.Data,1,res.Len,file_content);
    if(res.Len == 0){
        ESP_LOGW(TAG,"Read Fail %s",_filepath);
        free(res.Data);
        res.Data = NULL;
        goto end;
    }
end:
    if(file_content){fclose(file_content);}
    return res;
}

/**-------------------------------------------------------------------
 * @fn     : ReleasDataPack
 * @brief  : 
 * @param  : none
 * @return : res
 */
void zApp_WebSvr::ReleasDataPack(zApp_WebSvr::datapack_t *_val)
{
    if(_val == NULL){
        return;
    }
    if(_val->Data == NULL){
        return;
    }
    free(_val->Data);
    _val->Len = 0;
}

/**-------------------------------------------------------------------
 * @fn     : ReleasDataPack
 * @brief  : 
 * @param  : none
 * @return : res
 */
esp_err_t zApp_WebSvr::SendFile(httpd_req_t *req,const char* _filepath)
{
    esp_err_t res= ESP_FAIL;
    size_t filelen = 0;
    size_t sendlen = 0;
    uint8_t *data = NULL;
    FILE *file_content = NULL;
    file_content = fopen(_filepath,"r");
    if(file_content == NULL){
        ESP_LOGW(TAG,"Open Fail %s",_filepath);
        goto end;
    }
    // 获取文件大小
    fseek(file_content,0,SEEK_END);
    filelen = ftell(file_content);
    sendlen = filelen>1024?1024:filelen;
    // 恢复文件指针指向的位置，0
    fseek(file_content,0,SEEK_SET);
    // 申请内存
    data = (uint8_t*)malloc(sendlen);
    // 文件大小大于0 且 内存申请成功
    while(filelen>0 && data != NULL){
        sendlen = filelen>1024?1024:filelen;
        sendlen = fread(data,1,sendlen,file_content);
        if(sendlen == 0){
            goto end;
        }
        httpd_resp_send_chunk(req,(char*)data,sendlen);
        filelen -= sendlen;
    }
    httpd_resp_send_chunk(req,NULL,0);
    
end:
    if(file_content){fclose(file_content);}
    if(data != NULL){free(data);}
    return res;
}














