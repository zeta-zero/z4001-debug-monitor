/*--------------------------------------------------------------------
@file            : z_comm_base64.hpp
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

#ifndef __Z_COMM_BASE54_HPP__
#define __Z_COMM_BASE54_HPP__

#include "cstdint"
#include "cctype"
#include "cstring"

class zCommBase64{

public:
    static uint16_t Encode(uint8_t *_srcdata,uint16_t _srclen,uint8_t *_desdata,uint16_t _deslen)
    {    
        uint16_t res = 0;
        uint16_t i = 0, j = 0;
        uint16_t pos = 0;
        uint8_t char_array_3[3], char_array_4[4];
        res = 4 * ((_srclen + 2) / 3);
        if(_srcdata == NULL || _srclen == 0 || _desdata == NULL || _deslen == 0 ||
           res > _deslen ){
            res = 0;
            goto end;
        }

        while (_srclen--) {
            char_array_3[i++] = *(_srcdata++);
            if (i == 3) {
                char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                char_array_4[3] = char_array_3[2] & 0x3f;

                for (i = 0; (i < 4); i++) {
                    _desdata[pos++] = Base64Chars[char_array_4[i]];
                }
                i = 0;
            }
        }

        if (i) {
            for (j = i; j < 3; j++) {
                char_array_3[j] = '\0';
            }

            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (j = 0; (j < i + 1); j++) {
                _desdata[pos++] = Base64Chars[char_array_4[j]];
            }

            while (i++ < 3) {
                _desdata[pos++] = '=';
            }
        }

        _desdata[pos] = '\0';
    end:
        return res;
    }

    static uint16_t Decode(uint8_t *_srcdata,uint16_t _srclen,uint8_t *_desdata,uint16_t _deslen)
    {
        uint16_t res = (_srclen / 4) * 3;
        int i = 0, j = 0, in_ = 0;
        int pos = 0;
        unsigned char char_array_4[4], char_array_3[3];
        if(_srcdata == NULL || _srclen == 0 || _desdata == NULL || _deslen == 0 ||
           res > _deslen ){
            res = 0;
            goto end;
        }
        if (_srcdata[_srclen - 1] == '=') (res)--;
        if (_srcdata[_srclen - 2] == '=') (res)--;

        while (_srclen-- && (_srcdata[in_] != '=') && IsBase64(_srcdata[in_])) {
            char_array_4[i++] = _srcdata[in_]; in_++;
            if (i == 4) {
                for (i = 0; i < 4; i++) {
                    char_array_4[i] = strchr(Base64Chars, char_array_4[i]) - Base64Chars;
                }

                char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

                for (i = 0; (i < 3); i++) {
                    _desdata[pos++] = char_array_3[i];
                }
                i = 0;
            }
        }

        if (i) {
            for (j = i; j < 4; j++) {
                char_array_4[j] = 0;
            }

            for (j = 0; j < 4; j++) {
                char_array_4[j] = strchr(Base64Chars, char_array_4[j]) - Base64Chars;
            }

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (j = 0; (j < i - 1); j++) {
                _desdata[pos++] = char_array_3[j];
            }
        }
    end:
        return res;
    }

private:
    static const char Base64Chars[];
    static inline bool IsBase64(unsigned char c) {
        return (isalnum(c) || (c == '+') || (c == '/'));
    }

};

#ifndef __Z_COMM_BASE54_INIT__
#define __Z_COMM_BASE54_INIT__
const char zCommBase64::Base64Chars[] = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";
#endif

#endif  //__Z_COMM_BASE54_HPP__

