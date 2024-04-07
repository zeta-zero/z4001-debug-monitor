/*--------------------------------------------------------------------
@file            : z_driver.hpp
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

#ifndef __Z_DERIVER_HPP__
#define __Z_DERIVER_HPP__

#include "cstdint"
#include "driver/gpio.h"
#include "../hals/src/z_hal_i2c.hpp"
#include "../hals/src/z_hal_pwm.hpp"
#include "../hals/src/z_hal_gpio.hpp"
#include "../hals/src/z_hal_i80ctrl.hpp"
#include "../hals/src/z_hal_sdmmc.hpp"
#include "z_drv_rgbled.hpp"
#include "z_drv_lsm6d.hpp"
#include "z_drv_tmp112a.hpp"
#include "z_drv_gt911.hpp"
#include "z_drv_lcd.hpp"

gpio_num_t rgbio[3] = {GPIO_NUM_8,GPIO_NUM_18,GPIO_NUM_17};
gpio_num_t lcdgpio = GPIO_NUM_45;

uint16_t RGBBuf[240*320] = {0};

class zDriver{
private:
    zHal_I2C LocalI2C;
    zHal_PWM LEDPWM;
    zHal_PWM LCDPWM;
    zHal_GPIO GT911RST;
    zHal_GPIO GT911INTR;
    zHal_I80Ctrl ST7789;
    zHal_SDMMC LocalSDMMC;
public:
    zDrv_RGBLED LocalRGBLED;
    zDrv_LSM6D LocalGyro6D;
    zDrv_TMP112A LocalTemp;
    zDrv_GT911 LocalTouchCtrl;
    zDrv_LCD LocalLCD;
    zDriver(void) : LocalI2C(GPIO_NUM_35,GPIO_NUM_36),
                    LEDPWM(rgbio,3),
                    LCDPWM(&lcdgpio,1),
                    GT911RST(GPIO_NUM_37),
                    GT911INTR(GPIO_NUM_38,GPIO_MODE_INPUT),
                    LocalRGBLED(&LEDPWM),
                    LocalGyro6D(&LocalI2C,GPIO_NUM_42,GPIO_NUM_41),
                    LocalTemp(&LocalI2C)
    {
    };

    void Init(void){

        LocalSDMMC.Init(GPIO_NUM_7,GPIO_NUM_6,GPIO_NUM_2,GPIO_NUM_15,GPIO_NUM_16,GPIO_NUM_4,GPIO_NUM_5);
        LocalTouchCtrl.Init(&LocalI2C,&GT911RST,&GT911INTR);

        gpio_num_t lcdios[24];
        lcdios[0] = GPIO_NUM_10;
        lcdios[1] = GPIO_NUM_11;
        lcdios[2] = GPIO_NUM_12;
        lcdios[3] = GPIO_NUM_13;
        lcdios[4] = GPIO_NUM_14;
        lcdios[5] = GPIO_NUM_21;
        lcdios[6] = GPIO_NUM_47;
        lcdios[7] = GPIO_NUM_48;
        ST7789.Init(lcdios,8,GPIO_NUM_46,GPIO_NUM_9,GPIO_NUM_3,GPIO_NUM_NC,zHal_I80Ctrl::ST7789);
        LocalLCD.Init(&ST7789,&LCDPWM,240,320);

        LocalI2C.AddDev(LocalGyro6D.IICAddr);
        LocalI2C.AddDev(LocalTemp.IICAddr);
        LocalI2C.AddDev(LocalTouchCtrl.IICAddr0);
        LocalI2C.AddDev(LocalTouchCtrl.IICAddr1);

        LocalTouchCtrl.Start();
        // LocalGyro6D.Start();
        LocalTemp.SetUpdateFreq(zDrv_TMP112A::CR250ms);
        LocalLCD.SetLight(100);
        for(int i = 0;i<240*320;i++){
            RGBBuf[i] = 0xFF00;
        }
        for(int i = 0;i<5;i++)
        {
            LocalLCD.DrawBitMap(100,100,150,150,(const void*)RGBBuf);
            vTaskDelay(100);
        }
        
    }

};


#endif // __Z_DERIVER_HPP__


