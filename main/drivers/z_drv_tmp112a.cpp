/*--------------------------------------------------------------------
@file            : z_drv_tmp112a.cpp
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

#include "z_drv_tmp112a.hpp"

// DEFINE ---------------------------------------------------------------------
#define I2C_Addr    0x48
#define DEVICE_ID   0x6B

#define REG_TEMP    0x00
#define REG_CONFIG  0x01
#define REG_TLOW    0x02
#define REG_THIGH   0x03

#define TMP112A_CFGREG_CLEAN  0x0000
#define TMP112A_CFGREG_EM     0x0010
#define TMP112A_CFGREG_AL     0x0020
#define TMP112A_CFGREG_CR0    0x0040
#define TMP112A_CFGREG_CR1    0x0050
#define TMP112A_CFGREG_SD     0x0100
#define TMP112A_CFGREG_TM     0x0200
#define TMP112A_CFGREG_POL    0x0400
#define TMP112A_CFGREG_F0     0x0800
#define TMP112A_CFGREG_F1     0x1000
#define TMP112A_CFGREG_R0     0x2000
#define TMP112A_CFGREG_R1     0x4000
#define TMP112A_CFGREG_OS     0x8000

/**-------------------------------------------------------------------
 * @fn     : zDrv_TMP112A
 * @brief  : 
 * @param  : none
 * @return : res
 */
zDrv_TMP112A::zDrv_TMP112A(zHalBase_I2C *_i2c)
{
    LocalI2C = _i2c;
    IICAddr = I2C_Addr;
    ConfigRegBuf = 0;
    OnceModel = 0;
}

/**-------------------------------------------------------------------
 * @fn     : ReadF
 * @brief  : 
 * @param  : none
 * @return : res
 */
float zDrv_TMP112A::ReadF(void)
{
    float res = 0.0;
    uint8_t buf[2] = {0};
    uint8_t reg = REG_TEMP;
    int val;
    LocalI2C->WrRd(IICAddr,&reg,1,buf,2);
    val = (uint16_t)(((buf[0] << 4) & 0x1FF0) | ((buf[1] >> 4) & 0x0F));
    /* positive*/
    if((val & 0x1000) == 0 ){
        res = (val & 0xFFF) * 0.0625f;
    }
    else{
        res = (0x1000 - (val & 0xFFF)) * -0.625f;
    }

    return res;
}

/**-------------------------------------------------------------------
 * @fn     : ReadI
 * @brief  : 
 * @param  : none
 * @return : res   the val is multiple 100,such as 10.01 equal 1001
 */
int zDrv_TMP112A::ReadI(void)
{
    int res = 0;
    uint8_t buf[2] = {0};
    uint8_t reg = REG_TEMP;
    uint16_t val;
    LocalI2C->WrRd(IICAddr,&reg,1,buf,2);
    val = (uint16_t)(((buf[0] << 4) & 0x1FF0) | ((buf[1] >> 4) & 0x0F));
    /* positive*/
    if((val & 0x1000) == 0 ){
        res = ((val & 0xFFF) * 100) >> 4;
    }
    else{
        res = ((0x1000 - (val & 0xFFF)) * 100) >> 4;
    }
    return res;
}

/**-------------------------------------------------------------------
 * @fn     : ReadConfig
 * @brief  : 
 * @param  : none
 * @return : res
 */
uint16_t zDrv_TMP112A::ReadConfig(void)
{
    uint8_t buf[2] = {0};
    uint8_t reg = REG_CONFIG;
    LocalI2C->WrRd(IICAddr,&reg,1,buf,2);
    ConfigRegBuf = ((buf[0] << 8) & 0xFF00) | buf[1];
    if(ConfigRegBuf & TMP112A_CFGREG_SD){
        OnceModel = 1;
    }
    
    return ConfigRegBuf;
}

/**-------------------------------------------------------------------
 * @fn     : SetExpandModel
 * @brief  : 
 * @param  : none
 * @return : res
 */
bool zDrv_TMP112A::SetExpandModel(uint8_t _val)
{
    bool res = false;
    ConfigRegBuf &= (~TMP112A_CFGREG_EM);
    ConfigRegBuf |= ((_val & 0x01) << 4);
    uint8_t buf[3] = {REG_CONFIG,(uint8_t)(ConfigRegBuf >> 8),(uint8_t)(ConfigRegBuf & 0xFF)};
    LocalI2C->Write(IICAddr,buf,3);

    res = true;
    return res;
}

/**-------------------------------------------------------------------
 * @fn     : SetUpdateFreq
 * @brief  : 
 * @param  : none
 * @return : res
 */
bool zDrv_TMP112A::SetUpdateFreq(update_frequence_t _val)
{
    bool res = false;
    ConfigRegBuf &= (~TMP112A_CFGREG_EM);
    ConfigRegBuf |= ((_val & 0x03) << 6);
    uint8_t buf[3] = {REG_CONFIG,(uint8_t)(ConfigRegBuf >> 8),(uint8_t)(ConfigRegBuf & 0xFF)};
    LocalI2C->Write(IICAddr,buf,3);

    res = true;
    return res;
}

/**-------------------------------------------------------------------
 * @fn     : SetRunOnce
 * @brief  : 
 * @param  : none
 * @return : res
 */
bool zDrv_TMP112A::SetRunOnce(uint8_t _val)
{
    bool res = false;
    ConfigRegBuf &= (~TMP112A_CFGREG_EM);
    ConfigRegBuf |= ((_val & 0x01) << 8);
    uint8_t buf[3] = {REG_CONFIG,(uint8_t)(ConfigRegBuf >> 8),(uint8_t)(ConfigRegBuf & 0xFF)};
    LocalI2C->Write(IICAddr,buf,3);

    res = true;
    return res; 
}

/**-------------------------------------------------------------------
 * @fn     : SetThermostat
 * @brief  : 
 * @param  : none
 * @return : res
 */
bool zDrv_TMP112A::SetThermostat(thermostat_model_t _val)
{
    bool res = false;
    ConfigRegBuf &= (~TMP112A_CFGREG_EM);
    ConfigRegBuf |= ((_val & 0x01) << 9);
    uint8_t buf[3] = {REG_CONFIG,(uint8_t)(ConfigRegBuf >> 8),(uint8_t)(ConfigRegBuf & 0xFF)};
    LocalI2C->Write(IICAddr,buf,3);

    res = true;
    return res;
}

/**-------------------------------------------------------------------
 * @fn     : SetAlarmPolarity
 * @brief  : 
 * @param  : none
 * @return : res
 */
bool zDrv_TMP112A::SetAlarmPolarity(uint8_t _val)
{
    bool res = false;
    ConfigRegBuf &= (~TMP112A_CFGREG_EM);
    ConfigRegBuf |= ((_val & 0x01) << 10);
    uint8_t buf[3] = {REG_CONFIG,(uint8_t)(ConfigRegBuf >> 8),(uint8_t)(ConfigRegBuf & 0xFF)};
    LocalI2C->Write(IICAddr,buf,3);

    res = true;
    return res;
}

/**-------------------------------------------------------------------
 * @fn     : SetAlarmTriggerCount
 * @brief  : 
 * @param  : none
 * @return : res
 */
bool zDrv_TMP112A::SetAlarmTriggerCount(alarm_trigger_count_t _val)
{
    bool res = false;
    ConfigRegBuf &= (~TMP112A_CFGREG_EM);
    ConfigRegBuf |= ((_val & 0x03) << 11);
    uint8_t buf[3] = {REG_CONFIG,(uint8_t)(ConfigRegBuf >> 8),(uint8_t)(ConfigRegBuf & 0xFF)};
    LocalI2C->Write(IICAddr,buf,3);

    res = true;
    return res;
}

/**-------------------------------------------------------------------
 * @fn     : SetHighLimit
 * @brief  : Set higer alarm threshold
 * @param  : none
 * @return : res
 */
bool zDrv_TMP112A::SetHighLimit(float _val)
{
    bool res = false;
    uint16_t data = 0;
    if(_val > 0.0f){
        data = (uint16_t)(_val * 16);
    }
    else{
        data = 0xFFFF - (uint16_t)(_val * 16) + 1;
    }
    uint8_t buf[3] = {REG_THIGH,(uint8_t)((data >> 4) & 0xFF),(uint8_t)((data << 4) & 0xF0)};
    LocalI2C->Write(IICAddr,buf,3);

    res = true;
    return res;
}

/**-------------------------------------------------------------------
 * @fn     : SetLowLimit
 * @brief  : Set lower alarm threshold
 * @param  : none
 * @return : res
 */
bool zDrv_TMP112A::SetLowLimit(float _val)
{
    bool res = false;
    uint16_t data = 0;
    if(_val > 0.0f){
        data = (uint16_t)(_val / 0.0625f);
    }
    else{
        data = 0xFFFF - (uint16_t)(_val * 16) + 1;
    }
    uint8_t buf[3] = {REG_TLOW,(uint8_t)((data >> 4) & 0xFF),(uint8_t)((data << 4) & 0xF0)};
    LocalI2C->Write(IICAddr,buf,3);

    res = true;
    return res;
}


/**-------------------------------------------------------------------
 * @fn     : RunOnce
 * @brief  : 
 * @param  : none
 * @return : res
 */
void zDrv_TMP112A::RunOnce(void)
{
    if(OnceModel == 0){
        return;
    }
    ConfigRegBuf |= TMP112A_CFGREG_OS; 
    uint8_t buf[3] = {REG_CONFIG,(uint8_t)(ConfigRegBuf >> 8),(uint8_t)(ConfigRegBuf & 0xFF)};
    LocalI2C->Write(IICAddr,buf,3);
 
}
