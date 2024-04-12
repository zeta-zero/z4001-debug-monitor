/*--------------------------------------------------------------------
@file            : z_comm_check.hpp
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

#ifndef __Z_COMM_CHECK_HPP__
#define __Z_COMM_CHECK_HPP__

#include "cstdint"

class z_comm_check{

public:
    static uint8_t CheckSum8(uint8_t *_data,uint16_t _len);
    static uint16_t CheckSum16(uint8_t *_data,uint16_t _len);
    static uint32_t CheckSum32(uint8_t *_data,uint16_t _len);


};



#endif  //__Z_COMM_CHECK_HPP__

