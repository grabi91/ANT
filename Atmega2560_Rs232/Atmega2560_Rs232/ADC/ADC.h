/*
 * ADC.h
 *
 * Created: 2015-09-30 21:10:32
 *  Author: Lukasz
 */ 
#include "..\typedef.h"
#include "..\Debug_MSG\Debug_Msg.h"

#ifndef ADC_H_
#define ADC_H_

typedef uint8_t ADC_CHANNEL;
typedef uint16_t ADC_RESPONSE;

void ADC_Init();
STATUS ADC_Read(IN ADC_CHANNEL Channel, OUT ADC_RESPONSE *pADC);
STATUS ADC_ValueToVoltage(ADC_RESPONSE Adc, float *pVoltage);

#endif /* ADC_H_ */