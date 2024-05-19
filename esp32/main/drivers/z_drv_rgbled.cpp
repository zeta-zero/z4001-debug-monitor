/*--------------------------------------------------------------------
@file            : z_drv_rgbled.cpp
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
#include "z_drv_rgbled.hpp"
#include "cmath"

// DEFINE --------------------------------------------------------------------
#define _MAX(_A,_B) (_A)>(_B)?(_A):(_B)
#define _MIN(_A,_B) (_A)<(_B)?(_A):(_B)

// TYPE ----------------------------------------------------------------------




// FUNCTION ------------------------------------------------------------------


// VLAUE ---------------------------------------------------------------------


/**-------------------------------------------------------------------
 * @fn     : zDrv_RGBLED
 * @brief  : 功能初始化函数
 * @param  : none
 * @return : res
 */
zDrv_RGBLED::zDrv_RGBLED(zHalBase_PWM *_pwm)
{
    assert(_pwm != nullptr || _pwm->GetChannelNum() == 3);
    LocalPWM = _pwm;
}

/**-------------------------------------------------------------------
 * @fn     : ~zDrv_RGBLED
 * @brief  : 析构函数
 * @param  : none
 * @return : res
 */
zDrv_RGBLED::~zDrv_RGBLED(void)
{

}

/**-------------------------------------------------------------------
 * @fn     : SetRGB
 * @brief  : 设置颜色
 * @param  : none
 * @return : res
 */
void zDrv_RGBLED::SetRGB(uint32_t _rgb)
{
    uint16_t val = 0;
    val = ((_rgb >> 16) & 0xFF) * LocalPWM->GetMaxRawVal() / 0xFF;
    LocalPWM->SetRawVal(0,val);
    val = ((_rgb >> 8) & 0xFF) * LocalPWM->GetMaxRawVal() / 0xFF;
    LocalPWM->SetRawVal(1,val);
    val = ((_rgb >> 0) & 0xFF) * LocalPWM->GetMaxRawVal() / 0xFF;
    LocalPWM->SetRawVal(2,val);
}

/**-------------------------------------------------------------------
 * @fn     : SetVal
 * @brief  : 设置数值
 * @param  : none
 * @return : res
 */
void zDrv_RGBLED::SetBrightness(uint8_t _val)
{

}

/**-------------------------------------------------------------------
 * @fn     : SetVal
 * @brief  : 设置数值
 * @param  : none
 * @return : res
 */
void zDrv_RGBLED::SetTransitionTime(uint32_t _val)
{
    FadeTimeMS = _val;
}

/**-------------------------------------------------------------------
 * @fn     : RGB2HSV
 * @brief  : 
 * @param  : none
 * @return : res
 */
zDrv_RGBLED::hsv_t zDrv_RGBLED::RGB2HSV(uint32_t _rgb)
{
    float r = ((_rgb >> 16) & 0xFF) / 255.0f,
          g = ((_rgb >> 8) & 0xFF) / 255.0f,
          b = (_rgb & 0xFF) / 255.0f;
    uint8_t max_rgb = _MAX(r,_MAX(g,b));
    uint8_t min_rgb = _MIN(r,_MIN(g,b));
    hsv_t mhsv;
    float d = max_rgb - min_rgb;
    mhsv.V = max_rgb;
    mhsv.S = max_rgb == 0.0f? 0.0f : d / max_rgb;
    if(max_rgb == min_rgb){
        mhsv.H = 0.0f;
    }
    else{
        if(max_rgb == r){
            mhsv.H = (g - b) / d + (g < b?6:0);
        }
        else if(max_rgb == g){
            mhsv.H = (b - r) / d + 2;
        }
        else if(max_rgb == b){
            mhsv.H = (r - g) / d + 4;
        }
        mhsv.H /= 6;
    }

    return mhsv;
}

/**-------------------------------------------------------------------
 * @fn     : HSV2RGB
 * @brief  : 
 * @param  : none
 * @return : res
 */
uint32_t zDrv_RGBLED::HSV2RGB(hsv_t _val)
{
    uint32_t res = 0;
    uint8_t i = (uint8_t)(_val.H * 6);
    float f = _val.H * 6 - i;
    float p = _val.V * (i - _val.S);
    float q = _val.V * (i - f * _val.S);
    float t =  _val.V * (1 - (1 - f) * _val.S);

    switch(i % 6){
        case 0:{
            res = ((uint8_t)(_val.V * 255) << 16) + ((uint8_t)(t * 255) << 8) + ((uint8_t)(p * 255) << 0);
        }break;
        case 1:{
            res = ((uint8_t)(q * 255) << 16) + ((uint8_t)(_val.V * 255) << 8) + ((uint8_t)(p * 255) << 0);
        }break;
        case 2:{
            res = ((uint8_t)(p * 255) << 16) + ((uint8_t)(_val.V * 255) << 8) + ((uint8_t)(t * 255) << 0);
        }break;
        case 3:{
            res = ((uint8_t)(p * 255) << 16) + ((uint8_t)(q * 255) << 8) + ((uint8_t)(_val.V * 255) << 0);
        }break;
        case 4:{
            res = ((uint8_t)(t * 255) << 16) + ((uint8_t)(p * 255) << 8) + ((uint8_t)(_val.V * 255) << 0);
        }break;
        case 5:{
            res = ((uint8_t)(_val.V * 255) << 16) + ((uint8_t)(p * 255) << 8) + ((uint8_t)(q * 255) << 0);
        }break;
    }

    return res;
}

/*--------------------------------------------------------------------
This file was generate by BaseCFileCreateTools.
Version : 0.7 base
--------------------------------------------------------------------*/
