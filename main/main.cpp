#include <stdio.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "units/z_units.hpp"

extern "C" void app_main(void)
{
    ESP_LOGI("main","Start\n");

    uint32_t count = 0;
    LocalUnits.Init();
    LocalUnits.RGBLED.SetModel(zUnitLED::RainBow);
    while (1) {
        vTaskDelay(10 / portTICK_PERIOD_MS);
        LocalUnits.RGBLED.Tick(10);
        count++;
        if(count > 50){
            LocalUnits.Tick();
            count = 0;
        }
    }
    
}
