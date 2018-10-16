#include "tests.h"

#include <stm32f7xx_hal.h>

#include "ll/debug_usart.h"
#include "hw_setup.h"

#define TOLERANCE (float)0.1

uint8_t Test_V( char*         name
              , Destination_t d
              , Volts_t       v
              , ADC_t         a
              , float         expect
              )
{
    uint8_t error = 0;
    U_Print("testing ");
    U_Print(name);
    U_Print(": ");
    SelectVoltage( d, v );
    HAL_Delay(1); // wait for stabilizing
    LL_Adc_Process(); // read the ADCs
    float result = GetVolts( a );
    if( result > (expect-TOLERANCE)
     && result < (expect+TOLERANCE) ){
        U_PrintLn("passed");
    } else {
        U_PrintLn("FAILED");
        U_Print("\texpected "); U_PrintF(expect);
        U_Print("\tbut got  "); U_PrintF(result);
        error = 1;
    }
    return error;
}

uint8_t Test_V_V( char* name
                )
{
    //
    return 0;
}

