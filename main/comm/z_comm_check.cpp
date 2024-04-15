/*--------------------------------------------------------------------
@file            : z_comm_check.cpp
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

#include "z_comm_check.hpp"

/**-------------------------------------------------------------------
 * @fn     : CheckSum8
 * @brief  : 
 * @param  : none
 * @return : res
 */
uint8_t z_comm_check::CheckSum8(uint8_t *_data,uint16_t _len)
{
    uint8_t res = 0;
    if(_data == nullptr || _len == 0){
        goto end;
    }
    for(uint32_t i = 0 ;i<_len;i++){
        res += _data[i];
    }

end:
    return res;
}


/**-------------------------------------------------------------------
 * @fn     : CheckSum16
 * @brief  : 
 * @param  : none
 * @return : res
 */
uint16_t z_comm_check::CheckSum16(uint8_t *_data,uint16_t _len)
{
    uint16_t res = 0;
    if(_data == nullptr || _len == 0){
        goto end;
    }
    for(uint32_t i = 0 ;i<_len;i++){
        res += _data[i];
    }

end:
    return res;
}


/**-------------------------------------------------------------------
 * @fn     : CheckSum32
 * @brief  : 
 * @param  : none
 * @return : res
 */
uint32_t z_comm_check::CheckSum32(uint8_t *_data,uint16_t _len)
{
    uint32_t res = 0;
    if(_data == nullptr || _len == 0){
        goto end;
    }
    for(uint32_t i = 0 ;i<_len;i++){
        res += _data[i];
    }

end:
    return res;
}
