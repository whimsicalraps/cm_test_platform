#pragma once

#include <stm32f7xx.h>

#define LED_RUN_GPIO_RCC()	__HAL_RCC_GPIOD_CLK_ENABLE()
#define LED_RUN_GPIO		GPIOD
#define LED_RUN_PIN			GPIO_PIN_8

#define LED_1_GPIO_RCC()	__HAL_RCC_GPIOD_CLK_ENABLE()
#define LED_1_GPIO		    GPIOD
#define LED_1_PIN			GPIO_PIN_10

#define LED_2_GPIO_RCC()	__HAL_RCC_GPIOD_CLK_ENABLE()
#define LED_2_GPIO		    GPIOD
#define LED_2_PIN			GPIO_PIN_11

#define LED_3_GPIO_RCC()	__HAL_RCC_GPIOD_CLK_ENABLE()
#define LED_3_GPIO		    GPIOD
#define LED_3_PIN			GPIO_PIN_12

typedef enum{ LED_Running
            , LED_1
            , LED_2
            , LED_3
} LED_t;

void LL_Led_Init( void );
void LL_Led_Set( LED_t led, uint8_t state );
