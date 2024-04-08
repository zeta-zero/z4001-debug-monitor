/*--------------------------------------------------------------------
@file            : z_hal_bluetooth.hpp
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


#ifndef __Z_HAL_BLUETOOTH_HPP__
#define __Z_HAL_BLUETOOTH_HPP__

#include "cstdint"
#include "../base/z_halbase_gpio.hpp"

#include "esp_bt.h"
#include "bt_app_core.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include "esp_a2dp_api.h"
#include "esp_avrc_api.h"

// DEFINE ------------------------------------------------------------
#define HAL_BTMODE_A2DP   0x01
#define HAL_BTMODE_GATTC  0x02
#define HAL_BTMODE_GATTS  0x04

class zHal_Bluetooth {
public:
    void Init(uint8_t _mode)
    {
        if(_mode == HAL_BTMODE_A2DP){
            esp_bt_controller_mem_release(ESP_BT_MODE_BLE);
        }
        else if(_mode == HAL_BTMODE_GATTC || _mode == HAL_BTMODE_GATTS){
            esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);
        }
        
        
    }

private:

};


#endif  // __Z_HAL_BLUETOOTH_HPP__


