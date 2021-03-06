// includes
#include "main.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// low-level abstractions
#include "lib/ll/debug_usart.h"
#include "lib/ll/led.h"
#include "lib/hw_setup.h"
#include "lib/tests.h"

#define FRAMERATE 0x1FFFF

// private fn prototypes
static void LEDs_Off(void);

static void SystemClock_Config(void);
static void Error_Handler(void);
static void MPU_Config(void);
static void CPU_CACHE_Enable(void);


// exported fns
int main(void)
{
	// Configure low-level
	MPU_Config();
	CPU_CACHE_Enable();
	HAL_Init();
	SystemClock_Config();

	// HW initialization
    Debug_USART_Init();
    U_PrintLn("\n  CM_test_platform\n");

    HW_Init();

	U_PrintNow();  // print any queued debug messages
    int fr = FRAMERATE;
	while(1){
        //TODO add the 'presence' check here
        if( HW_Press( Sw_Run ) ){
            LEDs_Off();
            LL_Led_Set( LED_Running, 1);
            uint8_t error = Test_RunSuite();
            if( !error ){
                U_PrintLn("Success");
                LEDs_Off();
            } else {
                if( error & 0x1 ){
                    U_PrintLn("Power System Failure");
                    LEDs_Off();
                    LL_Led_Flash( LED_1, 1 ); // TODO BLINK
                }
                if( error & 0x2 ){
                    U_PrintLn("Voltage test failure");
                    LEDs_Off();
                    LL_Led_Flash( LED_2, 1 ); // TODO BLINK
                }
                if( error & 0x4 ){
                    U_PrintLn("Timing test failure");
                    LEDs_Off();
                    LL_Led_Flash( LED_3, 1 ); // TODO BLINK
                }
            }
            LL_Led_Set( LED_Running, 0);
            U_PrintLn("\n\n\n");
        }
        if(--fr <= 0){
            LL_Led_Step();
            fr = FRAMERATE;
        }
        if( HW_Press( Sw_Left ) ){
            LEDs_Off();
        }

	    U_PrintNow();  // print any queued debug messages
    }
	return 0;
}

static void LEDs_Off(void)
{
    LL_Led_Set( LED_1, 0 );
    LL_Led_Set( LED_2, 0 );
    LL_Led_Set( LED_3, 0 );
    LL_Led_Flash( LED_1, 0 );
    LL_Led_Flash( LED_2, 0 );
    LL_Led_Flash( LED_3, 0 );
}

// LOW LEVEL SYS INIT
static void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;
	HAL_StatusTypeDef ret = HAL_OK;
	
	// Enable Power Control clock
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

	// Enable HSE Oscillator and activate PLL with HSE as source
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 432;  // for 216MHz use 432 !!!!!!!!!!
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 9;
	//RCC_OscInitStruct.PLL.PLLR = 7;

	ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
	if(ret != HAL_OK) { Error_Handler(); }

	// Activate the OverDrive to reach the 216 MHz Frequency
	ret = HAL_PWREx_EnableOverDrive();
	if(ret != HAL_OK) { Error_Handler(); }

	// Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4; //TODO change to 2?
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2; // 1?

	ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
	if(ret != HAL_OK) { Error_Handler(); }
}

static void Error_Handler(void)
{
	while(1) {
		;;
	}
}

static void MPU_Config(void)
{
	MPU_Region_InitTypeDef MPU_InitStruct;
	
	// Disable the MPU
	HAL_MPU_Disable();

	// Configure the MPU attributes as WT for SRAM
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress = 0x20020000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
	MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER0;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	// Enable the MPU
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

static void CPU_CACHE_Enable(void)
{
	// Enable I-Cache
	SCB_EnableICache();

	// Enable D-Cache
	SCB_EnableDCache();
}

#ifdef  USE_FULL_ASSERT
// Reports the name of the source file and the source line number
void assert_failed(uint8_t* file, uint32_t line)
{ 
	// printf the failure: "error in %file at line %line"
	// Infinite loop (just freezing now)
	while (1) {
		;;
	}
}
#endif
