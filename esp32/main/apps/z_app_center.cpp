/*--------------------------------------------------------------------
@file            : z_app_wcenter.cpp
@brief           : 
----------------------------------------------------------------------
@author          : zhangpeiwei02@gmail.com
 Release Version : 0.0.1
 Release Date    : 2024/05/19
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

#include "z_app_center.hpp"
#include "z_app_aisle.hpp"



/**-------------------------------------------------------------------
 * @fn     : zAppCenter
 * @brief  : 构造函数
 * @param  : none
 * @return : res
 */
zAppCenter::zAppCenter(void)
{

}


/**-------------------------------------------------------------------
 * @fn     : Tick
 * @brief  : 周期函数
 * @param  : none
 * @return : res
 */
void zAppCenter::Tick(uint32_t _ms)
{
    uint32_t temp = 0;
    uint8_t snedres = 0;
    zAppAisle::datapack_t *buf = NULL;
    if(zAppAisle::Recv(zAppAisle::Ch_Center,&buf) == ESP_OK){
        if(buf != NULL){
            if(buf->Len == 10){
                LocalUnits.RGBLED.SetColor(buf->Data[6],buf->Data[7],buf->Data[8]);
                snedres = 1;
            }
            else{
                snedres = 0;
            }
            zAppAisle::Send(zAppAisle::Ch_Center,buf->SrcCh,&snedres,1);
            free(buf);
        }
    }
}


