#pragma once

#include <stm32f7xx.h>

// TODO this should be at /lib level not /lib/ll

// Relay control
#define STATE_PWR_GPIO_RCC()	__HAL_RCC_GPIOB_CLK_ENABLE()
#define STATE_PWR_GPIO		    GPIOB
#define STATE_PWR_PIN			GPIO_PIN_9

// 4 MUX chips
#define STATE_SRC1_GPIO_RCC()	__HAL_RCC_GPIOD_CLK_ENABLE()
#define STATE_SRC1_GPIO		    GPIOD
#define STATE_SRC1_PIN0			GPIO_PIN_3
#define STATE_SRC1_PIN1			GPIO_PIN_4
#define STATE_SRC1_PIN2			GPIO_PIN_5

#define STATE_SRC2_GPIO_RCC()	__HAL_RCC_GPIOB_CLK_ENABLE()
#define STATE_SRC2_GPIO		    GPIOB
#define STATE_SRC2_PIN0			GPIO_PIN_12
#define STATE_SRC2_PIN1			GPIO_PIN_13
#define STATE_SRC2_PIN2			GPIO_PIN_14

#define STATE_DST1_GPIO_RCC()	__HAL_RCC_GPIOD_CLK_ENABLE()
#define STATE_DST1_GPIO		    GPIOD
#define STATE_DST1_PIN0			GPIO_PIN_0
#define STATE_DST1_PIN1			GPIO_PIN_1
#define STATE_DST1_PIN2			GPIO_PIN_2

#define STATE_DST2_GPIO_RCC()	__HAL_RCC_GPIOE_CLK_ENABLE()
#define STATE_DST2_GPIO		    GPIOE
#define STATE_DST2_PIN0			GPIO_PIN_9
#define STATE_DST2_PIN1			GPIO_PIN_10
#define STATE_DST2_PIN2			GPIO_PIN_11

// UI Switches
#define STATE_SW_GPIO_RCC()	__HAL_RCC_GPIOD_CLK_ENABLE()
#define STATE_SW_GPIO		    GPIOD
#define STATE_SW1_PIN			GPIO_PIN_9
#define STATE_SW2_PIN			GPIO_PIN_13
#define STATE_SW3_PIN			GPIO_PIN_15

// change this to directly refer to functional names
typedef enum{ Set_Left
            , Set_Right
            , Set_NA
            , Set_Peak1
            , Set_Trough
            , Set_Flip
            , Set_NA1
            , Set_NA2
                , Set_ENDOFCHANONE
            , Set_XVCAOut
            , Set_Peak2
            , Set_Trough2
            , Set_Fold
} Destination_t;

typedef enum{ Volt_5
            , Volt_4
            , Volt_3
            , Volt_2
            , Volt_1
            , Volt_0
            , Volt_n2v5
            , Volt_n5
} Volts_t;

// LL
typedef enum{ Chan_Source1
            , Chan_Source2
            , Chan_Dest1
            , Chan_Dest2
} Chan_t;

void SelectVoltage( Destination_t d, Volts_t v );

void LL_State_Init( void );
void LL_PowerOn_Set( uint8_t state );
void LL_DigiSwitch_Set( Chan_t ch, uint8_t state );
uint8_t LL_isSwitchDown( uint8_t which );
