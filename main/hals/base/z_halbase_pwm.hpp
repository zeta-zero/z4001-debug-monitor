/*--------------------------------------------------------------------
@file            : z_halbase_pwm.hpp
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


#ifndef __Z_HALBASE_PWM_HPP__
#define __Z_HALBASE_PWM_HPP__

#include "cstdint"

class zHalBase_PWM{
public:
    /* _val : 0 - 100 */
    virtual bool SetVal(uint8_t _ch,uint8_t _val) = 0;
    virtual uint8_t GetVal(uint8_t _ch) = 0;
    
    virtual bool SetRawVal(uint8_t _ch,uint16_t _val) = 0;
    virtual uint16_t GetRawVal(uint8_t _ch) = 0;
    virtual uint16_t GetMaxRawVal() = 0;

    virtual bool EnableFade(bool _enable) = 0;
    virtual void SetFadeTime(uint16_t _ms) = 0;

    virtual uint8_t GetChannelNum(void) = 0;
};


#endif // __Z_HALBASE_PWM_HPP__
