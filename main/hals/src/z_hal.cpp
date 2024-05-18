/*--------------------------------------------------------------------
@file            : z_hal.cpp
@brief           : 
----------------------------------------------------------------------
@author          : zhangpeiwei02@gmail.com
 Release Version : 0.0.1
 Release Date    : 2024/05/18
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

#include "z_hal_pwm.hpp"
#include "z_hal_wifi.hpp"

// VLAUE ---------------------------------------------------------------------
uint8_t zHal_PWM::ChannelID = 0;
uint8_t zHal_PWM::LEDCTimerList = 0;
uint8_t zHal_PWM::FadeEnableCount = 0;
uint16_t zHal_PWM::ChannelRawData[ZHALPWM_CHANNEL_MAX] = {0};
ledc_channel_config_t zHal_PWM::Channel[ZHALPWM_CHANNEL_MAX];


bool zHal_WiFi::IsConnected = false;




