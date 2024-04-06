/*--------------------------------------------------------------------
@file            : z_unit_led.hpp
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

#ifndef __Z_UNIT_LED_H__
#define __Z_UNIT_LED_H__

#include "cstdint"
#include "../drivers/z_drv_rgbled.hpp"


// DEFINE --------------------------------------------------------------------

class zUnitLED{
public:    
// TYPE ----------------------------------------------------------------------
    typedef enum{
        Normal = 0,
        RainBow = 1,
    }led_model_t;

    zUnitLED(zDrv_RGBLED *_led);
    ~zUnitLED();

    void Tick(uint32_t _ms);
    void SetColor(uint32_t _rgb);
    void SetModel(led_model_t _model);
    void SetRate(uint32_t _rate);

private:
    zDrv_RGBLED *LocalLED;
    led_model_t Model;
    uint32_t ColorIndex;
    static uint32_t RainBowColor[];

    uint32_t LocalTime;
    uint32_t TimeBuf;
    uint32_t LEDChgSpeed;
    uint32_t NewColor;
};


// VLAUE ---------------------------------------------------------------------


// FUNCTION ------------------------------------------------------------------



#endif // __Z_DRV_LED_H__
/*--------------------------------------------------------------------
This file was generate by BaseCFileCreateTools.
Version : 0.7 base
--------------------------------------------------------------------*/
