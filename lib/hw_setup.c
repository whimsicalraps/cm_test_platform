#include "hw_setup.h"

#include "ll/led.h"
#include "ll/state.h"
#include "ll/debug_usart.h"

void LL_Test( void );

void HW_Init( void )
{
    LL_Led_Init();
    LL_State_Init();
    //LL_ADC_Init();
    //

    LL_Test();
}

// runs in the main while(1) loop
void HW_Process( void )
{
    SelectVoltage( Dest_Flip, Volt_2 );
}

void LL_Test( void )
{
    //LL_Led_Set( LED_Running, LL_isSwitchDown(0) );
    //LL_Led_Set( LED_1, LL_isSwitchDown(1) );
    //LL_Led_Set( LED_2, LL_isSwitchDown(2) );
    //if( LL_isSwitchDown(0) && LL_isSwitchDown(1) ){
    //    LL_Led_Set( LED_3, 1 );
    //}

}
