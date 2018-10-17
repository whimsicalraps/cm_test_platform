#pragma once

#include <stm32f7xx.h>

#include "interrupts.h"

// ADC
#define ADCx                            ADC1
#define ADCx_CLK_ENABLE()               __HAL_RCC_ADC1_CLK_ENABLE()
#define DMAx_CLK_ENABLE()               __HAL_RCC_DMA2_CLK_ENABLE()

#define ADCx_FORCE_RESET()              __HAL_RCC_ADC_FORCE_RESET()
#define ADCx_RELEASE_RESET()            __HAL_RCC_ADC_RELEASE_RESET()

#define ADC_GPIO_RCC1()		__HAL_RCC_GPIOA_CLK_ENABLE()
#define ADC_GPIO_RCC2()		__HAL_RCC_GPIOB_CLK_ENABLE()
#define ADC_GPIO_RCC3()		__HAL_RCC_GPIOC_CLK_ENABLE()

// Pin assignments
#define ADCx_CHANNEL_PIN0  GPIO_PIN_0
#define ADCx_CHANNEL_PORT0 GPIOA
#define ADCx_CHANNEL_PIN1  GPIO_PIN_1
#define ADCx_CHANNEL_PORT1 GPIOA
#define ADCx_CHANNEL_PIN2  GPIO_PIN_2
#define ADCx_CHANNEL_PORT2 GPIOA
#define ADCx_CHANNEL_PIN3  GPIO_PIN_3
#define ADCx_CHANNEL_PORT3 GPIOA
#define ADCx_CHANNEL_PIN4  GPIO_PIN_4
#define ADCx_CHANNEL_PORT4 GPIOA
#define ADCx_CHANNEL_PIN5  GPIO_PIN_5
#define ADCx_CHANNEL_PORT5 GPIOA
#define ADCx_CHANNEL_PIN6  GPIO_PIN_6
#define ADCx_CHANNEL_PORT6 GPIOA
#define ADCx_CHANNEL_PIN7  GPIO_PIN_7
#define ADCx_CHANNEL_PORT7 GPIOA

#define ADCx_CHANNEL_PIN8  GPIO_PIN_0
#define ADCx_CHANNEL_PORT8 GPIOB
#define ADCx_CHANNEL_PIN9  GPIO_PIN_1
#define ADCx_CHANNEL_PORT9 GPIOB

#define ADCx_CHANNEL_PIN10  GPIO_PIN_0
#define ADCx_CHANNEL_PORT10 GPIOC
#define ADCx_CHANNEL_PIN11  GPIO_PIN_1
#define ADCx_CHANNEL_PORT11 GPIOC
#define ADCx_CHANNEL_PIN12  GPIO_PIN_2
#define ADCx_CHANNEL_PORT12 GPIOC
#define ADCx_CHANNEL_PIN13  GPIO_PIN_3
#define ADCx_CHANNEL_PORT13 GPIOC
#define ADCx_CHANNEL_PIN14  GPIO_PIN_4
#define ADCx_CHANNEL_PORT14 GPIOC
#define ADCx_CHANNEL_PIN15  GPIO_PIN_5
#define ADCx_CHANNEL_PORT15 GPIOC

#define ADCx_DMA_CHANNEL                DMA_CHANNEL_0
#define ADCx_DMA_STREAM                 DMA2_Stream0

#define ADCx_DMA_IRQn                   DMA2_Stream0_IRQn
#define ADCx_DMA_IRQHandler             DMA2_Stream0_IRQHandler
//#define ADCx_DMA_IRQ_PRIORITY         defined-in-interrupts.h

#define ADC_VEC_AVG 32 //TODO refine this to match sampling rate
#define ADC_WIN_AVG 4

                            //< add comment slashes against tested vals
typedef enum{ Get_RAW       //
            , Get_RAWn      //
            , Get_amp_RAW   //
            , Get_amp_RAWn  //
            , Get_12        //
            , Get_12n       //
            , Get_5         //
            , Get_5n        //
            , Get_Left      //
            , Get_Right     //
            , Get_Crease    //
            , Get_Fullwave  //
            , Get_Location
            , Get_Follow    //
            , Get_Peak      //
            , Get_Trough    //
} ADC_t;

void LL_Adc_Init( void );

void ADCx_DMA_IRQHandler( void );

void LL_Adc_Process( void );
float LL_Adc_Get( ADC_t chan );
