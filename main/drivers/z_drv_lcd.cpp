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

#include "z_drv_lcd.hpp"
#include "assert.h"

/**-------------------------------------------------------------------
 * @fn     : Init
 * @brief  : 
 * @param  : none
 * @return : res
 */
void zDrv_LCD::Init(zHalBase_I80Ctrl *_ioctrl,zHalBase_PWM *_pwm,uint16_t _w,uint16_t _h)
{
    assert(_ioctrl != nullptr && _pwm != nullptr);
    
    LocalI80Ctrl = _ioctrl;
    BackLightPWM = _pwm;

    Width = _w;
    Height = _h;

    LocalI80Ctrl->DisplayOn(true);
}

/**-------------------------------------------------------------------
 * @fn     : SetLight
 * @brief  : 
 * @param  : none
 * @return : res
 */
void zDrv_LCD::SetLight(uint8_t _val)
{
    BackLightPWM->SetVal(0,_val);
}

/**-------------------------------------------------------------------
 * @fn     : GetWidth
 * @brief  : 
 * @param  : none
 * @return : res
 */
uint16_t zDrv_LCD::GetWidth(void)
{
    return Width;
}

/**-------------------------------------------------------------------
 * @fn     : GetHeight
 * @brief  : 
 * @param  : none
 * @return : res
 */
uint16_t zDrv_LCD::GetHeight(void)
{
    return Height;
}

/**-------------------------------------------------------------------
 * @fn     : DrawBitMap
 * @brief  : 
 * @param  : none
 * @return : res
 */
void zDrv_LCD::DrawBitMap(uint16_t _x0,uint16_t _y0,uint16_t _x1,uint16_t _y1,const void* _colors)
{
    if(_x0 > Width || _x1 > Width){
        return;
    }
    if(_x1 > Width){_x1 = Width;}
    if(_y1 > Height){_y1 = Height;}
    LocalI80Ctrl->DrawBitmap(_x0,_y0,_x1,_y1,_colors);
}