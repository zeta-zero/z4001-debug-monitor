/*--------------------------------------------------------------------
@file            : z_hal_i2c.hpp
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


#ifndef __Z_HAL_I2C_HPP__
#define __Z_HAL_I2C_HPP__

#include "cstdint"
#include "../base/z_halbase_i2c.hpp"
#include "driver/i2c_master.h"

#define ZDRV_I2C_DEV_NUM   8

class zHal_I2C : public zHalBase_I2C{
public:
    zHal_I2C(gpio_num_t _sda,gpio_num_t _scl,i2c_port_t _port = I2C_NUM_0){
        Port = _port;

        i2c_master_bus_config_t i2c_mst_config = {};
        i2c_mst_config.clk_source = I2C_CLK_SRC_DEFAULT;
        i2c_mst_config.i2c_port = Port;
        i2c_mst_config.scl_io_num = _scl;
        i2c_mst_config.sda_io_num = _sda;
        i2c_mst_config.glitch_ignore_cnt = 7;
        i2c_mst_config.flags.enable_internal_pullup = true;

        i2c_new_master_bus(&i2c_mst_config, &BusHandle);
    }

    ~zHal_I2C(void){
        for(int i = 0;i < DevIndex;i++){
            i2c_master_bus_rm_device(DevHandle[i]);
        }
        i2c_del_master_bus(BusHandle);
    }

    /* return : 0 is invalid*/
    uint8_t AddDev(uint16_t _addr,uint32_t _freq = 400000,i2c_addr_bit_len_t _addrlen = I2C_ADDR_BIT_LEN_7){
        uint8_t res = 0;
        i2c_device_config_t dev_cfg = {
            .dev_addr_length = _addrlen,
            .device_address = _addr,
            .scl_speed_hz = _freq,
        };
        if(DevIndex >= ZDRV_I2C_DEV_NUM){
            goto end;
        }
        AddrList[DevIndex] = _addr;
        i2c_master_bus_add_device(BusHandle, &dev_cfg, &DevHandle[DevIndex]);
        DevIndex++;
        res = DevIndex;
    end:
        return res;
    }

    bool Read(uint8_t _addr,uint8_t *_data,uint16_t _len){
        bool res = false;
        esp_err_t ret = ESP_FAIL;
        for(uint8_t index = 0;index<DevIndex;index++){
            if(AddrList[index] == _addr){
                ret = i2c_master_receive(DevHandle[index],_data, _len, 10);
            }
        }
        if(ret == ESP_OK){
            res = true;
        }
        return res;
    }

    bool Write(uint8_t _addr,const uint8_t *_data,uint16_t _len){
        bool res = false;
        esp_err_t ret = ESP_FAIL;
        for(uint8_t index = 0;index<DevIndex;index++){
            if(AddrList[index] == _addr){
                ret = i2c_master_transmit(DevHandle[index],_data, _len, 10);
            }
        }
        if(ret == ESP_OK){
            res = true;
        }
        return res;
    }

    bool WrRd(uint8_t _addr,const uint8_t *_wdata,uint16_t _wlen,uint8_t *_rdata,uint16_t _rlen){
        bool res = false;
        esp_err_t ret = ESP_FAIL;
        for(uint8_t index = 0;index<DevIndex;index++){
            if(AddrList[index] == _addr){
                ret = i2c_master_transmit_receive(DevHandle[index],_wdata, _wlen,_rdata,_rlen, 10);
            }
        }
        if(ret == ESP_OK){
            res = true;
        }
        return res;
    }
private:
    i2c_port_t Port;
    i2c_master_bus_handle_t BusHandle;
    i2c_master_dev_handle_t DevHandle[ZDRV_I2C_DEV_NUM];
    uint16_t AddrList[ZDRV_I2C_DEV_NUM];
    uint8_t DevIndex;

};


#endif // __Z_HAL_I2C_HPP__
