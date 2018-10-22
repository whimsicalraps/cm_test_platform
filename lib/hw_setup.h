#pragma once

#include <stm32f7xx.h>

#include "ll/state.h" //Switch_t

void HW_Init( void );

uint8_t HW_RunPress( void );
uint8_t HW_Press( Switch_t which );
