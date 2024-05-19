/*--------------------------------------------------------------------
@file            : z_hal_uart.hpp
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


#ifndef __Z_HAL_UART_HPP__
#define v

#include "cstdint"
#include "cstring"
#include "../base/z_halbase_uart.hpp"
#include "driver/uart.h"
#include "driver/gpio.h"


class zHal_UART : public zHalBase_UART{
public:

    typedef struct{
        uint8_t *Head;
        uint8_t HeadLen;
        uint8_t SizeLen;
        uint8_t CheckLen;
        CheckCalAction CheckHandl;
        uint8_t *End;
        uint8_t EndLen;
        uint8_t *Buffer;
        uint16_t TotalLen;
    }frameformat_t;


    void Init(gpio_num_t _rxio,gpio_num_t _txio,int _baudrate = 921600,uart_port_t _port = UART_NUM_0,
              uart_word_length_t _databits = UART_DATA_8_BITS,uart_parity_t _parity = UART_PARITY_DISABLE, uart_stop_bits_t _stopbis = UART_STOP_BITS_1)
    {
        uart_config_t config = {};
        config.baud_rate = _baudrate;
        config.data_bits =  _databits;
        config.parity = _parity;
        config.stop_bits = _stopbis;
        config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
        config.rx_flow_ctrl_thresh = 0;
        Port = _port;
        ESP_ERROR_CHECK(uart_param_config(Port, &config));

        ESP_ERROR_CHECK(uart_set_pin(Port, _txio,_rxio,GPIO_NUM_NC,GPIO_NUM_NC));

        // Install UART driver using an event queue here
        ESP_ERROR_CHECK(uart_driver_install(Port, 1024, 0, 0, nullptr, 0));

        FrameFormat.Head = nullptr;
        FrameFormat.HeadLen = 0;
        FrameFormat.SizeLen = 0;
        FrameFormat.CheckLen = 0;
        FrameFormat.CheckHandl = nullptr;
        FrameFormat.End = nullptr;
        FrameFormat.EndLen = 0;
        FrameFormat.TotalLen = 0;
    }

    bool SetFrameFormat(uint8_t *_head,uint8_t _headlen,uint8_t _sizelen,uint8_t _checlen,CheckCalAction _handle,uint8_t *_end,uint8_t _endlen)
    {
        bool res = false;
        if(_checlen != 0 && _handle == nullptr){
            goto end;
        }
        if(FrameFormat.Head != nullptr){
            free(FrameFormat.Head);
        }
        FrameFormat.Head = (uint8_t*)malloc(_headlen);
        memcpy(FrameFormat.Head,_head,_headlen);
        FrameFormat.HeadLen = _headlen;
        FrameFormat.SizeLen = _sizelen > 4 ? 4:_sizelen;
        FrameFormat.CheckLen = _checlen > 4 ? 4 :_checlen;
        FrameFormat.CheckHandl = _handle;
        if(FrameFormat.End != nullptr){
            free(FrameFormat.End);
        }
        FrameFormat.End = (uint8_t*)malloc(_endlen);
        memcpy(FrameFormat.End,_end,_endlen);
        FrameFormat.EndLen = _endlen;
        FrameFormat.TotalLen = _headlen + _sizelen + _checlen + _endlen;
        FrameFormat.Buffer = (uint8_t*)malloc(FrameFormat.TotalLen);
        res = true;
    end:
        return res;
    }

    uint16_t Send(uint8_t *_data,uint16_t _len)
    {
        uint16_t res = 0;
        uint8_t buf[4];
        uint32_t data = 0;
        if(FrameFormat.HeadLen != 0 && FrameFormat.Head != nullptr){
            uart_write_bytes(Port,FrameFormat.Head,FrameFormat.HeadLen);
        }
        if(FrameFormat.SizeLen != 0){
            for(int i = 0;i<FrameFormat.SizeLen;i++){
                buf[3 - i] = (_len >> i * 8) & 0xFF;
            }
            uart_write_bytes(Port,buf,FrameFormat.SizeLen);
        }
        res = uart_write_bytes(Port,_data,_len);
        if(FrameFormat.CheckLen != 0){
            data = FrameFormat.CheckHandl(_data,_len);
            for(int i = 0;i<FrameFormat.CheckLen;i++){
                buf[FrameFormat.CheckLen - i - 1] = (data >> i * 8) & 0xFF;
            }
            uart_write_bytes(Port,buf,FrameFormat.CheckLen);
        }
        if(FrameFormat.EndLen != 0 && FrameFormat.End != nullptr){
            uart_write_bytes(Port,FrameFormat.End,FrameFormat.EndLen);
        }

        return res;
    }
    bool Recv(uint8_t *_data,uint16_t _len,uint16_t *_rlen)
    {
        bool res = false;
        size_t len;
        uint32_t data = 0;
        if(_data == nullptr || _len == 0 || _rlen == 0){
            goto end;
        }
        if( uart_get_buffered_data_len(Port,&len) != ESP_OK){
            goto end;
        }
        if(len > _len){len = _len;}
        if(FrameFormat.TotalLen != 0 ){
            if(len < FrameFormat.TotalLen){
                goto end;
            }
            else if(FrameFormat.SizeLen == 0 && _len + FrameFormat.TotalLen > len){
                goto end;
            }
            if(FrameFormat.HeadLen != 0 && FrameFormat.Head != nullptr){
                len = uart_read_bytes(Port,FrameFormat.Buffer,FrameFormat.HeadLen,0);
                if(len != FrameFormat.HeadLen || memcmp(FrameFormat.Head,FrameFormat.Buffer,len) != 0){
                    goto end;
                }
            }
            if(FrameFormat.SizeLen != 0){
                len = uart_read_bytes(Port,FrameFormat.Buffer,FrameFormat.SizeLen,0);
                if(len != FrameFormat.SizeLen){
                    goto end;
                }
                *_rlen = 0;
                for(int i = 0;i<FrameFormat.SizeLen;i++){
                    *_rlen |= FrameFormat.Buffer[FrameFormat.SizeLen - i - 1] << (i * 8);
                }
                len = uart_read_bytes(Port,_data,*_rlen,0);
                if(*_rlen != len){
                    goto end;
                }
            }
            else{
                *_rlen = uart_read_bytes(Port,_data,*_rlen,0);
            }
            if(FrameFormat.CheckLen != 0){
                data = FrameFormat.CheckHandl(_data,*_rlen);
                uart_read_bytes(Port,FrameFormat.Buffer,FrameFormat.CheckLen,0);
                for(int i = 0;i<FrameFormat.CheckLen;i++){
                    if(FrameFormat.Buffer[0] != (data >> ((FrameFormat.CheckLen - i - 1) * 8) & 0xFF)){
                        *_rlen = 0;
                        goto end;
                    }
                }
            }
            if(FrameFormat.EndLen != 0 && FrameFormat.End != nullptr){
                len = uart_read_bytes(Port,FrameFormat.Buffer,FrameFormat.HeadLen,0);
                if(len != FrameFormat.EndLen || memcmp(FrameFormat.End,FrameFormat.Buffer,len) != 0){
                    *_rlen = 0;
                    goto end;
                }
            }


        }
        else{
            *_rlen = uart_read_bytes(Port,_data,len,0);
        }



        res = true;
    end:
        return res;
    }

    bool SetBaudRate(uint32_t _rate)
    {
        return uart_set_baudrate(Port,_rate) == ESP_OK;
    }

    bool UpdateConfig(uint32_t _baudrate = 921600,uart_word_length_t _databits = UART_DATA_8_BITS,
                      uart_parity_t _parity = UART_PARITY_DISABLE, uart_stop_bits_t _stopbis = UART_STOP_BITS_1)
    {
        uart_config_t config = {};
        config.baud_rate = _baudrate;
        config.data_bits =  _databits;
        config.parity = _parity;
        config.stop_bits = _stopbis;
        config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
        config.rx_flow_ctrl_thresh = 0;

        return uart_param_config(Port, &config) == ESP_OK;
    }

private:
    uart_port_t Port;
    QueueHandle_t RxTxQueue;
    frameformat_t FrameFormat;

};


#endif  // __Z_HAL_UART_HPP__

