

#ifndef _system_init_user_h
#define _system_init_user_h


#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_it.h"
#include "stm32f10x_conf.h"
#include <stdio.h>
#include <string.h>

#define RxMax  1024

extern  int RxCounterFirst;
extern  int RxCounterLast;
extern  uint8_t RxBuffer[RxMax];
extern  unsigned int value1;
extern  unsigned int value2;
extern  unsigned int value;
extern  uint8_t  output;
extern  vu32 DelayTime;
extern  __IO uint16_t ADCConvertedValue[5];



extern void USART_init(void);
extern void NVIC_Configuration(void);
extern void SendString(USART_TypeDef* USARTn,uint8_t *p);
extern void TIM2_Init(void)	;
extern void GPIO_Config(unsigned int GPIOx, unsigned short Pin,unsigned char GPIO_Mode,unsigned char GPIO_Speed);
extern void RCC_Configuration(void);
extern void Delay(vu32 nCount);
extern void SysTick_Configuration(void);
extern void Delay_ms(uint32_t nCount) ;
extern void EXTI_init(void);
extern void DMA_init(void) ;
extern void ADC_init(void);
extern void Change_PWM(float value);
extern void MotorSteppingInit(void);
extern void MotorSteppingControl(uint8_t Dir);
extern void MotorSteppingDegree(float Degree,uint8_t Dir);



#endif 
