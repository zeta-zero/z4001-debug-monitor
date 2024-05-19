/*--------------------------------------------------------------------
@file            : z_drv_lsm6d.hpp
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

#include "z_drv_lsm6d.hpp"

// DEFINE ---------------------------------------------------------------------
#define I2C_Addr    0x6A
#define DEVICE_ID   0x6B


// VLAUE ---------------------------------------------------------------------
zHalBase_I2C* zDrv_LSM6D::BaseAPI::LocalI2C = nullptr;

/**-------------------------------------------------------------------
 * @fn     : zDrv_LSM6D
 * @brief  : 
 * @param  : none
 * @return : res
 */
zDrv_LSM6D::zDrv_LSM6D(zHalBase_I2C *_i2c,gpio_num_t _int1,gpio_num_t _int2)
{
    IICAddr = I2C_Addr;
    LocalAPI.LocalI2C = _i2c;
    IOConfig.Init = LocalAPI.Init;
    IOConfig.DeInit = LocalAPI.DeInit;
    IOConfig.BusType = 0;
    IOConfig.Address = I2C_Addr;
    IOConfig.WriteReg = LocalAPI.WriteReg;
    IOConfig.ReadReg = LocalAPI.ReadReg;
    IOConfig.GetTick = LocalAPI.GetTick;

    LSM6DSO_RegisterBusIO(&LocalObject,&IOConfig);
    LSM6DSO_Init(&LocalObject);
}

/**-------------------------------------------------------------------
 * @fn     : ~zDrv_LSM6D
 * @brief  : 
 * @param  : none
 * @return : res
 */
zDrv_LSM6D::~zDrv_LSM6D(void)
{
    LSM6DSO_DeInit(&LocalObject);

}

/**-------------------------------------------------------------------
 * @fn     : Check
 * @brief  : 
 * @param  : none
 * @return : res
 */
bool zDrv_LSM6D::Check(void)
{
    uint8_t id = 0;
    LSM6DSO_ReadID(&LocalObject,&id);
    return id == DEVICE_ID;
}

/**-------------------------------------------------------------------
 * @fn     : Start
 * @brief  : 
 * @param  : none
 * @return : res
 */
bool zDrv_LSM6D::Start(void)
{
    bool res = false;
    // if(LSM6DSO_ACC_Enable_6D_Orientation(&LocalObject,LSM6DSO_INT1_PIN) != LSM6DSO_OK){
    //     goto end;
    // }
    if(LSM6DSO_ACC_Enable(&LocalObject) != LSM6DSO_OK){
        goto end;
    }
    if(LSM6DSO_GYRO_Enable(&LocalObject) != LSM6DSO_OK){
        goto end;
    }

    res = true;
end:
    return res;
}

/**-------------------------------------------------------------------
 * @fn     : Stop
 * @brief  : 
 * @param  : none
 * @return : res
 */
bool zDrv_LSM6D::Stop(void)
{
    bool res = false;
    // if(LSM6DSO_ACC_Disable_6D_Orientation(&LocalObject) != LSM6DSO_OK){
    //     goto end;
    // }
    if(LSM6DSO_ACC_Disable(&LocalObject) != LSM6DSO_OK){
        goto end;
    }
    if(LSM6DSO_GYRO_Disable(&LocalObject) != LSM6DSO_OK){
        goto end;
    }

    res = true;
end:
    return res;
}


/**-------------------------------------------------------------------
 * @fn     : GetAngularRate
 * @brief  : 
 * @param  : none
 * @return : res
 */
zDrv_LSM6D::point3_t zDrv_LSM6D::GetAngularRate(void)
{
    point3_t res={};
    LSM6DSO_Axes_t axes={};
    if(LSM6DSO_GYRO_GetAxes(&LocalObject,&axes) == LSM6DSO_OK){
        res.x = axes.x;
        res.y = axes.y;
        res.z = axes.z;
    }

    return res;
}

/**-------------------------------------------------------------------
 * @fn     : GetAcceleration
 * @brief  : 
 * @param  : none
 * @return : res
 */
zDrv_LSM6D::point3_t zDrv_LSM6D::GetAcceleration(void)
{
    point3_t res={};
    LSM6DSO_Axes_t axes={};
    if(LSM6DSO_ACC_GetAxes(&LocalObject,&axes) == LSM6DSO_OK){
        res.x = axes.x;
        res.y = axes.y;
        res.z = axes.z;
    }

    return res;
}

/**-------------------------------------------------------------------
 * @fn     : GetOrientation
 * @brief  : 
 * @param  : none
 * @return : res
 */
zDrv_LSM6D::point3_t zDrv_LSM6D::GetOrientation(void)
{
    point3_t res={};
    uint8_t val = 0;
    LSM6DSO_ACC_Get_6D_Orientation_XL(&LocalObject,&val);
    res.x = val;
    LSM6DSO_ACC_Get_6D_Orientation_XH(&LocalObject,&val);
    res.x += val * 0x100;
    LSM6DSO_ACC_Get_6D_Orientation_YL(&LocalObject,&val);
    res.y = val;
    LSM6DSO_ACC_Get_6D_Orientation_YH(&LocalObject,&val);
    res.y += val * 0x100;
    LSM6DSO_ACC_Get_6D_Orientation_ZL(&LocalObject,&val);
    res.z = val;
    LSM6DSO_ACC_Get_6D_Orientation_ZH(&LocalObject,&val);
    res.z += val * 0x100;

    return res;
}


