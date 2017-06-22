#ifndef ANT_MESSAGE_DEFS_H_
#define ANT_MESSAGE_DEFS_H_

//////////////////////////////////////////////
// Message ID's
//////////////////////////////////////////////
#define MESG_INVALID_ID                          0x00
#define MESG_EVENT_ID                            0x01

#define MESG_VERSION_ID                          0x3E
#define MESG_RESPONSE_EVENT_ID                   0x40

#define MESG_UNASSIGN_CHANNEL_ID                 0x41
#define MESG_ASSIGN_CHANNEL_ID                   0x42
#define MESG_CHANNEL_MESG_PERIOD_ID              0x43
#define MESG_CHANNEL_SEARCH_TIMEOUT_ID           0x44
#define MESG_CHANNEL_RADIO_FREQ_ID               0x45
#define MESG_NETWORK_KEY_ID                      0x46
#define MESG_RADIO_TX_POWER_ID                   0x47
#define MESG_RADIO_CW_MODE_ID                    0x48
#define MESG_SYSTEM_RESET_ID                     0x4A
#define MESG_OPEN_CHANNEL_ID                     0x4B
#define MESG_CLOSE_CHANNEL_ID                    0x4C
#define MESG_REQUEST_ID                          0x4D

#define MESG_BROADCAST_DATA_ID                   0x4E
#define MESG_ACKNOWLEDGED_DATA_ID                0x4F
#define MESG_BURST_DATA_ID                       0x50

#define MESG_CHANNEL_ID_ID                       0x51
#define MESG_CHANNEL_STATUS_ID                   0x52
#define MESG_RADIO_CW_INIT_ID                    0x53
#define MESG_CAPABILITIES_ID                     0x54

#define MESG_STACKLIMIT_ID                       0x55

#define MESG_SCRIPT_DATA_ID                      0x56
#define MESG_SCRIPT_CMD_ID                       0x57

#define MESG_ID_LIST_ADD_ID                      0x59
#define MESG_CRYPTO_ID_LIST_ADD_ID               0x59
#define MESG_ID_LIST_CONFIG_ID                   0x5A
#define MESG_CRYPTO_ID_LIST_CONFIG_ID            0x5A
#define MESG_OPEN_RX_SCAN_ID                     0x5B

#define MESG_EXT_CHANNEL_RADIO_FREQ_ID           0x5C  // OBSOLETE: for 905 radio
#define MESG_EXT_BROADCAST_DATA_ID               0x5D
#define MESG_EXT_ACKNOWLEDGED_DATA_ID            0x5E
#define MESG_EXT_BURST_DATA_ID                   0x5F

#define MESG_CHANNEL_RADIO_TX_POWER_ID           0x60
#define MESG_GET_SERIAL_NUM_ID                   0x61
#define MESG_GET_TEMP_CAL_ID                     0x62
#define MESG_SET_LP_SEARCH_TIMEOUT_ID            0x63
#define MESG_SET_TX_SEARCH_ON_NEXT_ID            0x64
#define MESG_SERIAL_NUM_SET_CHANNEL_ID_ID        0x65
#define MESG_RX_EXT_MESGS_ENABLE_ID              0x66  
#define MESG_RADIO_CONFIG_ALWAYS_ID              0x67
#define MESG_ENABLE_LED_FLASH_ID                 0x68
#define MESG_XTAL_ENABLE_ID                      0x6D
#define MESG_ANTLIB_CONFIG_ID                    0x6E
#define MESG_STARTUP_MESG_ID                     0x6F
#define MESG_AUTO_FREQ_CONFIG_ID                 0x70
#define MESG_PROX_SEARCH_CONFIG_ID               0x71

#define MESG_ADV_BURST_DATA_ID                   0x72
#define MESG_EVENT_BUFFERING_CONFIG_ID           0x74

#define MESG_SET_SEARCH_CH_PRIORITY_ID           0x75

#define MESG_HIGH_DUTY_SEARCH_MODE_ID            0x77
#define MESG_CONFIG_ADV_BURST_ID                 0x78
#define MESG_EVENT_FILTER_CONFIG_ID              0x79
#define MESG_SDU_CONFIG_ID                       0x7A
#define MESG_SDU_SET_MASK_ID                     0x7B
#define MESG_USER_CONFIG_PAGE_ID                 0x7C
#define MESG_ENCRYPT_ENABLE_ID                   0x7D
#define MESG_SET_CRYPTO_KEY_ID                   0x7E
#define MESG_SET_CRYPTO_INFO_ID                  0x7F
#define MESG_CUBE_CMD_ID                         0x80

#define MESG_ACTIVE_SEARCH_SHARING_ID            0x81
#define MESG_NVM_CRYPTO_KEY_OPS_ID               0x83

#define MESG_GET_PIN_DIODE_CONTROL_ID            0x8D
#define MESG_PIN_DIODE_CONTROL_ID                0x8E
#define MESG_FIT1_SET_AGC_ID                     0x8F

#define MESG_FIT1_SET_EQUIP_STATE_ID             0x91  // *** CONFLICT: w/ Sensrcore, Fit1 will never have sensrcore enabled

// Sensrcore Messages
#define MESG_SET_CHANNEL_INPUT_MASK_ID           0x90
#define MESG_SET_CHANNEL_DATA_TYPE_ID            0x91
#define MESG_READ_PINS_FOR_SECT_ID               0x92
#define MESG_TIMER_SELECT_ID                     0x93
#define MESG_ATOD_SETTINGS_ID                    0x94
#define MESG_SET_SHARED_ADDRESS_ID               0x95
#define MESG_ATOD_EXTERNAL_ENABLE_ID             0x96
#define MESG_ATOD_PIN_SETUP_ID                   0x97
#define MESG_SETUP_ALARM_ID                      0x98
#define MESG_ALARM_VARIABLE_MODIFY_TEST_ID       0x99
#define MESG_PARTIAL_RESET_ID                    0x9A
#define MESG_OVERWRITE_TEMP_CAL_ID               0x9B
#define MESG_SERIAL_PASSTHRU_SETTINGS_ID         0x9C

#define MESG_BIST_ID                             0xAA
#define MESG_UNLOCK_INTERFACE_ID                 0xAD
#define MESG_SERIAL_ERROR_ID                     0xAE
#define MESG_SET_ID_STRING_ID                    0xAF

#define MESG_PORT_GET_IO_STATE_ID                0xB4
#define MESG_PORT_SET_IO_STATE_ID                0xB5

#define MESG_RSSI_ID                             0xC0
#define MESG_RSSI_BROADCAST_DATA_ID              0xC1
#define MESG_RSSI_ACKNOWLEDGED_DATA_ID           0xC2
#define MESG_RSSI_BURST_DATA_ID                  0xC3
#define MESG_RSSI_SEARCH_THRESHOLD_ID            0xC4
#define MESG_SLEEP_ID                            0xC5
#define MESG_GET_GRMN_ESN_ID                     0xC6
#define MESG_SET_USB_INFO_ID                     0xC7

#define MESG_HCI_COMMAND_COMPLETE                0xC8

// 0xE0 - 0xEF reserved for extended ID
#define MESG_EXT_ID_0                            0xE0
#define MESG_EXT_ID_1                            0xE1
#define MESG_EXT_ID_2                            0xE2

// 0xE0 extended IDs
#define MESG_EXT_RESPONSE_ID                     0xE000

// 0xE1 extended IDs
#define MESG_EXT_REQUEST_ID                      0xE100

// 0xE2 extended IDs
#define MESG_FS_INIT_MEMORY_ID                   0xE200
#define MESG_FS_FORMAT_MEMORY_ID                 0xE201
#define MESG_FS_GET_USED_SPACE_ID                0xE202
#define MESG_FS_GET_FREE_SPACE_ID                0xE203
#define MESG_FS_FIND_FILE_INDEX_ID               0xE204
#define MESG_FS_DIRECTORY_READ_ABSOLUTE_ID       0xE205
#define MESG_FS_DIRECTORY_READ_ENTRY_ID          0xE206
#define MESG_FS_DIRECTORY_SAVE_ID                0xE207
#define MESG_FS_DIRECTORY_GET_SIZE_ID            0xE208
#define MESG_FS_DIRECTORY_REBUILD_ID             0xE209
#define MESG_FS_FILE_CREATE_ID                   0xE20A
#define MESG_FS_FILE_OPEN_ID                     0xE20B
#define MESG_FS_FILE_DELETE_ID                   0xE20C
#define MESG_FS_FILE_CLOSE_ID                    0xE20D
#define MESG_FS_FILE_READ_ABSOLUTE_ID            0xE20E
#define MESG_FS_FILE_READ_RELATIVE_ID            0xE20F
#define MESG_FS_FILE_WRITE_ABSOLUTE_ID           0xE210
#define MESG_FS_FILE_WRITE_RELATIVE_ID           0xE211
#define MESG_FS_FILE_SET_SPECIFIC_FLAGS_ID       0xE212
#define MESG_FS_FILE_GET_SIZE_ID                 0xE213
#define MESG_FS_FILE_GET_SPECIFIC_FILE_FLAGS_ID  0xE214
#define MESG_FS_FILE_GET_SIZE_IN_MEM_ID          0xE215
#define MESG_FS_DIRECTORY_READ_LOCK_ID           0xE216

#define MESG_FS_FILE_SET_GENERAL_FLAGS_ID        0xE21E
#define MESG_FS_DIRECTORY_WRITE_ABSOLUTE_ID      0xE21F
// reserved
#define MESG_MEMDEV_EEPROM_INIT_ID               0xE220
#define MESG_MEMDEV_FLASH_INIT_ID                0xE221
//reserved
#define MESG_FS_ANTFS_EVENT_ID                   0xE230
#define MESG_FS_ANTFS_OPEN_ID                    0xE231
#define MESG_FS_ANTFS_CLOSE_ID                   0xE232
#define MESG_FS_ANTFS_CONFIG_BEACON_ID           0xE233
#define MESG_FS_ANTFS_SET_AUTH_STRING_ID         0xE234
#define MESG_FS_ANTFS_SET_BEACON_STATE_ID        0xE235
#define MESG_FS_ANTFS_PAIR_RESPONSE_ID           0xE236
#define MESG_FS_ANTFS_SET_LINK_FREQ_ID           0xE237
#define MESG_FS_ANTFS_SET_BEACON_TIMEOUT_ID      0xE238
#define MESG_FS_ANTFS_SET_PAIRING_TIMEOUT_ID     0xE239
#define MESG_FS_ANTFS_REMOTE_FILE_CREATE_EN_ID   0xE23A
#define MESG_FS_ANTFS_GET_CMD_PIPE_ID            0xE23B
#define MESG_FS_ANTFS_SET_CMD_PIPE_ID            0xE23C
#define MESG_FS_SYSTEM_TIME_ID                   0xE23D
#define MESG_FS_ANTFS_SET_ANTFS_STATE_ID         0xE23E
// reserved
#define MESG_FS_CRYPTO_ADD_USER_KEY_INDEX_ID     0xE245
#define MESG_FS_CRYPTO_SET_USER_KEY_INDEX_ID     0xE246
#define MESG_FS_CRYPTO_SET_USER_KEY_VAL_ID       0xE247
// reserved
#define MESG_FS_FIT_FILE_INTEGRITY_CHECK_ID      0xE250


//////////////////////////////////////////////
// Message Sizes
//////////////////////////////////////////////
#define MESG_INVALID_SIZE                        0

#define MESG_VERSION_SIZE                        13
#define MESG_RESPONSE_EVENT_SIZE                 3
#define MESG_CHANNEL_STATUS_SIZE                 2

#define MESG_UNASSIGN_CHANNEL_SIZE               1
#define MESG_ASSIGN_CHANNEL_SIZE                 3
#define MESG_CHANNEL_ID_SIZE                     5
#define MESG_CHANNEL_MESG_PERIOD_SIZE            3
#define MESG_CHANNEL_SEARCH_TIMEOUT_SIZE         2
#define MESG_CHANNEL_RADIO_FREQ_SIZE             2
#define MESG_CHANNEL_RADIO_TX_POWER_SIZE         2
#define MESG_NETWORK_KEY_SIZE                    9
#define MESG_RADIO_TX_POWER_SIZE                 2
#define MESG_RADIO_CW_MODE_SIZE                  3
#define MESG_RADIO_CW_INIT_SIZE                  1
#define MESG_SYSTEM_RESET_SIZE                   1
#define MESG_OPEN_CHANNEL_SIZE                   1
#define MESG_CLOSE_CHANNEL_SIZE                  1
#define MESG_REQUEST_SIZE                        2
#define MESG_REQUEST_USER_NVM_SIZE               5

#define MESG_CAPABILITIES_SIZE                   8
#define MESG_STACKLIMIT_SIZE                     2

#define MESG_SCRIPT_DATA_SIZE                    10
#define MESG_SCRIPT_CMD_SIZE                     3

#define MESG_ID_LIST_ADD_SIZE                    6
#define MESG_ID_LIST_CONFIG_SIZE                 3
#define MESG_OPEN_RX_SCAN_SIZE                   1
#define MESG_EXT_CHANNEL_RADIO_FREQ_SIZE         3

#define MESG_RADIO_CONFIG_ALWAYS_SIZE            2
#define MESG_RX_EXT_MESGS_ENABLE_SIZE            2
#define MESG_SET_TX_SEARCH_ON_NEXT_SIZE          2
#define MESG_SET_LP_SEARCH_TIMEOUT_SIZE          2

#define MESG_SERIAL_NUM_SET_CHANNEL_ID_SIZE      3
#define MESG_ENABLE_LED_FLASH_SIZE               2
#define MESG_GET_SERIAL_NUM_SIZE                 4
#define MESG_GET_TEMP_CAL_SIZE                   4
#define MESG_CONFIG_ADV_BURST_SIZE               9
#define MESG_CONFIG_ADV_BURST_SIZE_EXT           12
#define MESG_ANTLIB_CONFIG_SIZE                  2
#define MESG_XTAL_ENABLE_SIZE                    1
#define MESG_STARTUP_MESG_SIZE                   1
#define MESG_AUTO_FREQ_CONFIG_SIZE               4
#define MESG_PROX_SEARCH_CONFIG_SIZE             2
#define MESG_EVENT_BUFFERING_CONFIG_SIZE         6
#define MESG_EVENT_FILTER_CONFIG_SIZE            3
#define MESG_HIGH_DUTY_SEARCH_MODE_SIZE          3
#define MESG_SDU_CONFIG_SIZE                     3
#define MESG_SDU_SET_MASK_SIZE                   9

#define MESG_USER_CONFIG_PAGE_SIZE               3

#define MESG_ENCRYPT_ENABLE_SIZE                 4
#define MESG_SET_CRYPTO_KEY_SIZE                 17
#define MESG_SET_CRYPTO_ID_SIZE                  5
#define MESG_SET_CRYPTO_USER_INFO_SIZE           20
#define MESG_SET_CRYPTO_RNG_SEED_SIZE            17
#define MESG_NVM_CRYPTO_KEY_LOAD_SIZE            3
#define MESG_NVM_CRYPTO_KEY_STORE_SIZE           18
#define MESG_CRYPTO_ID_LIST_ADD_SIZE             6
#define MESG_CRYPTO_ID_LIST_CONFIG_SIZE          3

#define MESG_GET_PIN_DIODE_CONTROL_SIZE          1
#define MESG_PIN_DIODE_CONTROL_ID_SIZE           2
#define MESG_FIT1_SET_EQUIP_STATE_SIZE           2
#define MESG_FIT1_SET_AGC_SIZE                   4

#define MESG_BIST_SIZE                           6
#define MESG_UNLOCK_INTERFACE_SIZE               1
#define MESG_SET_SHARED_ADDRESS_SIZE             3

#define MESG_GET_GRMN_ESN_SIZE                   5

#define MESG_PORT_SET_IO_STATE_SIZE              5


#define MESG_SLEEP_SIZE                          1


#define MESG_EXT_DATA_SIZE                       13

#define MESG_RSSI_SIZE                           5
#define MESG_RSSI_DATA_SIZE                      17
#define MESG_RSSI_RESPONSE_SIZE                  7
#define MESG_RSSI_SEARCH_THRESHOLD_SIZE          2

#define MESG_MEMDEV_EEPROM_INIT_SIZE             0x04
#define MESG_FS_INIT_MEMORY_SIZE                 0x01
#define MESG_FS_FORMAT_MEMORY_SIZE               0x05
#define MESG_FS_DIRECTORY_SAVE_SIZE              0x01
#define MESG_FS_DIRECTORY_REBUILD_SIZE           0x01
#define MESG_FS_FILE_DELETE_SIZE                 0x02
#define MESG_FS_FILE_CLOSE_SIZE                  0x02
#define MESG_FS_FILE_SET_SPECIFIC_FLAGS_SIZE     0x03
#define MESG_FS_DIRECTORY_READ_LOCK_SIZE         0x02
#define MESG_FS_SYSTEM_TIME_SIZE                 0x05
#define MESG_FS_CRYPTO_ADD_USER_KEY_INDEX_SIZE   0x34
#define MESG_FS_CRYPTO_SET_USER_KEY_INDEX_SIZE   0x05
#define MESG_FS_CRYPTO_SET_USER_KEY_VAL_SIZE     0x33
#define MESG_FS_FIT_FILE_INTEGRITY_CHECK_SIZE    0x02
#define MESG_FS_ANTFS_OPEN_SIZE                  0x01
#define MESG_FS_ANTFS_CLOSE_SIZE                 0x01
#define MESG_FS_ANTFS_CONFIG_BEACON_SIZE         0x09
#define MESG_FS_ANTFS_SET_AUTH_STRING_SIZE       0x02 
#define MESG_FS_ANTFS_SET_BEACON_STATE_SIZE      0x02
#define MESG_FS_ANTFS_PAIR_RESPONSE_SIZE         0x02
#define MESG_FS_ANTFS_SET_LINK_FREQ_SIZE         0x03
#define MESG_FS_ANTFS_SET_BEACON_TIMEOUT_SIZE    0x02
#define MESG_FS_ANTFS_SET_PAIRING_TIMEOUT_SIZE   0x02
#define MESG_FS_ANTFS_REMOTE_FILE_CREATE_EN_SIZE 0x02
#define MESG_FS_GET_USED_SPACE_SIZE              0x03
#define MESG_FS_GET_FREE_SPACE_SIZE              0x03
#define MESG_FS_FIND_FILE_INDEX_SIZE             0x07
#define MESG_FS_DIRECTORY_READ_ABSOLUTE_SIZE     0x08
#define MESG_FS_DIRECTORY_READ_ENTRY_SIZE        0x05
#define MESG_FS_DIRECTORY_GET_SIZE_SIZE          0x03
#define MESG_FS_FILE_CREATE_SIZE                 0x0B
#define MESG_FS_FILE_OPEN_SIZE                   0x06
#define MESG_FS_FILE_READ_ABSOLUTE_SIZE          0x09
#define MESG_FS_FILE_READ_RELATIVE_SIZE          0x05
#define MESG_FS_FILE_WRITE_ABSOLUTE_SIZE         0x09
#define MESG_FS_FILE_WRITE_RELATIVE_SIZE         0x05
#define MESG_FS_FILE_GET_SIZE_SIZE               0x04
#define MESG_FS_FILE_GET_SIZE_IN_MEM_SIZE        0x04
#define MESG_FS_FILE_GET_SPECIFIC_FILE_FLAGS_SIZE 0x04
#define MESG_FS_SYSTEM_TIME_REQUEST_SIZE         0x03
#define MESG_FS_ANTFS_GET_CMD_PIPE_SIZE          0x05
#define MESG_FS_ANTFS_SET_CMD_PIPE_SIZE          0x05
#define MESG_FS_REQUEST_RESPONSE_SIZE            0x03

#endif