/*--------------------------------------------------------------------
@file            : z_halbase_uart.hpp
@brief           : 
----------------------------------------------------------------------
@author          : zhangpeiwei02@gmail.com
 Release Version : 0.0.1
 Release Date    : 2024/4/5
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


#ifndef __Z_HALBASE_UART_HPP__
#define __Z_HALBASE_UART_HPP__

#include "cstdint"
#include <functional>


class zHalBase_UART{
public:
    typedef std::function<uint32_t (uint8_t*,uint16_t)> CheckCalAction;

    virtual uint16_t Send(uint8_t *_data,uint16_t _len) = 0;
    virtual bool Recv(uint8_t *_data,uint16_t _len,uint16_t *_rlen) = 0;
    virtual bool SetFrameFormat(uint8_t *_head,uint8_t _headlen,uint8_t _sizelen,uint8_t _checlen,CheckCalAction _handle,uint8_t *_end,uint8_t _endlen) = 0;
    virtual bool SetBaudRate(uint32_t _rate) = 0;

};


#endif  // __Z_HALBASE_UART_HPP__

