/*--------------------------------------------------------------------
@file            : z_drv_gt911.hpp
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

#ifndef __Z_DRV_GT911_HPP__
#define __Z_DRV_GT911_HPP__

#include "cstdint"
#include "driver/gpio.h"
#include "../hals/base/z_halbase_i2c.hpp"
#include "../hals/base/z_halbase_gpio.hpp"

//DEFINE ------------------------------------------------------------
#define GT911_MAX_POINT   5

class zDrv_GT911{
public:
    zDrv_GT911(void){};
    zDrv_GT911(zHalBase_I2C *_i2c,zHalBase_GPIO *_rst,zHalBase_GPIO *_intr = nullptr);

    typedef struct{
        uint16_t X;
        uint16_t Y;
    }point2_t;
    typedef struct{
        point2_t Point2[GT911_MAX_POINT];
        uint8_t Num;
    }point2_list_t;

    uint16_t IICAddr0;
    uint16_t IICAddr1;
    uint16_t IICAddr;
    static const uint8_t DefaultConfigTable[184];

    void Init(zHalBase_I2C *_i2c,zHalBase_GPIO *_rst,zHalBase_GPIO *_intr = nullptr);

    void Tick(uint32_t _ms);
    point2_list_t GetPoints(void);

    bool Check(void);
    void GetProductID(char _id[5]);
    uint8_t GetVendorID(void);
    void ResetKeep(void);
    void ResetRelease(void);
    void Start(void);

    void UpdateConfig(const uint8_t _buf[184]);
    

private:
    zHalBase_I2C *LocalI2C;
    zHalBase_GPIO *LocalRST;
    zHalBase_GPIO *LocalIntr;
    int CheckCount;

    point2_list_t PointList[GT911_MAX_POINT];


    uint8_t GetStatus(void);
    void CleanStatus(void);
    void SetModel(uint8_t _command);

    point2_list_t ReadPoints(uint8_t _num);

};



#endif

