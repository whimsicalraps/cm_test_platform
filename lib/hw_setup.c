#include "hw_setup.h"

#include <stm32f7xx_hal.h>
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

    LL_PowerOn_Set( 1 );
}

// runs in the main while(1) loop
void HW_Process( void )
{
    // call this "run tests"
    // TODO wrap this in a macro that prints error msg / hard faults
    Test_V( "+RAW", Set_NA  , Volt_0
                  , Get_RAW , 12.0
                  );
    HAL_Delay(100);
}

float GetVolts( ADC_t chan )
{
    //TODO: refine these per board (calibration stage?)
    const float volt_scalars[] = {  16.72  //Get_RAW
                                 , -14.59  //Get_RAWn
                                 ,  16.52  //Get_amp_RAW
                                 , -14.61  //Get_amp_RAWn
                                 ,  16.52  //Get_12
                                 ,  14.55  //Get_12n
                                 ,  6.58   //Get_5
                                 , -6.62   //Get_5n
                                 ,  6.62   //Get_Left
                                 ,  6.61   //Get_Right
                                 ,  6.62   //Get_Crease
                                 ,  6.60   //Get_Fullwave
                                 ,  6.63   //Get_Integral
                                 ,  6.60   //Get_Follow
                                 ,  6.60   //Get_Peak
                                 ,  6.60   //Get_Trough
                                 };
    return ((float)volt_scalars[chan] * LL_Adc_Get( chan ));
}
