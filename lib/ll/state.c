#include "state.h"

void LL_State_Init( void )
{
    STATE_PWR_GPIO_RCC();
    STATE_SRC1_GPIO_RCC();
    STATE_SRC2_GPIO_RCC();
    STATE_DST1_GPIO_RCC();
    STATE_DST2_GPIO_RCC();
    STATE_SW_GPIO_RCC();

 	GPIO_InitTypeDef gpio;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Pin  = STATE_PWR_PIN;
	HAL_GPIO_Init( STATE_PWR_GPIO, &gpio );

    gpio.Pin = STATE_SRC1_PIN0
             | STATE_SRC1_PIN1
             | STATE_SRC1_PIN2
             ;
	HAL_GPIO_Init( STATE_SRC1_GPIO, &gpio );

    gpio.Pin = STATE_SRC2_PIN0
             | STATE_SRC2_PIN1
             | STATE_SRC2_PIN2
             ;
	HAL_GPIO_Init( STATE_SRC2_GPIO, &gpio );

    gpio.Pin = STATE_DST1_PIN0
             | STATE_DST1_PIN1
             | STATE_DST1_PIN2
             ;
	HAL_GPIO_Init( STATE_DST1_GPIO, &gpio );

    gpio.Pin = STATE_DST2_PIN0
             | STATE_DST2_PIN1
             | STATE_DST2_PIN2
             ;
	HAL_GPIO_Init( STATE_DST2_GPIO, &gpio );

	gpio.Mode = GPIO_MODE_INPUT;
	gpio.Pull = GPIO_PULLUP;
    gpio.Pin = STATE_SW1_PIN
             | STATE_SW2_PIN
             | STATE_SW3_PIN
             ;
	HAL_GPIO_Init( STATE_SW_GPIO, &gpio );
}

void SelectVoltage( Destination_t d, Volts_t v )
{
    if( d < Set_ENDOFCHANONE ){
        LL_DigiSwitch_Set( Chan_Source1, (uint8_t)v );
        LL_DigiSwitch_Set( Chan_Dest1, (uint8_t)d );
    } else {
        LL_DigiSwitch_Set( Chan_Source2, (uint8_t)v );
        LL_DigiSwitch_Set( Chan_Dest2, (uint8_t)(d-Set_ENDOFCHANONE-1) );
    }
}

// Actual low-level stuff
void LL_PowerOn_Set( uint8_t state )
{
	const uint8_t st[2] = { GPIO_PIN_RESET
		                  , GPIO_PIN_SET
		                  };
    // TODO might need to invert this logic
    HAL_GPIO_WritePin( STATE_PWR_GPIO
                     , STATE_PWR_PIN
                     , st[state]
                     );
}

void LL_DigiSwitch_Set( Chan_t ch, uint8_t state )
{
	const uint8_t st[2] = { GPIO_PIN_RESET
		                  , GPIO_PIN_SET
		                  };
    const GPIO_TypeDef* port[] = { STATE_SRC1_GPIO
                                 , STATE_SRC2_GPIO
                                 , STATE_DST1_GPIO
                                 , STATE_DST2_GPIO
                                 };
    const uint32_t pin[4][3] = { { STATE_SRC1_PIN0
                                 , STATE_SRC1_PIN1
                                 , STATE_SRC1_PIN2
                                 }
                               , { STATE_SRC2_PIN0
                                 , STATE_SRC2_PIN1
                                 , STATE_SRC2_PIN2
                                 }
                               , { STATE_DST1_PIN0
                                 , STATE_DST1_PIN1
                                 , STATE_DST1_PIN2
                                 }
                               , { STATE_DST2_PIN0
                                 , STATE_DST2_PIN1
                                 , STATE_DST2_PIN2
                                 }
                               };
    for( int i=0; i<3; i++ ){
        HAL_GPIO_WritePin( (GPIO_TypeDef*)port[ch]
                         , (uint32_t)pin[ch][i]
                         , st[0x1 & (state>>i)]
                         );
    }
}

uint8_t LL_isSwitchDown( uint8_t which )
{
    const uint32_t pin[] = { STATE_SW1_PIN
                           , STATE_SW2_PIN
                           , STATE_SW3_PIN
                           };
    return !HAL_GPIO_ReadPin( STATE_SW_GPIO
                            , pin[which]
                            );
}
