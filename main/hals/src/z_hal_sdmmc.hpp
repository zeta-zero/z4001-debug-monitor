/*--------------------------------------------------------------------
@file            : z_hal_sdmmc.hpp
@brief           : 
----------------------------------------------------------------------
@author          : zhangpeiwei02@gmail.com
 Release Version : 0.0.1
 Release Date    : 2024/4/6
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


#ifndef __Z_HAL_SDMMC_HPP__
#define __Z_HAL_SDMMC_HPP__

#include "cstdint"

#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"

// DEFINE ------------------------------------------------------------
#define TAG "hal_sdmmc"

class zHal_SDMMC{
public:
    const char Point[4] = "/sd";

    void Init(gpio_num_t _clk,gpio_num_t _cmd,gpio_num_t _cd,gpio_num_t _d0,gpio_num_t _d1,gpio_num_t _d2,gpio_num_t _d3)
    {    
        GPIOList[0] = _clk;
        GPIOList[1] = _cmd;
        GPIOList[2] = _cd;
        GPIOList[3] = _d0;
        GPIOList[4] = _d1;
        GPIOList[5] = _d2;
        GPIOList[6] = _d3;

        gpio_config_t io_cfg = {
            .pin_bit_mask = BIT64(_cd),
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE,
        };
        gpio_config(&io_cfg);
        __Init();
    }

    void DeInit(void)
    {
        
        esp_vfs_fat_sdcard_unmount(Point,Card);
    }

    void CheckAndReInstall(void)
    {
        if(IsExist == false && gpio_get_level(GPIOList[2]) == 1){
            __Init();
        }
        else if(IsExist == false && gpio_get_level(GPIOList[2]) == 0){
            DeInit();
        }
    }

    bool GetExist(void)
    {
        return IsExist;
    }


private:
    sdmmc_card_t *Card;
    gpio_num_t GPIOList[7];
    bool IsExist;

    void __Init(void)
    {
        sdmmc_host_t host = SDMMC_HOST_DEFAULT();
        host.max_freq_khz = SDMMC_FREQ_52M;

        sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
        slot_config.width = 4;
        slot_config.clk = GPIOList[0];
        slot_config.cmd = GPIOList[1];
        slot_config.d0 = GPIOList[3];
        slot_config.d1 = GPIOList[4];
        slot_config.d2 = GPIOList[5];
        slot_config.d3 = GPIOList[6];
        slot_config.flags |= SDMMC_SLOT_FLAG_INTERNAL_PULLUP;

        esp_vfs_fat_sdmmc_mount_config_t mount_config = {
            .format_if_mount_failed = false,
            .max_files = 5,
            .allocation_unit_size = 16 * 1024,
            .disk_status_check_enable = true,
        };
        
        esp_err_t ret = esp_vfs_fat_sdmmc_mount(Point, &host, &slot_config, &mount_config, &Card);

        if (ret != ESP_OK) {
            if (ret == ESP_FAIL) {
                ESP_LOGE(TAG, "Failed to mount filesystem. "
                        "If you want the card to be formatted, set the EXAMPLE_FORMAT_IF_MOUNT_FAILED menuconfig option.");
            } else {
                ESP_LOGE(TAG, "Failed to initialize the card (%s). "
                        "Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(ret));
            }
            IsExist = false;
        }
        else{
            IsExist = true;
        }
    }
};


#endif // __Z_HAL_SDMMC_HPP__
