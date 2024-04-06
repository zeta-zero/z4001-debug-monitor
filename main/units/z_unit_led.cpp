/*--------------------------------------------------------------------
@file            : z_unit_led.cpp
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
#include "z_unit_led.hpp"


// DEFINE --------------------------------------------------------------------


// TYPE ----------------------------------------------------------------------




// FUNCTION ------------------------------------------------------------------


// VLAUE ---------------------------------------------------------------------
uint32_t zUnitLED::RainBowColor[] = {0xFF0000,0xFFA500,0xFFFF00,0x00FF00,0x007FFF,0x0000FF,0x8800FF,0xFFFFFF};


/**-------------------------------------------------------------------
 * @fn     : zUnitLED
 * @brief  : 功能初始化函数
 * @param  : none
 * @return : res
 */
zUnitLED::zUnitLED(zDrv_RGBLED *_led) 
{
    LocalTime = 0;
    ColorIndex = 0;
    LEDChgSpeed = 1000;
    Model = Normal;

    LocalLED = _led;
}

/**-------------------------------------------------------------------
 * @fn     : ~zUnitLED
 * @brief  : 析构函数
 * @param  : none
 * @return : res
 */
zUnitLED::~zUnitLED()
{

}

/**-------------------------------------------------------------------
 * @fn     : Tick
 * @brief  : 周期运行函数
 * @param  : none
 * @return : res
 */
void zUnitLED::Tick(uint32_t _ms)
{
    if(LocalLED == nullptr){
        goto end;
    }
    if(LocalTime > TimeBuf){
        TimeBuf = LocalTime + LEDChgSpeed;
        switch(Model){
            case Normal:{
                if(NewColor != (uint32_t)-1){
                    LocalLED->SetRGB(NewColor);
                    NewColor = (uint32_t)-1;
                }
            }break;
            case RainBow:{
                LocalLED->SetRGB(RainBowColor[ColorIndex % 9]);
                ColorIndex++;
            }break;
            default:break;
        }
    }
    LocalTime += _ms;
    
end:
    return;
}

/**-------------------------------------------------------------------
 * @fn     : SetColor
 * @brief  : 设置颜色
 * @param  : none
 * @return : res
 */
void zUnitLED::SetColor(uint32_t _rgb)
{
    Model = Normal;
    NewColor = _rgb & 0xFFFFFF;
}

/**-------------------------------------------------------------------
 * @fn     : SetModel
 * @brief  : 设置运行模型
 * @param  : none
 * @return : res
 */
void zUnitLED::SetModel(led_model_t _model)
{
    if(_model != Model){
        Model = _model;
        LocalTime = 0;
        TimeBuf = 0;
        ColorIndex = 0;
    }
}


/**-------------------------------------------------------------------
 * @fn     : SetRate
 * @brief  : 设置变化速率，单位：100毫秒
 * @param  : _rate   unit:100 ms
 * @return : res
 */
void zUnitLED::SetRate(uint32_t _rate)
{
    if(LocalLED == nullptr){
        goto end;
    }
    if(_rate == 0){_rate = 1;}
    LEDChgSpeed = _rate * 100;
    LocalLED->SetTransitionTime(LEDChgSpeed);

end:
    return;
}

/*--------------------------------------------------------------------
This file was generate by BaseCFileCreateTools.
Version : 0.7 base
--------------------------------------------------------------------*/
