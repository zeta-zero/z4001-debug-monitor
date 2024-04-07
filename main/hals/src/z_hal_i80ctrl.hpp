/*--------------------------------------------------------------------
@file            : z_hal_i80ctrl.hpp
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


#ifndef __Z_HAL_I80CTRL_HPP__
#define __Z_HAL_I80CTRL_HPP__

#include "cstdint"
#include "driver/gpio.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "../base/z_halbase_i80ctrl.hpp"


static bool example_notify_lvgl_flush_ready(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx)
{
    return false;
}

class zHal_I80Ctrl: public zHalBase_I80Ctrl{
public:
    typedef enum{
        ST7789 = 0x00,
        NT35510,
        ILI9341,
    }lcd_chip_id_t;

    void Init(gpio_num_t _ios[24],uint8_t _ionum,gpio_num_t _dcio,gpio_num_t _wrio,gpio_num_t _csio,gpio_num_t _rstio,
              lcd_chip_id_t _chipid,uint32_t _w = 240,uint32_t _h = 320)
    {
        BusHanle = nullptr;
        esp_lcd_i80_bus_config_t bus_config = {
            .dc_gpio_num = _dcio,
            .wr_gpio_num = _wrio,
            .clk_src = LCD_CLK_SRC_DEFAULT,
            .data_gpio_nums = {-1},
            .bus_width = _ionum,
            .max_transfer_bytes = _w * _h * 2,
            .psram_trans_align = 64,
            .sram_trans_align = 4,
        };
        for(int i = 0;i<_ionum;i++){
            bus_config.data_gpio_nums[i] = _ios[i];
        }
        ESP_ERROR_CHECK(esp_lcd_new_i80_bus(&bus_config, &BusHanle));

        esp_lcd_panel_io_i80_config_t io_config = {};
        io_config.cs_gpio_num = _csio;
        io_config.pclk_hz = 10000000;
        io_config.trans_queue_depth = 10;
        io_config.dc_levels = {
                .dc_idle_level = 0,
                .dc_cmd_level = 0,
                .dc_dummy_level = 0,
                .dc_data_level = 1,
        };
        io_config.flags = {
                .cs_active_high = 0,
                .reverse_color_bits = 0,
                .swap_color_bytes = 0,
                .pclk_active_neg = 0,
                .pclk_idle_low = 1,
        };
        io_config.on_color_trans_done = example_notify_lvgl_flush_ready;
        io_config.user_ctx = nullptr;
        io_config.lcd_cmd_bits = _ionum;
        io_config.lcd_param_bits = _ionum;
        ESP_ERROR_CHECK(esp_lcd_new_panel_io_i80(BusHanle, &io_config, &IOHandle));

        LCDPanelInit(_chipid,_rstio);
    }

    void DisplayOn(bool _on)
    {
        ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(PanelHandle, _on));
    }

    void DrawBitmap(uint16_t _x0,uint16_t _y0,uint16_t _x1,uint16_t _y1,const void* _colors)
    {
        ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(PanelHandle, _x0, _y0, _x1+1, _y1+1, _colors));
    }

private:
    esp_lcd_i80_bus_handle_t BusHanle;
    esp_lcd_panel_io_handle_t IOHandle;
    esp_lcd_panel_handle_t PanelHandle;

    void LCDPanelInit(lcd_chip_id_t _chipid,gpio_num_t _rstio)
    {
        esp_lcd_panel_dev_config_t panel_config = {};
        panel_config.reset_gpio_num = _rstio;
        panel_config.bits_per_pixel = 16;
        panel_config.flags.reset_active_high = 0;
        switch(_chipid){
            case ST7789:{
                panel_config.rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB;
                ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(IOHandle, &panel_config, &PanelHandle));
            }break;
            default:break;
        }
        esp_lcd_panel_reset(PanelHandle);
        esp_lcd_panel_init(PanelHandle);
        switch(_chipid){
            case ST7789:{
                esp_lcd_panel_invert_color(PanelHandle, false);
                esp_lcd_panel_set_gap(PanelHandle, 0, 0);
            }break;
            default:break;
        }
        
    }
};


#endif // __Z_DRVBASE_I80CTRL_HPP__
