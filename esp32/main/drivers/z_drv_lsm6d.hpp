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



#ifndef __Z_DRV_LSM6D_HPP__
#define __Z_DRV_LSM6D_HPP__

#include "../hals/base/z_halbase_i2c.hpp"
#include "driver/gpio.h"
#include "cstring"
#include "esp_log.h"
#include "third/stm32-lsm6dso-main/lsm6dso.h"

class zDrv_LSM6D{
public:
    zDrv_LSM6D(zHalBase_I2C *_i2c,gpio_num_t _int1,gpio_num_t _int2);
    ~zDrv_LSM6D();

    typedef struct {
        int32_t x,y,z;
    }point3_t;

    uint16_t IICAddr;

    bool Check(void);
    bool Start(void);
    bool Stop(void);
    point3_t GetAngularRate(void);
    point3_t GetAcceleration(void);
    point3_t GetOrientation(void);
private:
    LSM6DSO_IO_t IOConfig;
    LSM6DSO_Object_t LocalObject;
    class BaseAPI{
        private:
        public:
            static zHalBase_I2C *LocalI2C;
            BaseAPI(zHalBase_I2C *_i2c){
                LocalI2C = _i2c;
            };
            static int32_t Init(void){
                return LSM6DSO_OK;
            }
            static int32_t DeInit(void){
                return LSM6DSO_OK;
            }
            static int32_t GetTick(void){
                return esp_log_timestamp();
            }
            static int32_t WriteReg(uint16_t _addr, uint16_t _reg, uint8_t *_data, uint16_t _len){
                if(LocalI2C == nullptr){
                    return LSM6DSO_ERROR;
                }
                uint8_t buf[32] = {0};
                buf[0] = (uint8_t)(_reg & 0xFF);
                memcpy(&buf[1],_data,_len);
                LocalI2C->Write(_addr,buf,1+_len);

                return LSM6DSO_OK;
            }
            static int32_t ReadReg(uint16_t _addr, uint16_t _reg, uint8_t *_data, uint16_t _len){
                if(LocalI2C == nullptr){
                    return LSM6DSO_ERROR;
                }
                uint8_t buf = (uint8_t)(_reg & 0xFF);
                LocalI2C->WrRd(_addr,&buf,1,_data,_len);
                
                return LSM6DSO_OK;
            }

    };
    static BaseAPI LocalAPI;

};


#endif  // __Z_DRV_LSM6D_HPP__
