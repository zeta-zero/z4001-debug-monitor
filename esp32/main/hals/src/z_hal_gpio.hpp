/*--------------------------------------------------------------------
@file            : z_hal_gpio.hpp
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


#ifndef __Z_HAL_GPIO_HPP__
#define __Z_HAL_GPIO_HPP__

#include "cstdint"
#include "driver/gpio.h"
#include "../base/z_halbase_gpio.hpp"


class zHal_GPIO : public zHalBase_GPIO{
public:
    zHal_GPIO(gpio_num_t _io,gpio_mode_t _mode = GPIO_MODE_OUTPUT,gpio_int_type_t _intr = GPIO_INTR_DISABLE)
    {
        Enable = false;
        IONum = _io;
        gpio_config_t cfg = {
            .pin_bit_mask = BIT64(IONum),
            .mode = _mode,
            //for powersave reasons, the GPIO should not be floating, select pullup
            .pull_up_en = GPIO_PULLUP_ENABLE,
            .pull_down_en = GPIO_PULLDOWN_ENABLE,
            .intr_type = _intr,
        };
        if(_io != GPIO_NUM_NC){
            assert(gpio_config(&cfg) == ESP_OK);
            Enable = true;
        }

    }
    /* _hl : 0 or 1*/
    void SetLevel(uint32_t _hl)
    {
        if(Enable == false){
            return;
        }
        gpio_set_level(IONum,_hl);
    }

    uint8_t GetLevel(void)
    {
        if(Enable == false){
            return 0xFF;
        }
        return gpio_get_level(IONum) == 0?0:1;
    }

    /* 0 : out 1 : in*/
    void SetInOutModel(uint8_t _inout)
    {
        if(Enable == false){
            return;
        }
        if(_inout == 0){
            gpio_set_direction(IONum,GPIO_MODE_OUTPUT);
        }
        else{
            gpio_set_direction(IONum,GPIO_MODE_INPUT);
        }
        
    }

private:
    gpio_num_t IONum;
    bool Enable;

};


#endif  // __Z_HAL_GPIO_HPP__


