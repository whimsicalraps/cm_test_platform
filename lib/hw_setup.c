#include "hw_setup.h"

#include "ll/adc.h"
#include "ll/led.h"
#include "ll/debug_usart.h"
#include "tests.h"

void HW_Init( void )
{
    LL_Led_Init();
    LL_State_Init();
    LL_Adc_Init();
}

uint8_t HW_RunPress( void )
{
    static uint8_t state = 0;
    uint8_t new_state = LL_isSwitchDown(Sw_Run);

    if(new_state != state){
        state = new_state;
        if(state == 1){ return 1; }
    }
    return 0;
}

uint8_t HW_Press( Switch_t which )
{
    static uint8_t state[3];
    uint8_t new_state = LL_isSwitchDown(which);
    if(new_state != state[which]){
        state[which] = new_state;
        if(state[which]){ return 1; }
    }
    return 0;
}
