#include "led.h"

void LL_Led_Init( void )
{
    LED_RUN_GPIO_RCC();
    LED_1_GPIO_RCC();
    LED_2_GPIO_RCC();
    LED_3_GPIO_RCC();

 	GPIO_InitTypeDef gpio;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_NOPULL;

	gpio.Pin = LED_RUN_PIN;
	HAL_GPIO_Init( LED_RUN_GPIO, &gpio );
	gpio.Pin = LED_1_PIN;
	HAL_GPIO_Init( LED_1_GPIO, &gpio );
	gpio.Pin = LED_2_PIN;
	HAL_GPIO_Init( LED_2_GPIO, &gpio );
	gpio.Pin = LED_3_PIN;
	HAL_GPIO_Init( LED_3_GPIO, &gpio );
}

void LL_Led_Set( LED_t led, uint8_t state )
{
	const uint8_t st[2] = { GPIO_PIN_RESET
		                  , GPIO_PIN_SET
		                  };
	const GPIO_TypeDef* port[] = { LED_RUN_GPIO
		                         , LED_1_GPIO
		                         , LED_2_GPIO
		                         , LED_3_GPIO
							     };
	const uint32_t pin[] = { LED_RUN_PIN
		                   , LED_1_PIN
		                   , LED_2_PIN
		                   , LED_3_PIN
						   };

    // assign state to led
    HAL_GPIO_WritePin( (GPIO_TypeDef*)port[led]
                     , (uint32_t)pin[led]
                     , st[state]
                     );
}
