#ifndef __ADC_H
#define	__ADC_H

#include "stm32h7xx.h"


//���Ŷ���
#define RHEOSTAT_ADC_PIN                            GPIO_PIN_3                 
#define RHEOSTAT_ADC_GPIO_PORT                      GPIOC                     
#define RHEOSTAT_ADC_GPIO_CLK_ENABLE()              __GPIOC_CLK_ENABLE()

// ADC ��ź궨��
#define RHEOSTAT_ADC                        ADC3
#define RHEOSTAT_ADC_CLK_ENABLE()           __ADC3_CLK_ENABLE()
#define RHEOSTAT_ADC_CHANNEL                ADC_CHANNEL_1

// ADC �жϺ궨��
#define Rheostat_ADC_IRQ                    ADC3_IRQn
#define Rheostat_ADC_INT_FUNCTION           ADC3_IRQHandler

void Rheostat_Init(void);
#endif /* __ADC_H */
