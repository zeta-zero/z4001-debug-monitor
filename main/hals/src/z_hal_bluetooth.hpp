/*--------------------------------------------------------------------
@file            : z_hal_bluetooth.hpp
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


#ifndef __Z_HAL_BLUETOOTH_HPP__
#define __Z_HAL_BLUETOOTH_HPP__

#include "cstdint"
#include "../base/z_halbase_gpio.hpp"

#include "esp_bt.h"
#include "bt_app_core.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include "esp_a2dp_api.h"
#include "esp_avrc_api.h"

// DEFINE ------------------------------------------------------------
#define HAL_BTMODE_A2DP   0x01
#define HAL_BTMODE_GATTC  0x02
#define HAL_BTMODE_GATTS  0x04

#define BT_TAG  "hal_bt"

/* AVRCP used transaction label */
#define BT_RC_CT_TL_GET_CAPS            (0)
#define BT_RC_CT_TL_RN_VOLUME_CHANGE    (1)


class zHal_Bluetooth {
public:

    static void Init(uint8_t _mode)
    {
        esp_bt_mode_t mode = ESP_BT_MODE_IDLE;
        if(_mode == HAL_BTMODE_A2DP){
            ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_BLE));
        }
        else if(_mode == HAL_BTMODE_GATTC || _mode == HAL_BTMODE_GATTS ||
                _mode == (HAL_BTMODE_GATTC && HAL_BTMODE_GATTS)){
            ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));
        }

        if(_mode & HAL_BTMODE_A2DP != 0){
            mode |= ESP_BT_MODE_CLASSIC_BT;
        }
        if(_mode & (HAL_BTMODE_GATTC && HAL_BTMODE_GATTS) != 0){
            mode |= ESP_BT_MODE_BLE;
        }
        
        esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
        ESP_ERROR_CHECK(esp_bt_controller_init(&bt_cfg));
        ESP_ERROR_CHECK(esp_bt_controller_enable(mode));
        esp_bluedroid_config_t bluedroid_cfg = BT_BLUEDROID_INIT_CONFIG_DEFAULT();
        ESP_ERROR_CHECK(esp_bluedroid_init_with_cfg(&bluedroid_cfg));

        esp_bt_pin_type_t pin_type = ESP_BT_PIN_TYPE_VARIABLE;
        esp_bt_pin_code_t pin_code;
        esp_bt_gap_set_pin(pin_type, 0, pin_code);

        BTClassicInit();
    }

    static void Enable(bool _enable)
    {
        if(_enable){
            esp_bluedroid_enable();
            ESP_LOGI(BT_TAG, "Starting device discovery...");
            LocalState = APP_AV_STATE_DISCOVERING;
            esp_bt_gap_start_discovery(ESP_BT_INQ_MODE_GENERAL_INQUIRY, 10, 0);

        }
        else{
            esp_bluedroid_disable();
            xTimerStop(LocalTimerHandle);
        }
    }

    static void Tick(uint32_t _ms)
    {
        if(TimeCount % 1000 == 0){
            BTA2DHeartBeat();
        }
        

        TimeCount += _ms;
    }

private:
    static esp_avrc_rn_evt_cap_mask_t AvrcPeerRnCap;
    static int32_t TimeCount;
    static uint32_t LocalState;

    static void BTClassicInit(char *_name)
    {
        esp_bt_dev_set_device(_name);
        esp_bt_gap_register_callback(GAPRegEvent);
        esp_avrc_ct_init();
        esp_avrc_ct_register_callback(BTRegEvent);

        esp_avrc_rn_evt_cap_mask_t evt_set = {0};
        esp_avrc_rn_evt_bit_mask_operation(ESP_AVRC_BIT_MASK_OP_SET, &evt_set, ESP_AVRC_RN_VOLUME_CHANGE);
        ESP_ERROR_CHECK(esp_avrc_tg_set_rn_evt_cap(&evt_set));

        esp_a2d_source_init();
        esp_a2d_register_callback(A2DRegEvent);
        esp_a2d_source_register_data_callback(A2DDataEvent);

        /* Avoid the state error of LocalState caused by the connection initiated by the peer device. */
        esp_bt_gap_set_scan_mode(ESP_BT_NON_CONNECTABLE, ESP_BT_NON_DISCOVERABLE);

    }

    static void GAPRegEvent(esp_bt_gap_cb_event_t _event, esp_bt_gap_cb_param_t *_param)
    {
        switch(_event){
            case ESP_BT_GAP_DISC_RES_EVT = 0:{                    /*!< Device discovery result event */
                ESP_LOGI(BT_TAG, "Scanned device: %s", bda2str(_param->disc_res.bda, bda_str, 18));
                for (int i = 0; i < _param->disc_res.num_prop; i++) {
                    p = _param->disc_res.prop + i;
                    switch (p->type) {
                    case ESP_BT_GAP_DEV_PROP_COD:
                        cod = *(uint32_t *)(p->val);
                        ESP_LOGI(BT_TAG, "--Class of Device: 0x%"PRIx32, cod);
                        break;
                    case ESP_BT_GAP_DEV_PROP_RSSI:
                        rssi = *(int8_t *)(p->val);
                        ESP_LOGI(BT_TAG, "--RSSI: %"PRId32, rssi);
                        break;
                    case ESP_BT_GAP_DEV_PROP_EIR:
                        eir = (uint8_t *)(p->val);
                        break;
                    case ESP_BT_GAP_DEV_PROP_BDNAME:
                    default:
                        break;
                    }
            }break;
            case ESP_BT_GAP_DISC_STATE_CHANGED_EVT:{              /*!< Discovery state changed event */

            }break;
            case ESP_BT_GAP_RMT_SRVCS_EVT:{                       /*!< Get remote services event */

            }break;
            case ESP_BT_GAP_RMT_SRVC_REC_EVT:{                   /*!< Get remote service record event */

            }break;
            case ESP_BT_GAP_AUTH_CMPL_EVT:{                       /*!< Authentication complete event */

            }break;
            case ESP_BT_GAP_PIN_REQ_EVT:{                         /*!< Legacy Pairing Pin code request */

            }break;
            case ESP_BT_GAP_CFM_REQ_EVT:{                        /*!< Security Simple Pairing User Confirmation request. */

            }break;
            case ESP_BT_GAP_KEY_NOTIF_EVT:{                      /*!< Security Simple Pairing Passkey Notification */

            }break;
            case ESP_BT_GAP_KEY_REQ_EVT:{                         /*!< Security Simple Pairing Passkey request */

            }break;
            case ESP_BT_GAP_READ_RSSI_DELTA_EVT:{                 /*!< Read rssi event */

            }break;
            case ESP_BT_GAP_CONFIG_EIR_DATA_EVT:{                 /*!< Config EIR data event */

            }break;
            case ESP_BT_GAP_SET_AFH_CHANNELS_EVT:{                /*!< Set AFH channels event */

            }break;
            case ESP_BT_GAP_READ_REMOTE_NAME_EVT:{                /*!< Read Remote Name event */

            }break;
            case ESP_BT_GAP_MODE_CHG_EVT:{

            }break;
            case ESP_BT_GAP_REMOVE_BOND_DEV_COMPLETE_EVT:{         /*!< remove bond device complete event */

            }break;
            case ESP_BT_GAP_QOS_CMPL_EVT:{                        /*!< QOS complete event */

            }break;
            case ESP_BT_GAP_ACL_CONN_CMPL_STAT_EVT:{              /*!< ACL connection complete status event */

            }break;
            case ESP_BT_GAP_ACL_DISCONN_CMPL_STAT_EVT:{           /*!< ACL disconnection complete status event */

            }break;
            case ESP_BT_GAP_SET_PAGE_TO_EVT:{                     /*!< Set page timeout event */

            }break;
            case ESP_BT_GAP_GET_PAGE_TO_EVT:{                     /*!< Get page timeout event */

            }break;
            case ESP_BT_GAP_ACL_PKT_TYPE_CHANGED_EVT:{            /*!< Set ACL packet types event */

            }break;
            case ESP_BT_GAP_ENC_CHG_EVT:{                         /*!< Encryption change event */
            }break;
            default:break;

        }

    }

    static void A2DRegEvent(esp_a2d_cb_event_t event, esp_a2d_cb_param_t *param)
    {
        ESP_LOGI(BT_TAG, "%s state: %d, event: 0x%x", __func__, LocalState, event);

        /* select handler according to different states */
        switch(event){
            case ESP_A2D_CONNECTION_STATE_EVT:{             /*!< connection state changed event */
            }break;
            case ESP_A2D_AUDIO_STATE_EVT:{                   /*!< audio stream transmission state changed event */
            }break;
            case ESP_A2D_AUDIO_CFG_EVT:{                     /*!< audio codec is configured, only used for A2DP SINK */
            }break;
            case ESP_A2D_MEDIA_CTRL_ACK_EVT:{                /*!< acknowledge event in response to media control commands */
            }break;
            case ESP_A2D_PROF_STATE_EVT:{                    /*!< indicate a2dp init&deinit complete */
            }break;
            case ESP_A2D_SNK_PSC_CFG_EVT:{                   /*!< protocol service capabilities configured，only used for A2DP SINK */
            }break;
            case ESP_A2D_SNK_SET_DELAY_VALUE_EVT:{           /*!< indicate a2dp sink set delay report value complete,  only used for A2DP SINK */
            }break;
            case ESP_A2D_SNK_GET_DELAY_VALUE_EVT:{           /*!< indicate a2dp sink get delay report value complete,  only used for A2DP SINK */
            }break;
            case ESP_A2D_REPORT_SNK_DELAY_VALUE_EVT:{        /*!< report delay value,  only used for A2DP SRC */
            }
            default:break;
        }
    }

    static void BTA2DHeartBeat(TimerHandle_t arg)
    {
        // AvSmHandle(BT_APP_HEART_BEAT_EVT,NULL);
    }

    /* generate some random noise to simulate source audio */
    static int32_t A2DDataEvent(uint8_t *data, int32_t len)
    {
        if (data == NULL || len < 0) {
            return 0;
        }

        int16_t *p_buf = (int16_t *)data;
        for (int i = 0; i < (len >> 1); i++) {
            p_buf[i] = rand() % (1 << 16);
        }

        return len;
    }

    static void BTRegEvent(esp_avrc_ct_cb_event_t event, esp_avrc_ct_cb_param_t *param)
    {
        switch(event){
            case ESP_AVRC_CT_CONNECTION_STATE_EVT:
            case ESP_AVRC_CT_PASSTHROUGH_RSP_EVT:
            case ESP_AVRC_CT_METADATA_RSP_EVT:
            case ESP_AVRC_CT_CHANGE_NOTIFY_EVT:
            case ESP_AVRC_CT_REMOTE_FEATURES_EVT:
            case ESP_AVRC_CT_GET_RN_CAPABILITIES_RSP_EVT:
            case ESP_AVRC_CT_SET_ABSOLUTE_VOLUME_RSP_EVT: {
                BTAvRspEvent(event,param);
            }break;
            default: {
                ESP_LOGE(BT_TAG, "Invalid AVRC event: %d", event);
            }break;
        }
    }

    static void BTAvRspEvent(uint16_t _eventid,esp_avrc_ct_cb_param_t *_val)
    {
        switch (_eventid) {
            /* when connection state changed, this event comes */
            case ESP_AVRC_CT_CONNECTION_STATE_EVT: {
                uint8_t *bda = _val->conn_stat.remote_bda;
                ESP_LOGI(BT_TAG, "AVRC conn_state event: state %d, [%02x:%02x:%02x:%02x:%02x:%02x]",
                        _val->conn_stat.connected, bda[0], bda[1], bda[2], bda[3], bda[4], bda[5]);

                if (_val->conn_stat.connected) {
                    esp_avrc_ct_send_get_rn_capabilities_cmd(APP_RC_CT_TL_GET_CAPS);
                } else {
                    AvrcPeerRnCap.bits = 0;
                }
            }break;
            /* when passthrough responsed, this event comes */
            case ESP_AVRC_CT_PASSTHROUGH_RSP_EVT: {
                ESP_LOGI(BT_TAG, "AVRC passthrough response: key_code 0x%x, key_state %d, rsp_code %d", _val->psth_rsp.key_code,
                            _val->psth_rsp.key_state, _val->psth_rsp.rsp_code);
                
            }break;
            /* when metadata responsed, this event comes */
            case ESP_AVRC_CT_METADATA_RSP_EVT: {
                ESP_LOGI(BT_TAG, "AVRC metadata response: attribute id 0x%x, %s", _val->meta_rsp.attr_id, _val->meta_rsp.attr_text);
                free(_val->meta_rsp.attr_text);
                
            }break;
            /* when notification changed, this event comes */
            case ESP_AVRC_CT_CHANGE_NOTIFY_EVT: {
                ESP_LOGI(BT_TAG, "AVRC event notification: %d", _val->change_ntf.event_id);
                AvNotifyEvent(_val->change_ntf.event_id, &_val->change_ntf.event_parameter);
                
            }break;
            /* when indicate feature of remote device, this event comes */
            case ESP_AVRC_CT_REMOTE_FEATURES_EVT: {
                ESP_LOGI(BT_TAG, "AVRC remote features %"PRIx32", TG features %x", _val->rmt_feats.feat_mask, _val->rmt_feats.tg_feat_flag);
                
            }break;
            /* when get supported notification events capability of peer device, this event comes */
            case ESP_AVRC_CT_GET_RN_CAPABILITIES_RSP_EVT: {
                ESP_LOGI(BT_TAG, "remote rn_cap: count %d, bitmask 0x%x", _val->get_rn_caps_rsp.cap_count,
                        _val->get_rn_caps_rsp.evt_set.bits);
                AvrcPeerRnCap.bits = _val->get_rn_caps_rsp.evt_set.bits;

                AVVolumeChanged();
                
            }break;
            /* when set absolute volume responsed, this event comes */
            case ESP_AVRC_CT_SET_ABSOLUTE_VOLUME_RSP_EVT: {
                ESP_LOGI(BT_TAG, "Set absolute volume response: volume %d", _val->set_volume_rsp.volume);
                
            }break;
            /* other */
            default: {
                ESP_LOGE(BT_TAG, "%s unhandled event: %d", __func__, event);
                
            }break;
        }
    }
    
    static void AVVolumeChanged(void)
    {
        if (esp_avrc_rn_evt_bit_mask_operation(ESP_AVRC_BIT_MASK_OP_TEST, &s_avrc_peer_rn_cap,
                                            ESP_AVRC_RN_VOLUME_CHANGE)) {
            esp_avrc_ct_send_register_notification_cmd(BT_RC_CT_TL_RN_VOLUME_CHANGE, ESP_AVRC_RN_VOLUME_CHANGE, 0);
        }
    }
    
    static void AvNotifyEvent(uint8_t event_id, esp_avrc_rn_param_t *event_parameter)
    {
        switch (event_id) {
        /* when volume changed locally on target, this event comes */
        case ESP_AVRC_RN_VOLUME_CHANGE: {
            ESP_LOGI(BT_RC_CT_TAG, "Volume changed: %d", event_parameter->volume);
            ESP_LOGI(BT_RC_CT_TAG, "Set absolute volume: volume %d", event_parameter->volume + 5);
            esp_avrc_ct_send_set_absolute_volume_cmd(BT_RC_CT_TL_RN_VOLUME_CHANGE, event_parameter->volume + 5);
            AVVolumeChanged();
            break;
        }
        /* other */
        default:
            break;
        }
    }


};


#endif  // __Z_HAL_BLUETOOTH_HPP__


