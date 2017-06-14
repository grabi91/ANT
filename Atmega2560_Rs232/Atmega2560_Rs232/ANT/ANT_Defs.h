#ifndef ANT_DEFS_H_
#define ANT_DEFS_H_

#define ANT_STANDARD_DATA_PAYLOAD_SIZE             ((unsigned char)8)
#define ANT_EXT_MESG_DEVICE_ID_FIELD_SIZE          ((unsigned char)4)
#define ANT_EXT_STRING_SIZE                     ((unsigned char)27)             // increase buffer size to support longer messages (32 extra bytes after ANT standard payload)

#define MESG_TX_SYNC                         ((unsigned char)0xA4)
#define MESG_RX_SYNC                         ((unsigned char)0xA5)
#define MESG_SYNC_SIZE                       ((unsigned char)1)
#define MESG_SIZE_SIZE                       ((unsigned char)1)
#define MESG_ID_SIZE                         ((unsigned char)1)
#define MESG_CHANNEL_NUM_SIZE                ((unsigned char)1)
#define MESG_EXT_MESG_BF_SIZE                ((unsigned char)1)  // NOTE: this could increase in the future
#define MESG_CHECKSUM_SIZE                   ((unsigned char)1)
#define MESG_DATA_SIZE                       ((unsigned char)9)

// The largest serial message is an ANT data message with all of the extended fields
#define MESG_ANT_MAX_PAYLOAD_SIZE            ANT_STANDARD_DATA_PAYLOAD_SIZE

#define MESG_MAX_EXT_DATA_SIZE               (ANT_EXT_MESG_DEVICE_ID_FIELD_SIZE + ANT_EXT_STRING_SIZE) // ANT device ID (4 bytes) +  Padding for ANT EXT string size(27 bytes)

#define MESG_MAX_DATA_SIZE                   (MESG_ANT_MAX_PAYLOAD_SIZE + MESG_EXT_MESG_BF_SIZE + MESG_MAX_EXT_DATA_SIZE) // ANT data payload (8 bytes) + extended bitfield (1 byte) + extended data (31 bytes) = 40 bytes
#define MESG_MAX_SIZE_VALUE                  (MESG_MAX_DATA_SIZE + MESG_CHANNEL_NUM_SIZE)  // this is the maximum value that the serial message size value is allowed to be (40 + 1 = 41 bytes)


#endif