/*--------------------------------------------------------------------
@file            : z_drv_rgbled.hpp
@brief           : 
----------------------------------------------------------------------
@author          : zhangpeiwei02@gmail.com
 Release Version : 0.0.1
 Release Date    : 2024/4/3
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

#include "driver/gpio.h"
#include "driver/ledc.h"

#ifndef __Z_DRV_RGBLED_H__
#define __Z_DRV_RGBLED_H__

#include "cstdint"
#include "../hals/base/z_halbase_pwm.hpp"


// DEFINE --------------------------------------------------------------------


// TYPE ----------------------------------------------------------------------

class zDrv_RGBLED{
public:
    zDrv_RGBLED(zHalBase_PWM *_pwm);
    ~zDrv_RGBLED(void);

    typedef struct{
        float H;
        float S;
        float V;
    }hsv_t;

    void SetRGB(uint32_t _rgb);
    void SetHSV(uint32_t _hsv);
    /* _val : 0 - 100*/
    void SetBrightness(uint8_t _val);
    void SetTransitionTime(uint32_t _val);

private:
    zHalBase_PWM *LocalPWM;
    uint32_t FadeTimeMS;

    hsv_t RGB2HSV(uint32_t);
    uint32_t HSV2RGB(hsv_t _val);
};


// VLAUE ---------------------------------------------------------------------


// FUNCTION ------------------------------------------------------------------



#endif // __Z_DRV_LED_H__
/*--------------------------------------------------------------------
This file was generate by BaseCFileCreateTools.
Version : 0.7 base
--------------------------------------------------------------------*/
