/*--------------------------------------------------------------------
@file            : z_halbase_i80ctrl.hpp
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


#ifndef __Z_HALBASE_I80CTRL_HPP__
#define __Z_HALBASE_I80CTRL_HPP__

#include "cstdint"

class zHalBase_I80Ctrl{
public:
    virtual void DrawBitmap(uint16_t _x0,uint16_t _y0,uint16_t _x1,uint16_t _y1,const void* _colors) = 0;
    virtual void DisplayOn(bool _on) = 0;
};


#endif // __Z_HALBASE_I80CTRL_HPP__
