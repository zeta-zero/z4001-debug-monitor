#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "apps/z_app_webserver.hpp"
#include "apps/z_app_center.hpp"

extern "C" void app_main(void)
{
    ESP_LOGI("main","Start\n");

    uint32_t count = 0;
    int flag = 0;
    LocalUnits.Init();
    LocalUnits.RGBLED.SetModel(zUnitLED::RainBow);
    zApp_WebSvr WebSvr;
    zAppCenter LocalCenter;
    while (1) {
        vTaskDelay(10 / portTICK_PERIOD_MS);
        LocalUnits.RGBLED.Tick(10);
        LocalCenter.Tick(10);
        count++;
        if(count > 50){
            LocalUnits.Tick();
            count = 0;
        }
        if(flag != 200){
            flag++;
        }
        else if(flag == 200){
            flag++;
            WebSvr.Start();
        }
    }
    
}
