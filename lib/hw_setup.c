#include "hw_setup.h"

#include "ll/adc.h"
#include "ll/led.h"
#include "ll/state.h"
#include "ll/debug_usart.h"
#include "tests.h"

void HW_Init( void )
{
    LL_Led_Init();
    LL_State_Init();
    LL_Adc_Init();

}

