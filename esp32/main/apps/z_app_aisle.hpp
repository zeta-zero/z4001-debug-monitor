/*--------------------------------------------------------------------
@file            : z_app_aisle.hpp
@brief           : 
----------------------------------------------------------------------
@author          : zhangpeiwei02@gmail.com
 Release Version : 0.0.1
 Release Date    : 2024/05/19
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

#ifndef __Z_COMM_AISLE_HPP__
#define __Z_COMM_AISLE_HPP__

#include "cstdint"
#include "cctype"
#include "cstring"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

class zAppAisle{

public:
    typedef enum{
        Ch_None,
        Ch_Center,
        Ch_HTTP_Post,
    }channel_t;

    typedef struct{
        uint32_t Len;
        channel_t SrcCh;
        uint8_t Data[0];
    }datapack_t;

    static esp_err_t Send(channel_t _srcch,channel_t _desch,uint8_t *_data,uint16_t _len,uint16_t _delayms = 0)
    {
        esp_err_t res = ESP_FAIL;
        datapack_t *buf = NULL;
        if(_data == NULL || _len == 0){
            goto end;
        }
        switch(_desch){
            case Ch_HTTP_Post:{
                if(HandleList.HTTPPostEnable == false){
                    HandleList.HTTPPostHandle = xQueueCreate(20,sizeof(datapack_t*));
                    HandleList.HTTPPostEnable = true;
                }
                buf = (datapack_t*)malloc(sizeof(datapack_t) + _len);
                if(buf == 0){
                    res = ESP_ERR_NO_MEM;
                    goto end;
                }
                memcpy(buf->Data,_data,_len);
                buf->SrcCh = _srcch;
                buf->Len = _len;
                if(xQueueSend(HandleList.HTTPPostHandle,&buf,pdMS_TO_TICKS(_delayms)) != pdTRUE){
                    free(buf);
                    goto end;
                }
                
            }break;
            case Ch_Center:{
                if(HandleList.CenterEnable == false){
                    HandleList.CenterHandle = xQueueCreate(20,sizeof(datapack_t*));
                    HandleList.CenterEnable = true;
                }
                buf = (datapack_t*)malloc(sizeof(datapack_t) + _len);
                if(buf == 0){
                    ESP_LOGW("Aisle","Send Fail Center No Mem");
                    res = ESP_ERR_NO_MEM;
                    goto end;
                }
                memcpy(buf->Data,_data,_len);
                buf->SrcCh = _srcch;
                buf->Len = _len;
                if(xQueueSend(HandleList.CenterHandle,&buf,_delayms==0?0:pdMS_TO_TICKS(_delayms)) != pdTRUE){
                    ESP_LOGW("Aisle","Send Fail Center");
                    free(buf);
                    goto end;
                }
                ESP_LOGI("Aisle","Send Ok Center");
            }break;
            default:break;
        }
        res = ESP_OK;
    end:
        return res;
    }

    static esp_err_t Recv(channel_t _setch,channel_t *_srcch,uint8_t *_data,uint16_t _len,uint16_t *_recvlen,uint16_t _delayms = 0)
    {
        esp_err_t res = ESP_FAIL;
        datapack_t *buf = NULL;
        if(_data == NULL || _len == 0 || _recvlen == 0){
            res = ESP_ERR_INVALID_ARG;
            goto end;
        }
        switch(_setch){
            case Ch_HTTP_Post:{
                if(HandleList.HTTPPostEnable == false){
                    HandleList.HTTPPostHandle = xQueueCreate(20,sizeof(datapack_t*));
                    HandleList.HTTPPostEnable = true;
                }
                if(xQueueReceive(HandleList.HTTPPostHandle,&buf,pdMS_TO_TICKS(_delayms)) != pdTRUE){
                    res = ESP_ERR_NOT_FOUND;
                    goto end;
                }
                if(buf == NULL || buf->Len == 0){
                    res = ESP_ERR_NOT_FOUND;
                    goto end;
                }
                if(buf->Len > _len){
                    xQueueSendToFront(HandleList.HTTPPostHandle,&buf,pdMS_TO_TICKS(_delayms));
                    res = ESP_ERR_INVALID_SIZE;
                    goto end;
                }
                
            }break;
            case Ch_Center:{
                if(HandleList.CenterEnable == false){
                    HandleList.CenterHandle = xQueueCreate(20,sizeof(datapack_t*));
                    HandleList.CenterEnable = true;
                }
                if(xQueueReceive(HandleList.CenterHandle,&buf,pdMS_TO_TICKS(_delayms)) != pdTRUE){
                    res = ESP_ERR_NOT_FOUND;
                    goto end;
                }
                if(buf == NULL || buf->Len == 0){
                    res = ESP_ERR_NOT_FOUND;
                    goto end;
                }
                if(buf->Len > _len){
                    xQueueSendToFront(HandleList.CenterHandle,&buf,pdMS_TO_TICKS(_delayms));
                    res = ESP_ERR_INVALID_SIZE;
                    goto end;
                }
                memcpy(_data,buf->Data,buf->Len);
            }break;
            default:break;
        }
        if(buf != NULL){
            memcpy(_data,buf->Data,buf->Len);
            if(_srcch != NULL){*_srcch = buf->SrcCh;}
            *_recvlen = buf->Len;
        }
        res = ESP_OK;
    end:
        if(buf != NULL){free(buf);}
        return res;
    }

    static esp_err_t Recv(channel_t _setch,datapack_t** _srcdata,uint16_t _delayms = 0)
    {
        esp_err_t res = ESP_FAIL;
        if(_srcdata == NULL)
        {
            goto end;
        }
        switch(_setch){
            case Ch_HTTP_Post:{
                if(HandleList.HTTPPostEnable == false){
                    HandleList.HTTPPostHandle = xQueueCreate(20,sizeof(datapack_t*));
                    HandleList.HTTPPostEnable = true;
                }
                if(xQueueReceive(HandleList.HTTPPostHandle,_srcdata,pdMS_TO_TICKS(_delayms)) != pdTRUE){
                    res = ESP_ERR_NOT_FOUND;
                    goto end;
                }
                
            }break;
            case Ch_Center:{
                if(HandleList.CenterEnable == false){
                    HandleList.CenterHandle = xQueueCreate(20,sizeof(datapack_t*));
                    HandleList.CenterEnable = true;
                }
                if(xQueueReceive(HandleList.CenterHandle,_srcdata,_delayms==0?0:pdMS_TO_TICKS(_delayms)) != pdTRUE){
                    res = ESP_ERR_NOT_FOUND;
                    goto end;
                }
            }break;
            default:break;
        }
        res = ESP_OK;
    end:
        return res;
    }
private:
    typedef struct{
        QueueHandle_t HTTPPostHandle;
        QueueHandle_t CenterHandle;
        bool HTTPPostEnable;
        bool CenterEnable;
    }handlelist_t;
    static handlelist_t HandleList;

};

#endif  //__Z_COMM_AISLE_HPP__

