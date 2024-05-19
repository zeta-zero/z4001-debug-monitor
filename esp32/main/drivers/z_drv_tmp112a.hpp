/*--------------------------------------------------------------------
@file            : z_drv_tmp112a.cpp
@brief           : 
----------------------------------------------------------------------
@author          : zhangpeiwei02@gmail.com
 Release Version : 0.0.1
 Release Date    : 2024/4/4
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

#ifndef __Z_DRV_TMP112A_HPP__
#define __Z_DRV_TMP112A_HPP__

#include "cstdint"
#include "../hals/base/z_halbase_i2c.hpp"


// DEFINE ---------------------------------------------------------------------


class zDrv_TMP112A{
public:
    zDrv_TMP112A(zHalBase_I2C *_i2c);

    typedef enum{
        CR4000ms = 0x00,
        CR1000ms = 0x01,
        CR250ms = 0x02,
        CR125ms = 0x03,
    }update_frequence_t;
    
    typedef enum{
        CompareModel = 0x00, 
        InterruptModel = 0x01,
    }thermostat_model_t;

    typedef enum{
        Trigger_1 = 0x00,
        Trigger_2 = 0x01,
        Trigger_4 = 0x02,
        Trigger_6 = 0x03,
    }alarm_trigger_count_t;

    uint16_t IICAddr;

    float ReadF(void);
    int ReadI(void);
    uint16_t ReadConfig(void);
    bool SetExpandModel(uint8_t _val);
    bool SetUpdateFreq(update_frequence_t _val);
    bool SetRunOnce(uint8_t _val);
    bool SetThermostat(thermostat_model_t _val);
    bool SetAlarmPolarity(uint8_t _val);
    bool SetAlarmTriggerCount(alarm_trigger_count_t _val);
    bool SetHighLimit(float _val);
    bool SetLowLimit(float _val);
    void RunOnce(void);

private:
    zHalBase_I2C *LocalI2C;
    uint16_t ConfigRegBuf;
    uint16_t OnceModel;


};



#endif // __Z_DRV_TMP112A_HPP__

