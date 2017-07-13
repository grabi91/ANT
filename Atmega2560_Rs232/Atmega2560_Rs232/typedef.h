/*
 * typedef.h
 *
 * Created: 2015-08-25 19:18:10
 *  Author: Lukasz
 */
#ifndef WIN32
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>
#endif

#ifdef WIN32
#include <stdint.h>
#include <stdio.h>
#endif

#ifndef TYPEDEF_H_
#define TYPEDEF_H_

#define F_CPU 16000000
#define BAUD 9600

#define IN
#define OUT
#define INOUT

//define STATUS_SUCCES 0
//#define STATUS_FAILURE 1

typedef enum _STATUS{
   STATUS_SUCCESS,
   STATUS_FAILURE,
   STATUS_NDEF_DMsg,
   STATUS_MESSEGE_FUNCTION_NOT_DEFINE,
   STATUS_MESSEGE_FUNCTION_ALREADY_DEFINE,
   STATUS_DATA_BUFFER_TO_SHORT,
   STATUS_PAGE_UNSUPPORTED,
} STATUS;

#ifdef WIN32
#define REGISTER_ADRESS_8(x)
#else
#define REGISTER_ADRESS_8(x) _SFR_MEM8(x) 
#endif

//typedef uint8_t STATUS;

#define USER_BAUDRATE         (50000)  // For AT3/AP2, use 57600
#define USER_RADIOFREQ        (57)

#define USER_ANTCHANNEL       (0)
#define USER_DEVICENUM        (100)
#define USER_DEVICETYPE       (25)
#define USER_TRANSTYPE        (5)

#define USER_NETWORK_KEY      {0xB9, 0xA5, 0x21, 0xFB, 0xBD, 0x72, 0xC3, 0x45}
#define USER_NETWORK_KEY_SIZE (8)
#define USER_NETWORK_NUM      (0)      // The network key is assigned to this network number

#define BYTE_MASK 0x00FF
#define UPPER_BYTE_MASK 0xFF00
#define UNIBBLE_UBYTE_SIGNXTND 0xF000
#define UPPER_NIBBLE_MASK 0xF0
#define LOWER_NIBBLE_MASK 0x0F

#endif /* TYPEDEF_H_ */

