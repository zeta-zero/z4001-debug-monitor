/*--------------------------------------------------------------------
@file            : z_drvbase_led.hpp
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


#ifndef __Z_DRVBASE_LED_HPP__
#define __Z_DRVBASE_LED_HPP__

#include "cstdint"

class zHalBase_LED{
public:
    /* param : _val - 0 - 255 */
    virtual void SetVal(uint8_t _val) = 0;
    /* param : _rgb - 0 - 0xFFFFFF */
    virtual void SetRGB(uint32_t _rgb) = 0;
    /* param : _val - unit ms */
    virtual void SetTransitionTime(uint32_t _val) = 0;
};


#endif // __Z_DRVBASE_LED_HPP__
