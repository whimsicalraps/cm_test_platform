#pragma once

#include <stm32f7xx.h>
#include "ll/adc.h"

void HW_Init( void );
void HW_Process( void );

float GetVolts( ADC_t chan );
