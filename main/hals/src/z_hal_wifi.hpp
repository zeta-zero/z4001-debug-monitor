/*--------------------------------------------------------------------
@file            : z_hal_wifi.hpp
@brief           : 
----------------------------------------------------------------------
@author          : zhangpeiwei02@gmail.com
 Release Version : 0.0.1
 Release Date    : 2024/4/5
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


#ifndef __Z_HAL_WIFI_HPP__
#define __Z_HAL_WIFI_HPP__

#include <functional>
#include "cstdint"
#include "cstring"

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_dpp.h"
#include "esp_log.h"
#include "nvs.h"

class zHal_WiFi{
public:

    void Init()
    {
        ESP_ERROR_CHECK(esp_netif_init());
        ESP_ERROR_CHECK(esp_event_loop_create_default());
        NetIf = esp_netif_create_default_wifi_sta();

        wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
        ESP_ERROR_CHECK(esp_wifi_init(&cfg));

        ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, WifiEvent, NULL));
        ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, WifiEvent, NULL));

        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    }

    void Connect(char _ssid[32],char _password[64]){
        wifi_config_t wifi_config = {};
        memcpy(wifi_config.sta.ssid,_ssid,32);
        memcpy(wifi_config.sta.password,_password,64);
        /* Authmode threshold resets to WPA2 as default if password matches WPA2 standards (pasword len => 8).
        * If you want to connect the device to deprecated WEP/WPA networks, Please set the threshold value
        * to WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK and set the password with length and format matching to
        * WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK standards.
        */
        wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA_WPA2_PSK;
        wifi_config.sta.sae_pwe_h2e = WPA3_SAE_PWE_BOTH;
        wifi_config.sta.pmf_cfg.capable = true;
        wifi_config.sta.pmf_cfg.required = false;
        
        ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );

    }

    void StartDPPModel(bool _on)
    {

    }

    void Start(void)
    {
        ESP_ERROR_CHECK(esp_wifi_start());
    }

    void Stop(void)
    {
        esp_wifi_stop();
    }

    uint32_t GetIP(void)
    {
        uint32_t res = 0;
        if(NetIf == nullptr){
            goto end;
        }
        esp_netif_ip_info_t ip_info;
        esp_netif_get_ip_info(NetIf,&ip_info);

        res = ip_info.ip.addr;
    end:
        return res;
    }

    bool GetStatus(void)
    {
        return IsConnected;
    }

private:
    char SSID[32];
    char Password[64];
    static bool IsConnected;
    esp_netif_t *NetIf;
    
    static void WifiEvent(void *arg, esp_event_base_t event_base,int32_t event_id, void *event_data)
    {
        if(WIFI_EVENT == event_base){
            switch(event_id){
                case WIFI_EVENT_STA_START:{
                    esp_wifi_connect();
                }break;
                case WIFI_EVENT_STA_CONNECTED:{
                    IsConnected = true;
                }break;
                case WIFI_EVENT_STA_DISCONNECTED:{
                    IsConnected = false;
                }break;
                case IP_EVENT_STA_GOT_IP:{

                }break;
                default:break;
            }
        }
        else if(IP_EVENT == event_base){
            switch(event_id){
                case IP_EVENT_STA_GOT_IP:{
                    ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
                    ESP_LOGI("HAL_WIFI", "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
                }break;
                default:break;
            }
        }
        
    }

};


#endif  // __Z_HAL_WIFI_HPP__

