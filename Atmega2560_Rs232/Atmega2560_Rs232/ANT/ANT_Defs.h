#ifndef ANT_DEFS_H_
#define ANT_DEFS_H_

#include "ANT_Message_Defs.h"

#define ANT_STANDARD_DATA_PAYLOAD_SIZE       ((unsigned char)8)
#define ANT_EXT_MESG_DEVICE_ID_FIELD_SIZE    ((unsigned char)4)
#define ANT_EXT_STRING_SIZE                  ((unsigned char)27)             // increase buffer size to support longer messages (32 extra bytes after ANT standard payload)

#define MESG_SYNC_OFFSET                     ((unsigned char)0)
#define MESG_TX_SYNC                         ((unsigned char)0xA4)
#define MESG_RX_SYNC                         ((unsigned char)0xA5)
#define MESG_SYNC_SIZE                       ((unsigned char)1)
#define MESG_SIZE_SIZE                       ((unsigned char)1)
#define MESG_ID_SIZE                         ((unsigned char)1)
#define MESG_CHANNEL_NUM_SIZE                ((unsigned char)1)
#define MESG_EXT_MESG_BF_SIZE                ((unsigned char)1)  // NOTE: this could increase in the future
#define MESG_CHECKSUM_SIZE                   ((unsigned char)1)
#define MESG_DATA_SIZE                       ((unsigned char)9)

#define MESG_SIZE_OFFSET                     (MESG_SYNC_OFFSET + MESG_SYNC_SIZE)
#define MESG_ID_OFFSET                       (MESG_SIZE_OFFSET + MESG_SIZE_SIZE)
#define MESG_DATA_OFFSET                     (MESG_ID_OFFSET + MESG_ID_SIZE)
#define MESG_CHECKSUM_OFFSET(MesgSize)       (MESG_ID_OFFSET + MESG_ID_SIZE + MesgSize)

#define MESG_SIZE_WITH_OUT_PAYLOAD           (MESG_SYNC_SIZE + MESG_SIZE_SIZE + MESG_ID_SIZE + MESG_CHECKSUM_SIZE)

// The largest serial message is an ANT data message with all of the extended fields
#define MESG_ANT_MAX_PAYLOAD_SIZE            ANT_STANDARD_DATA_PAYLOAD_SIZE

#define MESG_MAX_EXT_DATA_SIZE               (ANT_EXT_MESG_DEVICE_ID_FIELD_SIZE + ANT_EXT_STRING_SIZE) // ANT device ID (4 bytes) +  Padding for ANT EXT string size(27 bytes)

#define MESG_MAX_DATA_SIZE                   (MESG_ANT_MAX_PAYLOAD_SIZE + MESG_EXT_MESG_BF_SIZE + MESG_MAX_EXT_DATA_SIZE) // ANT data payload (8 bytes) + extended bitfield (1 byte) + extended data (31 bytes) = 40 bytes
#define MESG_MAX_SIZE_VALUE                  (MESG_MAX_DATA_SIZE + MESG_CHANNEL_NUM_SIZE)  // this is the maximum value that the serial message size value is allowed to be (40 + 1 = 41 bytes)

///////////////////////////////////////////////////////
// Reset Mesg Codes
///////////////////////////////////////////////////////
#define RESET_FLAGS_MASK                           0xE0
#define RESET_SUSPEND                              0x80              // this must follow bitfield def
#define RESET_SYNC                                 0x40              // this must follow bitfield def
#define RESET_CMD                                  0x20              // this must follow bitfield def
#define RESET_WDT                                  0x02
#define RESET_RST                                  0x01
#define RESET_POR                                  0x00

//////////////////////////////////////////////
// Response / Event Codes
//////////////////////////////////////////////
#define RESPONSE_NO_ERROR                          0x00
#define NO_EVENT                                   0x00

#define EVENT_RX_SEARCH_TIMEOUT                    0x01
#define EVENT_RX_FAIL                              0x02
#define EVENT_TX                                   0x03
#define EVENT_TRANSFER_RX_FAILED                   0x04
#define EVENT_TRANSFER_TX_COMPLETED                0x05
#define EVENT_TRANSFER_TX_FAILED                   0x06
#define EVENT_CHANNEL_CLOSED                       0x07
#define EVENT_RX_FAIL_GO_TO_SEARCH                 0x08
#define EVENT_CHANNEL_COLLISION                    0x09
#define EVENT_TRANSFER_TX_START                    0x0A           // a pending transmit transfer has begun

#define EVENT_CHANNEL_ACTIVE                       0x0F

#define EVENT_TRANSFER_TX_NEXT_MESSAGE             0x11           // only enabled in FIT1

#define CHANNEL_IN_WRONG_STATE                     0x15           // returned on attempt to perform an action from the wrong channel state
#define CHANNEL_NOT_OPENED                         0x16           // returned on attempt to communicate on a channel that is not open
#define CHANNEL_ID_NOT_SET                         0x18           // returned on attempt to open a channel without setting the channel ID
#define CLOSE_ALL_CHANNELS                         0x19           // returned when attempting to start scanning mode, when channels are still open

#define TRANSFER_IN_PROGRESS                       0x1F           // returned on attempt to communicate on a channel with a TX transfer in progress
#define TRANSFER_SEQUENCE_NUMBER_ERROR             0x20           // returned when sequence number is out of order on a Burst transfer
#define TRANSFER_IN_ERROR                          0x21
#define TRANSFER_BUSY                              0x22

#define INVALID_MESSAGE_CRC                        0x26           // returned if there is a framing error on an incomming message
#define MESSAGE_SIZE_EXCEEDS_LIMIT                 0x27           // returned if a data message is provided that is too large
#define INVALID_MESSAGE                            0x28           // returned when the message has an invalid parameter
#define INVALID_NETWORK_NUMBER                     0x29           // returned when an invalid network number is provided
#define INVALID_LIST_ID                            0x30           // returned when the provided list ID or size exceeds the limit
#define INVALID_SCAN_TX_CHANNEL                    0x31           // returned when attempting to transmit on channel 0 when in scan mode
#define INVALID_PARAMETER_PROVIDED                 0x33           // returned when an invalid parameter is specified in a configuration message

#define EVENT_SERIAL_QUE_OVERFLOW                  0x34
#define EVENT_QUE_OVERFLOW                         0x35           // ANT event que has overflowed and lost 1 or more events

#define EVENT_CLK_ERROR                            0x36           // debug event for XOSC16M on LE1
#define EVENT_STATE_OVERRUN                        0x37


#define SCRIPT_FULL_ERROR                          0x40           // error writing to script, memory is full
#define SCRIPT_WRITE_ERROR                         0x41           // error writing to script, bytes not written correctly
#define SCRIPT_INVALID_PAGE_ERROR                  0x42           // error accessing script page
#define SCRIPT_LOCKED_ERROR                        0x43           // the scripts are locked and can't be dumped

#define NO_RESPONSE_MESSAGE                        0x50           // returned to the Command_SerialMessageProcess function, so no reply message is generated
#define RETURN_TO_MFG                              0x51           // default return to any mesg when the module determines that the mfg procedure has not been fully completed

#define FIT_ACTIVE_SEARCH_TIMEOUT                  0x60           // Fit1 only event added for timeout of the pairing state after the Fit module becomes active
#define FIT_WATCH_PAIR                             0x61           // Fit1 only
#define FIT_WATCH_UNPAIR                           0x62           // Fit1 only

#define USB_STRING_WRITE_FAIL                      0x70

// Internal only events below this point
#define INTERNAL_ONLY_EVENTS                       0x80
#define EVENT_RX                                   0x80           // INTERNAL: Event for a receive message
#define EVENT_NEW_CHANNEL                          0x81           // INTERNAL: EVENT for a new active channel
#define EVENT_PASS_THRU                            0x82           // INTERNAL: Event to allow an upper stack events to pass through lower stacks

#define EVENT_BLOCKED                              0xFF           // INTERNAL: Event to replace any event we do not wish to go out, will also zero the size of the Tx message


//////////////////////////////////////////////
// ANT Extended Data Message Bifield Definitions
//////////////////////////////////////////////
#define ANT_EXT_MESG_BITFIELD_DEVICE_ID            0x80           // first field after bitfield

// 4 bits free reserved set to 0
#define ANT_EXT_MESG_BIFIELD_EXTENSION             0x01

// extended message input bitfield defines
#define ANT_EXT_MESG_BITFIELD_OVERWRITE_SHARED_ADR 0x10
#define ANT_EXT_MESG_BITFIELD_TRANSMISSION_TYPE    0x08  


//////////////////////////////////////////////
// ANT Library Config
//////////////////////////////////////////////
#define ANT_LIB_CONFIG_MASK_ALL                    0xFF
#define ANT_LIB_CONFIG_RADIO_CONFIG_ALWAYS         0x01
#define ANT_LIB_CONFIG_MESG_OUT_INC_TIME_STAMP     0x20
#define ANT_LIB_CONFIG_MESG_OUT_INC_RSSI           0x40
#define ANT_LIB_CONFIG_MESG_OUT_INC_DEVICE_ID      0x80

//////////////////////////////////////////////
// ID Definitions
//////////////////////////////////////////////
#define ANT_ID_SIZE                                4
#define ANT_ID_TRANS_TYPE_OFFSET                   3
#define ANT_ID_DEVICE_TYPE_OFFSET                  2
#define ANT_ID_DEVICE_NUMBER_HIGH_OFFSET           1
#define ANT_ID_DEVICE_NUMBER_LOW_OFFSET            0
#define ANT_ID_DEVICE_TYPE_PAIRING_FLAG            0x80

#define ANT_TRANS_TYPE_SHARED_ADDR_MASK            0x03
#define ANT_TRANS_TYPE_1_BYTE_SHARED_ADDRESS       0x02
#define ANT_TRANS_TYPE_2_BYTE_SHARED_ADDRESS       0x03


//////////////////////////////////////////////
// Assign Channel Parameters
//////////////////////////////////////////////
#define PARAMETER_RX_NOT_TX                        0x00
#define PARAMETER_TX_NOT_RX                        0x10
#define PARAMETER_SHARED_CHANNEL                   0x20
#define PARAMETER_NO_TX_GUARD_BAND                 0x40
#define PARAMETER_ALWAYS_RX_WILD_CARD_SEARCH_ID    0x40                 //Pre-AP2
#define PARAMETER_RX_ONLY                          0x40

//////////////////////////////////////////////
// Ext. Assign Channel Parameters
//////////////////////////////////////////////
#define EXT_PARAM_ALWAYS_SEARCH                    0x01
#define EXT_PARAM_FREQUENCY_AGILITY                0x04

//////////////////////////////////////////////
// Radio TX Power Definitions
//////////////////////////////////////////////
#define RADIO_TX_POWER_LVL_MASK                    0x03

#define RADIO_TX_POWER_LVL_0                       0x00                //(formerly: RADIO_TX_POWER_MINUS20DB) lowest
#define RADIO_TX_POWER_LVL_1                       0x01                //(formerly: RADIO_TX_POWER_MINUS10DB)
#define RADIO_TX_POWER_LVL_2                       0x02                //(formerly: RADIO_TX_POWER_MINUS5DB)
#define RADIO_TX_POWER_LVL_3                       0x03                //(formerly: RADIO_TX_POWER_0DB) highest

//////////////////////////////////////////////
// Channel Status
//////////////////////////////////////////////
#define STATUS_CHANNEL_STATE_MASK                  0x03
#define STATUS_UNASSIGNED_CHANNEL                  0x00
#define STATUS_ASSIGNED_CHANNEL                    0x01
#define STATUS_SEARCHING_CHANNEL                   0x02
#define STATUS_TRACKING_CHANNEL                    0x03

//////////////////////////////////////////////
// Standard capabilities defines
//////////////////////////////////////////////
#define CAPABILITIES_NO_RX_CHANNELS                0x01
#define CAPABILITIES_NO_TX_CHANNELS                0x02
#define CAPABILITIES_NO_RX_MESSAGES                0x04
#define CAPABILITIES_NO_TX_MESSAGES                0x08
#define CAPABILITIES_NO_ACKD_MESSAGES              0x10
#define CAPABILITIES_NO_BURST_TRANSFER             0x20

//////////////////////////////////////////////
// Advanced capabilities defines
//////////////////////////////////////////////
#define CAPABILITIES_OVERUN_UNDERRUN               0x01     // Support for this functionality has been dropped
#define CAPABILITIES_NETWORK_ENABLED               0x02
#define CAPABILITIES_AP1_VERSION_2                 0x04     // This Version of the AP1 does not support transmit and only had a limited release
#define CAPABILITIES_SERIAL_NUMBER_ENABLED         0x08
#define CAPABILITIES_PER_CHANNEL_TX_POWER_ENABLED  0x10
#define CAPABILITIES_LOW_PRIORITY_SEARCH_ENABLED   0x20
#define CAPABILITIES_SCRIPT_ENABLED                0x40
#define CAPABILITIES_SEARCH_LIST_ENABLED           0x80

//////////////////////////////////////////////
// Advanced capabilities 2 defines
//////////////////////////////////////////////
#define CAPABILITIES_LED_ENABLED                   0x01
#define CAPABILITIES_EXT_MESSAGE_ENABLED           0x02
#define CAPABILITIES_SCAN_MODE_ENABLED             0x04
#define CAPABILITIES_RESERVED                      0x08
#define CAPABILITIES_PROX_SEARCH_ENABLED           0x10
#define CAPABILITIES_EXT_ASSIGN_ENABLED            0x20
#define CAPABILITIES_FS_ANTFS_ENABLED              0x40
#define CAPABILITIES_FIT1_ENABLED                  0x80

//////////////////////////////////////////////
// Advanced capabilities 3 defines
//////////////////////////////////////////////
#define CAPABILITIES_ADVANCED_BURST_ENABLED              0x01
#define CAPABILITIES_EVENT_BUFFERING_ENABLED             0x02
#define CAPABILITIES_EVENT_FILTERING_ENABLED             0x04
#define CAPABILITIES_HIGH_DUTY_SEARCH_MODE_ENABLED       0x08
#define CAPABILITIES_SELECTIVE_DATA_UPDATE_ENABLED       0x40
#define CAPABILITIES_ENCRYPTED_CHANNEL_ENABLED           0x80


//////////////////////////////////////////////
// Burst Message Sequence
//////////////////////////////////////////////
#define CHANNEL_NUMBER_MASK                        0x1F
#define SEQUENCE_NUMBER_MASK                       0xE0
#define SEQUENCE_NUMBER_ROLLOVER                   0x60
#define SEQUENCE_FIRST_MESSAGE                     0x00
#define SEQUENCE_LAST_MESSAGE                      0x80
#define SEQUENCE_NUMBER_INC                        0x20

//////////////////////////////////////////////
// Advanced Burst Config defines
//////////////////////////////////////////////
#define ADV_BURST_CONFIG_FREQ_HOP                  0x00000001

//////////////////////////////////////////////
// Extended Message ID Mask
//////////////////////////////////////////////
#define MSG_EXT_ID_MASK                            0xE0

//////////////////////////////////////////////
// Control Message Flags
//////////////////////////////////////////////
#define BROADCAST_CONTROL_BYTE                     0x00
#define ACKNOWLEDGED_CONTROL_BYTE                  0xA0
#endif