/*--------------------------------------------------------------------
@file            : z_drv_gt911.cpp
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

#include "z_drv_gt911.hpp"
#include "esp_log.h"
#include "cstring"


// DEFINE ------------------------------------------------------------
#define I2C_ADDR_0    0x14    // INTR High
#define I2C_ADDR_1    0x5D    // INTR Low


#define GT_COMMAND_REG              (uint16_t)0X8040
#define GT_COMMANDCHECK_REG         (uint16_t)0X8046

#define GT_COMMAND_ENTER_CHG        0x06
#define GT_COMMAND_EXIST_CHG        0x07
#define GT_COMMAND_ENTER_RCV        0x22
#define GT_COMMAND_EXIST_RCV        0x2A

// Configuration information (R/W)
#define GT_CFGS_REG                 (uint16_t)0X8047
#define GT_CONFIG_START             (uint16_t)0x8047
#define GT_CONFIG_VERSION           (uint16_t)0x8047
#define GT_X_OUTPUT_MAX_LOW         (uint16_t)0x8048
#define GT_X_OUTPUT_MAX_HIGH        (uint16_t)0x8049
#define GT_Y_OUTPUT_MAX_LOW         (uint16_t)0x804A
#define GT_Y_OUTPUT_MAX_HIGH        (uint16_t)0x804B
#define GT_TOUCH_NUMBER             (uint16_t)0x804C
#define GT_MODULE_SWITCH_1          (uint16_t)0x804D
#define GT_MODULE_SWITCH_2          (uint16_t)0x804E
#define GT_SHAKE_COUNT              (uint16_t)0x804F
#define GT_FILTER                   (uint16_t)0x8050

#define GT_LARGE_TOUCH              (uint16_t)0x8051
#define GT_NOISE_REDUCTION          (uint16_t)0x8052
#define GT_SCREEN_TOUCH_LEVEL       (uint16_t)0x8053
#define GT_SCREEN_RELEASE_LEVEL     (uint16_t)0x8054
#define GT_LOW_POWER_CONTROL        (uint16_t)0x8055
#define GT_REFRESH_RATE             (uint16_t)0x8056
#define GT_X_THRESHOLD              (uint16_t)0x8057
#define GT_Y_THRESHOLD              (uint16_t)0x8058
#define GT_X_SPEED_LIMIT            (uint16_t)0x8059 //Reserve
#define GT_Y_SPEED_LIMIT            (uint16_t)0x805A //Reserve

#define GT_SPACE_TOP_BOTTOM         (uint16_t)0x805B
#define GT_SPACE_LEFT_RIGHT         (uint16_t)0x805C
#define GT_MINI_FILTER              (uint16_t)0x805D
#define GT_STRETCH_R0               (uint16_t)0x805E
#define GT_STRETCH_R1               (uint16_t)0x805F
#define GT_STRETCH_R2               (uint16_t)0x8060
#define GT_STRETCH_RM               (uint16_t)0x8061
#define GT_DRV_GROUPA_NUM           (uint16_t)0x8062
#define GT_DRV_GROUPB_NUM           (uint16_t)0x8063
#define GT_SENSOR_NUM               (uint16_t)0x8064
#define GT_FREQ_A_FACTOR            (uint16_t)0x8065

#define GT_FREQ_B_FACTOR            (uint16_t)0x8066
#define GT_PANEL_BIT_FREQ_L         (uint16_t)0x8067
#define GT_PANEL_BIT_FREQ_H         (uint16_t)0x8068
#define GT_PANEL_SENSOR_TIME_L      (uint16_t)0x8069 //Reserve
#define GT_PANEL_SENSOR_TIME_H      (uint16_t)0x806A
#define GT_PANEL_TX_GAIN            (uint16_t)0x806B
#define GT_PANEL_RX_GAIN            (uint16_t)0x806C
#define GT_PANEL_DUMP_SHIFT         (uint16_t)0x806D
#define GT_DRV_FRAME_CONTROL        (uint16_t)0x806E
#define GT_CHARGING_LEVEL_UP        (uint16_t)0x806F

#define GT_MODULE_SWITCH3           (uint16_t)0x8070
#define GT_GESTURE_DIS              (uint16_t)0X8071
#define GT_GESTURE_LONG_PRESS_TIME  (uint16_t)0x8072
#define GT_X_Y_SLOPE_ADJUST         (uint16_t)0X8073
#define GT_GESTURE_CONTROL          (uint16_t)0X8074
#define GT_GESTURE_SWITCH1          (uint16_t)0X8075
#define GT_GESTURE_SWITCH2          (uint16_t)0X8076
#define GT_GESTURE_REFRESH_RATE     (uint16_t)0x8077
#define GT_GESTURE_TOUCH_LEVEL      (uint16_t)0x8078
#define GT_NEWGREENWAKEUPLEVEL      (uint16_t)0x8079

#define GT_FREQ_HOPPING_START       (uint16_t)0x807A
#define GT_FREQ_HOPPING_END         (uint16_t)0X807B
#define GT_NOISE_DETECT_TIMES       (uint16_t)0x807C
#define GT_HOPPING_FLAG             (uint16_t)0X807D
#define GT_HOPPING_THRESHOLD        (uint16_t)0X807E
#define GT_NOISE_THRESHOLD          (uint16_t)0X807F //Reserve
#define GT_NOISE_MIN_THRESHOLD      (uint16_t)0X8080
#define GT_HOPPING_SENSOR_GROUP     (uint16_t)0X8082
#define GT_HOPPING_SEG1_NORMALIZE   (uint16_t)0X8083
#define GT_HOPPING_SEG1_FACTOR      (uint16_t)0X8084

#define GT_MAIN_CLOCK_AJDUST        (uint16_t)0X8085
#define GT_HOPPING_SEG2_NORMALIZE   (uint16_t)0X8086
#define GT_HOPPING_SEG2_FACTOR      (uint16_t)0X8087
#define GT_HOPPING_SEG3_NORMALIZE   (uint16_t)0X8089
#define GT_HOPPING_SEG3_FACTOR      (uint16_t)0X808A
#define GT_HOPPING_SEG4_NORMALIZE   (uint16_t)0X808C
#define GT_HOPPING_SEG4_FACTOR      (uint16_t)0X808D
#define GT_HOPPING_SEG5_NORMALIZE   (uint16_t)0X808F
#define GT_HOPPING_SEG5_FACTOR      (uint16_t)0X8090
#define GT_HOPPING_SEG6_NORMALIZE   (uint16_t)0X8092

#define GT_KEY_1                    (uint16_t)0X8093
#define GT_KEY_2                    (uint16_t)0X8094
#define GT_KEY_3                    (uint16_t)0X8095
#define GT_KEY_4                    (uint16_t)0X8096
#define GT_KEY_AREA                 (uint16_t)0X8097
#define GT_KEY_TOUCH_LEVEL          (uint16_t)0X8098
#define GT_KEY_LEAVE_LEVEL          (uint16_t)0X8099
#define GT_KEY_SENS_1_2             (uint16_t)0X809A
#define GT_KEY_SENS_3_4             (uint16_t)0X809B
#define GT_KEY_RESTRAIN             (uint16_t)0X809C

#define GT_KEY_RESTRAIN_TIME        (uint16_t)0X809D
#define GT_GESTURE_LARGE_TOUCH      (uint16_t)0X809E
#define GT_HOTKNOT_NOISE_MAP        (uint16_t)0X80A1
#define GT_LINK_THRESHOLD           (uint16_t)0X80A2
#define GT_PXY_THRESHOLD            (uint16_t)0X80A3
#define GT_GHOT_DUMP_SHIFT          (uint16_t)0X80A4
#define GT_GHOT_RX_GAIN             (uint16_t)0X80A5
#define GT_FREQ_GAIN0               (uint16_t)0X80A6
#define GT_FREQ_GAIN1               (uint16_t)0X80A7
#define GT_FREQ_GAIN2               (uint16_t)0X80A8

#define GT_FREQ_GAIN3               (uint16_t)0X80A9
#define GT_COMBINE_DIS              (uint16_t)0X80B3
#define GT_SPLIT_SET                (uint16_t)0X80B4
#define GT_SENSOR_CH0               (uint16_t)0X80B7
#define GT_DRIVER_CH0               (uint16_t)0X80D5
#define GT_CONFIG_CHKSUM            (uint16_t)0X80FF
#define GT_CONFIG_FRESH             (uint16_t)0X8100
#define GT_CONFIG_SIZE              (uint16_t)(0xFF-0x46)
// Coordinate information
#define GT_PID_REG                  (uint16_t)0X8140
#define GT_FIRMWARE_VERSION         (uint16_t)0X8140
#define GT_RESOLUTION               (uint16_t)0X8140
#define GT_VENDOR_ID                (uint16_t)0X8140
#define GT_IMFORMATION              (uint16_t)0X8140
#define GT_VID_REG                  (uint16_t)0x814A      // Vendor ID
#define GT_GSTID_REG                (uint16_t)0X814E      // touch status
#define GT_POINT_1                  (uint16_t)0X814F
#define GT_POINT_2                  (uint16_t)0X8157
#define GT_POINT_3                  (uint16_t)0X815F
#define GT_POINT_4                  (uint16_t)0X8167
#define GT_POINT_5                  (uint16_t)0X816F
#define GT_MAX_POINT                5

#define GT_TPBASE_REG               (uint16_t)0X814F      // 触摸点基准地址

#define GT_TOUCH_DATA_READY  0x80    // is ready for host to read
#define GT_TOUCH_LARGE       0x40    // indicates there is large-area touch on TP.
#define GT_TOUCH_HAVEKEY     0x10    // Have touch key;
#define GT_TOUCH_NUMBER      0x0F    // Num of Touch Points;

// VALUE ------------------------------------------------------------
const uint8_t zDrv_GT911::DefaultConfigTable[] = {
    0x42, 0x40, 0x01, 0xF0, 0x00, 0x05, 0xCF, 0x00, 0x01, 0x0F,     /* ver,x_l,x_h,y_l,y_h,touch_num,mode_sw */
    0x14, 0x01, 0x50, 0x30, 0x05, 0x05, 0x00, 0x00, 0x00, 0x00,     /* 0x8051 - 0x805A,*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8a, 0x2a, 0x0c,     /* 0x805B - 0x8064,*/
    0x45, 0x47, 0x0c, 0x08, 0x00, 0x00, 0x00, 0x40, 0x03, 0x2c,     /* 0x8065 - 0x806E,*/
    0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x64, 0x32, 0x00, 0x00,     /* 0x806F - 0x8078,*/
    0x00, 0x28, 0x64, 0x94, 0xd5, 0x02, 0x07, 0x00, 0x00, 0x04,     /* 0x8079 - 0x8083,*/
    0x95, 0x2c, 0x00, 0x8b, 0x34, 0x00, 0x82, 0x3f, 0x00, 0x7d,     /* 0x8083 - 0x808C,*/
    0x4c, 0x00, 0x7a, 0x5b, 0x00, 0x7a, 0x00, 0x00, 0x00, 0x00,     /* 0x808D - 0x8096,*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     /* 0x8097 - 0x80A0,*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     /* 0x80A1 - 0x80AA,*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     /* 0x80AB - 0x80B4,*/
    0x00, 0x00, 0x18, 0x16, 0x14, 0x12, 0x10, 0x0e, 0x0c, 0x0a,     /* 0x80B5 - 0x80BE,*/
    0x08, 0x06, 0x04, 0x02, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,     /* 0x80BF - 0x80C8,*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     /* 0x80C9 - 0x80D2,*/
    0x00, 0x00, 0x16, 0x18, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21,     /* 0x80D3 - 0x80DC,*/
    0x22, 0x24, 0x13, 0x12, 0x10, 0x0f, 0x0a, 0x08, 0x06, 0x04,     /* 0x80DD - 0x80E6,*/
    0x02, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,     /* 0x80E7 - 0x80F0,*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     /* 0x80F1 - 0x80FA,*/
    0x00, 0x00, 0x00, 0x00                                          /* 0x80FB - 0x80FE,*/
};

/**-------------------------------------------------------------------
 * @fn     : zDrv_GT911
 * @brief  : 
 * @param  : none
 * @return : res
 */
zDrv_GT911::zDrv_GT911(zHalBase_I2C *_i2c,zHalBase_GPIO *_rst,zHalBase_GPIO *_intr)
{
    Init(_i2c,_rst,_intr);
}

/**-------------------------------------------------------------------
 * @fn     : Init
 * @brief  : 
 * @param  : none
 * @return : res
 */
void zDrv_GT911::Init(zHalBase_I2C *_i2c,zHalBase_GPIO *_rst,zHalBase_GPIO *_intr)
{
    assert(_i2c != nullptr || _rst != nullptr || _intr != nullptr);
    LocalI2C = _i2c;
    IICAddr = I2C_ADDR_0;
    IICAddr0 = I2C_ADDR_0;
    IICAddr1 = I2C_ADDR_1;
    CheckCount = 0;

    LocalRST = _rst;
    LocalIntr = _intr;

    ResetRelease();
}

/**-------------------------------------------------------------------
 * @fn     : Check
 * @brief  : 
 * @param  : none
 * @return : res
 */
bool zDrv_GT911::Check(void)
{
    bool res = false;
    uint8_t buf[5] = {0};
    GetProductID((char*)buf);
    CheckCount++;
    ESP_LOGI("gt911","chip : %s",(char*)buf);
    if(memcmp((char*)buf,"911",3) == 0){
        res = true;
    }
    if(CheckCount > 3){
        IICAddr = I2C_ADDR_0;
        CheckCount = 0;
    }

    return res;
}

/**-------------------------------------------------------------------
 * @fn     : Start
 * @brief  : 
 * @param  : none
 * @return : res
 */
void zDrv_GT911::Start(void)
{
    uint8_t buf[1] = {0};
    uint8_t reg[3] = {(uint8_t)((GT_CFGS_REG >> 8) & 0xFF),(uint8_t)(GT_CFGS_REG & 0xFF),0};
    uint8_t count = 0;

    while(Check() == false && count++ < 5);
    LocalI2C->Write(IICAddr,reg,3);
    LocalI2C->WrRd(IICAddr,reg,2,buf,1);
    // if(buf[0] == (uint8_t)'A')
    {
        ESP_LOGI("gt911","update config");
        UpdateConfig(DefaultConfigTable);
    }

    SetModel(0);
    CleanStatus();
}

/**-------------------------------------------------------------------
 * @fn     : GetProductID
 * @brief  : 
 * @param  : none
 * @return : res
 */
void zDrv_GT911::GetProductID(char _id[5])
{
    uint8_t reg[2] = {(uint8_t)((GT_PID_REG >> 8) & 0xFF),(uint8_t)(GT_PID_REG & 0xFF)};

    LocalI2C->WrRd(IICAddr,reg,2,(uint8_t*)_id,4);
}

/**-------------------------------------------------------------------
 * @fn     : GetProductID
 * @brief  : 
 * @param  : none
 * @return : res
 */
uint8_t zDrv_GT911::GetVendorID(void)
{
    uint8_t res = 0;
    uint8_t reg[2] = {(uint8_t)((GT_PID_REG >> 8) & 0xFF),(uint8_t)(GT_PID_REG & 0xFF)};

    LocalI2C->WrRd(IICAddr,reg,2,&res,1);

    return res;
}

/**-------------------------------------------------------------------
 * @fn     : ResetKeep
 * @brief  : 
 * @param  : none
 * @return : res
 */
void zDrv_GT911::ResetKeep(void)
{
    LocalRST->SetLevel(0);
}

/**-------------------------------------------------------------------
 * @fn     : ResetRelease
 * @brief  : 
 * @param  : none
 * @return : res
 */
void zDrv_GT911::ResetRelease(void)
{
    LocalRST->SetLevel(1);
}

/**-------------------------------------------------------------------
 * @fn     : Tick
 * @brief  : 
 * @param  : none
 * @return : res
 */
void zDrv_GT911::Tick(uint32_t _ms)
{

}

/**-------------------------------------------------------------------
 * @fn     : UpdateConfig
 * @brief  : 
 * @param  : none
 * @return : res
 */
void zDrv_GT911::UpdateConfig(const uint8_t _buf[184])
{
    uint8_t data[32]={0};
    
    // SetModel(GT_COMMAND_ENTER_CHG);

    data[0] = (GT_CFGS_REG >> 8) & 0xFF;
    data[1] = GT_CFGS_REG & 0xFF;
    for(int i = 0;i<8;i++){
        memcpy(&data[2],&_buf[i * 20],20);
        LocalI2C->Write(IICAddr,data,22);
        data[1] += 20;
    }
    
    memcpy(&data[2],&_buf[160],24);
    LocalI2C->Write(IICAddr,data,26);
    data[1] += 24;

    data[2] = 0;
    for(int i = 0;i<184;i++){
        data[2] += _buf[i];
    }
    data[2] = (~data[2]) + 1;
    data[3] = 0;
    LocalI2C->Write(IICAddr,data,4);
    
    // SetModel(GT_COMMAND_EXIST_CHG);

}

/**-------------------------------------------------------------------
 * @fn     : GetPoints
 * @brief  : 
 * @param  : none
 * @return : res
 */
zDrv_GT911::point2_list_t zDrv_GT911::GetPoints(void)
{
    point2_list_t res = {0};
    uint8_t sta = 0;

    if(LocalIntr->GetLevel() == 0){
        goto end;
    }
    sta = GetStatus();
    ESP_LOGI("drv_911","sta : 0x%x",sta);

    if((sta & GT_TOUCH_DATA_READY) != 0){
        if((sta & GT_TOUCH_NUMBER) != 0){
            res = ReadPoints(sta & GT_TOUCH_NUMBER);
        }
        CleanStatus();
    }
    
end:
    return res;
}

/**-------------------------------------------------------------------
 * @fn     : GetStatus
 * @brief  : 
 * @param  : none
 * @return : res
 */
uint8_t zDrv_GT911::GetStatus(void)
{
    uint8_t res;
    uint8_t reg[2] = {(uint8_t)(GT_GSTID_REG >> 8),(uint8_t)(GT_GSTID_REG & 0xFF)};

    LocalI2C->WrRd(IICAddr,reg,2,&res,1);
    return res;
}

/**-------------------------------------------------------------------
 * @fn     : CleanStatus
 * @brief  : 
 * @param  : none
 * @return : res
 */
void zDrv_GT911::CleanStatus(void)
{
    uint8_t buf[3] = {(uint8_t)(GT_GSTID_REG >> 8),(uint8_t)(GT_GSTID_REG & 0xFF),0};

    LocalI2C->Write(IICAddr,buf,3);
}

/**-------------------------------------------------------------------
 * @fn     : CleanStatus
 * @brief  : 
 * @param  : none
 * @return : res
 */
void zDrv_GT911::SetModel(uint8_t _command)
{
    uint8_t buf[3] = {0};

    buf[2] = _command;
    if(_command > 0x07){
        buf[0] = (uint8_t)(GT_COMMANDCHECK_REG >> 8);
        buf[1] = (uint8_t)(GT_COMMANDCHECK_REG & 0xFF);
        LocalI2C->Write(IICAddr,buf,3);
    }
    
    buf[0] = (uint8_t)(GT_COMMAND_REG >> 8);
    buf[1] = (uint8_t)(GT_COMMAND_REG & 0xFF);
    LocalI2C->Write(IICAddr,buf,3);
    
}

/**-------------------------------------------------------------------
 * @fn     : ReadPoints
 * @brief  : 
 * @param  : none
 * @return : res
 */
zDrv_GT911::point2_list_t zDrv_GT911::ReadPoints(uint8_t _num)
{
    point2_list_t res = {0};
    uint8_t buf[GT_MAX_POINT * 8] = {0};
    uint8_t reg[2] = {(uint8_t)(GT_TPBASE_REG >> 8),(uint8_t)(GT_TPBASE_REG & 0xFF)};
    if(_num > GT_MAX_POINT){
        _num = GT_MAX_POINT;
    }
    LocalI2C->WrRd(IICAddr,reg,2,buf,_num * 8);
    for(int i = 0;i<_num;i++){
        res.Point2[i].X = ((buf[8 * i + 1] << 0)&0xFF) | ((buf[8 * i + 2] << 8)&0xFF00);
        res.Point2[i].Y = ((buf[8 * i + 3] << 0)&0xFF) | ((buf[8 * i + 4] << 8)&0xFF00);
    }
    res.Num = _num;

    return res;
}
