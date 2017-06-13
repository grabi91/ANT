/*
 * typedef.h
 *
 * Created: 2015-08-25 19:18:10
 *  Author: Lukasz
 */
#ifndef WIN32
#include <avr/io.h>
#include <avr/interrupt.h>
#endif
#include <stddef.h>

#ifdef WIN32
#include <stdint.h>
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
	STATUS_NDEF_DMsg
} STATUS;

#ifdef WIN32
#define REGISTER_ADRESS_8(x)
#else
#define REGISTER_ADRESS_8(x) _SFR_MEM8(x) 
#endif

//typedef uint8_t STATUS;


#endif /* TYPEDEF_H_ */

