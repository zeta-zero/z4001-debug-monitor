/*--------------------------------------------------------------------
@file            : z_hal_pwm.hpp
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


#ifndef __Z_HAL_PWM_HPP__
#define __Z_HAL_PWM_HPP__

#include "cstdint"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "../base/z_halbase_pwm.hpp"

// DEFINE --------------------------------------------------------------------
#define ZHALPWM_CHANNEL_MAX  LEDC_CHANNEL_MAX

// CLASS ---------------------------------------------------------------------
class zHal_PWM : public zHalBase_PWM{
public:
    zHal_PWM(gpio_num_t *_ios,uint8_t _ionum,uint32_t _freq = 10000,ledc_timer_t _timerid = LEDC_TIMER_0)
    {
        assert(_ionum != 0 || _ios != nullptr);

        ChannelIDBase = ChannelID;
        if(ChannelIDBase + _ionum >= ZHALPWM_CHANNEL_MAX){
            ChannelNum = ZHALPWM_CHANNEL_MAX - ChannelIDBase;
        }
        else{
            ChannelNum = _ionum;
        }
        
        FadeTime = 1000;
        FadeEnable = false;

        if((LEDCTimerList & (1 << (int)LEDC_TIMER_0)) == 0){
            LEDCTimerList |= (uint8_t)(1 << (int)LEDC_TIMER_0);

            ledc_timer_config_t ledc_timer = {};
            ledc_timer.duty_resolution = LEDC_TIMER_14_BIT;
            ledc_timer.speed_mode = LEDC_LOW_SPEED_MODE;     // timer mode
            ledc_timer.timer_num = _timerid;                 // timer index
            ledc_timer.freq_hz = _freq;                      // frequency of PWM signal
            ledc_timer.clk_cfg = LEDC_AUTO_CLK;               // Auto select the source clock
            for(int i = 0;i<(int)LEDC_TIMER_BIT_MAX;i++){
                if(_freq < (1 << (12 + i))){
                    ledc_timer.duty_resolution = (ledc_timer_bit_t)((int)LEDC_TIMER_14_BIT - i);
                    MaxCount = (1 << ledc_timer.duty_resolution);
                    break;
                }
            }
            ledc_timer_config(&ledc_timer);
        }
        else{
            uint32_t mfreq = ledc_get_freq(LEDC_LOW_SPEED_MODE,_timerid);
            for(int i = 0;i<(int)LEDC_TIMER_BIT_MAX;i++){
                if(mfreq < (1 << (12 + i))){
                    MaxCount = (1 << (ledc_timer_bit_t)((int)LEDC_TIMER_14_BIT - i));
                    break;
                }
            }
        }

        for(int index = 0;index < ChannelNum;index++){
            Channel[ChannelIDBase + index].gpio_num = _ios[index];
            Channel[ChannelIDBase + index].speed_mode = LEDC_LOW_SPEED_MODE;
            Channel[ChannelIDBase + index].channel = (ledc_channel_t)(ChannelIDBase + index);
            Channel[ChannelIDBase + index].intr_type = LEDC_INTR_DISABLE;
            Channel[ChannelIDBase + index].timer_sel = _timerid;
            Channel[ChannelIDBase + index].duty = 0;
            Channel[ChannelIDBase + index].hpoint = 0;
            Channel[ChannelIDBase + index].flags.output_invert = 0;
            ledc_channel_config(&Channel[ChannelIDBase + index]);
            ChannelRawData[ChannelIDBase + index] = 0;
            ChannelID++;
        }

        EnableFade(true);
    }
    /* _val : 0 - 100 */
    bool SetVal(uint8_t _ch,uint8_t _val) 
    {
        bool res = false;
        ledc_channel_t ch = (ledc_channel_t)(ChannelIDBase + _ch);
        if(ch + 1 > ZHALPWM_CHANNEL_MAX){
            goto end;
        }
        if(_val > 100){_val = 100;}
        ChannelRawData[ch]  = _val * MaxCount / 100;
        if(FadeEnable == true){
            ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE,ch,ChannelRawData[ch],FadeTime);
            ledc_fade_start(LEDC_LOW_SPEED_MODE,ch,LEDC_FADE_NO_WAIT);
        }
        else{
            ledc_set_duty(LEDC_LOW_SPEED_MODE,ch,ChannelRawData[ch]);
            ledc_update_duty(LEDC_LOW_SPEED_MODE,ch);
        }
    end:
        return res;
    }
    /* return : 0 - 100 */
    uint8_t GetVal(uint8_t _ch)
    {
        uint8_t res = 0xFF;
        ledc_channel_t ch = (ledc_channel_t)(ChannelIDBase + _ch);
        if(ch + 1 > ZHALPWM_CHANNEL_MAX){
            goto end;
        }
        res = ChannelRawData[ch] * 100 / MaxCount;
    end:
        return res;

    }
    
    bool SetRawVal(uint8_t _ch,uint16_t _val)
    {
        bool res = false;
        ledc_channel_t ch = (ledc_channel_t)(ChannelIDBase + _ch);
        if(_ch + 1 > ZHALPWM_CHANNEL_MAX){
            goto end;
        }
        if(_val > MaxCount){_val = MaxCount;}
        ChannelRawData[ch]  = _val;
        if(FadeEnable == true){
            ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE,ch,ChannelRawData[ch],FadeTime);
            ledc_fade_start(LEDC_LOW_SPEED_MODE,ch,LEDC_FADE_NO_WAIT);
        }
        else{
            ledc_set_duty(LEDC_LOW_SPEED_MODE,ch,ChannelRawData[ch]);
            ledc_update_duty(LEDC_LOW_SPEED_MODE,ch);
        }
    end:
        return res;
    }

    virtual uint16_t GetRawVal(uint8_t _ch)
    {
        uint16_t res = 0xFFFF;
        ledc_channel_t ch = (ledc_channel_t)(ChannelIDBase + _ch);
        if(ch + 1 > ZHALPWM_CHANNEL_MAX){
            goto end;
        }
        res = ChannelRawData[ch];
    end:
        return res;
    }

    uint16_t GetMaxRawVal()
    {
        return MaxCount;
    }

    
    bool EnableFade(bool _enable){
        bool res = false;
        if(FadeEnable == _enable){
            res = true;
            goto end;
        }
        if(_enable == true){
            FadeEnableCount++;
        }
        else{
            FadeEnableCount--;
        }
        if(FadeEnableCount == 1){
            // Initialize fade service.
            ledc_fade_func_install(0);
        }
        else if(FadeEnableCount == 0){
            ledc_fade_func_uninstall();
        }
        FadeEnable = _enable;
    end:
        return res;
    }

    void SetFadeTime(uint16_t _ms)
    {
        FadeTime = _ms;
    }

    uint8_t GetChannelNum(void)
    {
        return ChannelNum;
    }

private:
    static uint8_t ChannelID;
    static ledc_channel_config_t Channel[ZHALPWM_CHANNEL_MAX];
    static uint16_t ChannelRawData[ZHALPWM_CHANNEL_MAX];
    static uint8_t LEDCTimerList;
    static uint8_t FadeEnableCount;
    bool FadeEnable;
    uint8_t ChannelIDBase;
    uint8_t ChannelNum;
    uint16_t MaxCount;
    uint16_t FadeTime;

};



#endif // __Z_HAL_PWM_HPP__
