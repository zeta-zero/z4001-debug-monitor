/*--------------------------------------------------------------------
@file            : z_drvlcdc.cpp
@brief           : 
----------------------------------------------------------------------
@author          : zhangpeiwei02@gmail.com
 Release Version : 0.0.1
 Release Date    : 2024/4/6
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


#ifndef __Z_DRV_LCD_HPP__
#define __Z_DRV_LCD_HPP__

#include "cstdint"
#include "../hals/base/z_halbase_i80ctrl.hpp"
#include "../hals/base/z_halbase_pwm.hpp"

class zDrv_LCD{
public:
    void Init(zHalBase_I80Ctrl *_ioctrl,zHalBase_PWM *_pwm,uint16_t _w,uint16_t _h);

    /* _val : 0 -100 */
    void SetLight(uint8_t _val);
    uint16_t GetWidth(void);
    uint16_t GetHeight(void);

    void DrawBitMap(uint16_t _x0,uint16_t _y0,uint16_t _x1,uint16_t _y1,const void* _colors);

private:
    zHalBase_I80Ctrl *LocalI80Ctrl;
    zHalBase_PWM *BackLightPWM;

    uint16_t Width,Height;
};


#endif  //__Z_DRV_LCD_HPP__
