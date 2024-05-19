/*--------------------------------------------------------------------
@file            : z_units.hpp
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

#ifndef __Z_UNITS_HPP__
#define __Z_UNITS_HPP__

#include "../drivers/z_driver.hpp"
#include "../drivers/z_drv_rgbled.hpp"

#include "z_unit_led.hpp"

class zUnits{
private:
    zDriver LocalDriver;
public:
    zUnitLED RGBLED;
    const char* SDPath;

    zUnits(void):
        LocalDriver(),
        RGBLED(&LocalDriver.LocalRGBLED)
    {
        SDPath = LocalDriver.LocalSDMMC.Point;
        ESP_LOGI("Units","test");
    }

    void Init(void){
        LocalDriver.Init();
    }

    void Tick(void){
        // ESP_LOGI("Units","Temp : %.2f",LocalDriver.LocalTemp.ReadF());
        zDrv_GT911::point2_list_t points = LocalDriver.LocalTouchCtrl.GetPoints();
        if(points.Num != 0)
        {
            ESP_LOGI("uints","touch num : %d x0: %u y0: %u",points.Num,points.Point2[0].X,points.Point2[0].Y);
        }
        
    }

};
extern zUnits LocalUnits;


#endif // __Z_UNITS_HPP__


