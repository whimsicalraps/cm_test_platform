#pragma once

#include "ll/state.h"
#include "ll/adc.h"

uint8_t Test_V( char*         name
              , Destination_t d
              , Volts_t       v
              , ADC_t         a
              , float         expect
              );
