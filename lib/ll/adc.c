#include "adc.h"

#include <stm32f7xx_hal.h> // HAL_Delay()
#include "debug_usart.h"
#include "wrFilter.h"      // &awin

#define ADC_CHANNEL_COUNT   16
#define ADC_VEC_SIZE        (ADC_CHANNEL_COUNT * ADC_VEC_AVG)

ADC_HandleTypeDef AdcHandle;
ADC_ChannelConfTypeDef sConfig;

filter_awin_t adc_smooth[ADC_CHANNEL_COUNT];
volatile uint16_t adc_ivector[ADC_VEC_SIZE];

void LL_Adc_Init( void )
{
	AdcHandle.Instance                   = ADCx;
	HAL_ADC_DeInit(&AdcHandle);

	AdcHandle.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV4;
	AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;
	AdcHandle.Init.ScanConvMode          = ENABLE;
	AdcHandle.Init.ContinuousConvMode    = ENABLE;
	AdcHandle.Init.DiscontinuousConvMode = DISABLE;
	AdcHandle.Init.NbrOfDiscConversion   = 0;
	AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
	AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;
	AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
	AdcHandle.Init.NbrOfConversion       = ADC_CHANNEL_COUNT;
	AdcHandle.Init.DMAContinuousRequests = ENABLE;
	AdcHandle.Init.EOCSelection          = ADC_EOC_SEQ_CONV;

	if( HAL_ADC_Init( &AdcHandle ) != HAL_OK ){
		U_PrintLn("HAL_ADC_Init failed");
	}

	// Channel configuration
	sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
	sConfig.Offset       = 0;

    sConfig.Channel      = ADC_CHANNEL_0;
	sConfig.Rank         = 1;
	if( HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK ){ U_PrintLn("CC failed"); }
	sConfig.Channel      = ADC_CHANNEL_1;
	sConfig.Rank         = 2;
	if( HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK ){ U_PrintLn("CC failed"); }
	sConfig.Channel      = ADC_CHANNEL_2;
	sConfig.Rank         = 3;
	if( HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK ){ U_PrintLn("CC failed"); }
	sConfig.Channel      = ADC_CHANNEL_3;
	sConfig.Rank         = 4;
	if( HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK ){ U_PrintLn("CC failed"); }
	sConfig.Channel      = ADC_CHANNEL_4;
	sConfig.Rank         = 5;
	if( HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK ){ U_PrintLn("CC failed"); }
	sConfig.Channel      = ADC_CHANNEL_5;
	sConfig.Rank         = 6;
	if( HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK ){ U_PrintLn("CC failed"); }
	sConfig.Channel      = ADC_CHANNEL_6;
	sConfig.Rank         = 7;
	if( HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK ){ U_PrintLn("CC failed"); }
	sConfig.Channel      = ADC_CHANNEL_7;
	sConfig.Rank         = 8;
	if( HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK ){ U_PrintLn("CC failed"); }
	sConfig.Channel      = ADC_CHANNEL_8;
	sConfig.Rank         = 9;
	if( HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK ){ U_PrintLn("CC failed"); }
	sConfig.Channel      = ADC_CHANNEL_9;
	sConfig.Rank         = 10;
	if( HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK ){ U_PrintLn("CC failed"); }
	sConfig.Channel      = ADC_CHANNEL_10;
	sConfig.Rank         = 11;
	if( HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK ){ U_PrintLn("CC failed"); }
	sConfig.Channel      = ADC_CHANNEL_11;
	sConfig.Rank         = 12;
	if( HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK ){ U_PrintLn("CC failed"); }
	sConfig.Channel      = ADC_CHANNEL_12;
	sConfig.Rank         = 13;
	if( HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK ){ U_PrintLn("CC failed"); }
	sConfig.Channel      = ADC_CHANNEL_13;
	sConfig.Rank         = 14;
	if( HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK ){ U_PrintLn("CC failed"); }
	sConfig.Channel      = ADC_CHANNEL_14;
	sConfig.Rank         = 15;
	if( HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK ){ U_PrintLn("CC failed"); }
	sConfig.Channel      = ADC_CHANNEL_15;
	sConfig.Rank         = 16;
	if( HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK ){ U_PrintLn("CC failed"); }


	// zero buffer (?)
	for( uint32_t i=0; i<ADC_VEC_SIZE; i++ ){
		adc_ivector[i] = 0;
	}

	// Init window averagers (only 4(*24samps) )
    for( int i=0; i<ADC_CHANNEL_COUNT; i++ ){
        awin_init( &adc_smooth[i], ADC_WIN_AVG );
    }

	HAL_Delay(1);
	if( HAL_ADC_Start_DMA( &AdcHandle
		                 , (uint32_t*)adc_ivector
		                 , ADC_VEC_SIZE
		                 ) != HAL_OK ){
		U_PrintLn("HAL_ADC_Start_DMA failed");
		// retry
		HAL_Delay(10);
		HAL_ADC_Start_DMA( &AdcHandle
		                 , (uint32_t*)adc_ivector
		                 , ADC_VEC_SIZE );
	}
}
void ADC_DeInit( void )
{
    HAL_ADC_Stop_DMA( &AdcHandle );
	HAL_ADC_DeInit( &AdcHandle );
}

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
	GPIO_InitTypeDef          gpio;
	static DMA_HandleTypeDef  hdma_adc;

	ADCx_CLK_ENABLE();
	DMAx_CLK_ENABLE();
    ADC_GPIO_RCC1();
    ADC_GPIO_RCC2();
    ADC_GPIO_RCC3();

	gpio.Mode = GPIO_MODE_ANALOG;
	gpio.Pull = GPIO_NOPULL;

	gpio.Pin  = ADCx_CHANNEL_PIN0;
	HAL_GPIO_Init(ADCx_CHANNEL_PORT0, &gpio);
	gpio.Pin  = ADCx_CHANNEL_PIN1;
	HAL_GPIO_Init(ADCx_CHANNEL_PORT1, &gpio);
	gpio.Pin  = ADCx_CHANNEL_PIN2;
	HAL_GPIO_Init(ADCx_CHANNEL_PORT2, &gpio);
	gpio.Pin  = ADCx_CHANNEL_PIN3;
	HAL_GPIO_Init(ADCx_CHANNEL_PORT3, &gpio);
	gpio.Pin  = ADCx_CHANNEL_PIN4;
	HAL_GPIO_Init(ADCx_CHANNEL_PORT4, &gpio);
	gpio.Pin  = ADCx_CHANNEL_PIN5;
	HAL_GPIO_Init(ADCx_CHANNEL_PORT5, &gpio);
	gpio.Pin  = ADCx_CHANNEL_PIN6;
	HAL_GPIO_Init(ADCx_CHANNEL_PORT6, &gpio);
	gpio.Pin  = ADCx_CHANNEL_PIN7;
	HAL_GPIO_Init(ADCx_CHANNEL_PORT7, &gpio);
	gpio.Pin  = ADCx_CHANNEL_PIN8;
	HAL_GPIO_Init(ADCx_CHANNEL_PORT8, &gpio);
	gpio.Pin  = ADCx_CHANNEL_PIN9;
	HAL_GPIO_Init(ADCx_CHANNEL_PORT9, &gpio);
	gpio.Pin  = ADCx_CHANNEL_PIN10;
	HAL_GPIO_Init(ADCx_CHANNEL_PORT10, &gpio);
	gpio.Pin  = ADCx_CHANNEL_PIN11;
	HAL_GPIO_Init(ADCx_CHANNEL_PORT11, &gpio);
	gpio.Pin  = ADCx_CHANNEL_PIN12;
	HAL_GPIO_Init(ADCx_CHANNEL_PORT12, &gpio);
	gpio.Pin  = ADCx_CHANNEL_PIN13;
	HAL_GPIO_Init(ADCx_CHANNEL_PORT13, &gpio);
	gpio.Pin  = ADCx_CHANNEL_PIN14;
	HAL_GPIO_Init(ADCx_CHANNEL_PORT14, &gpio);
	gpio.Pin  = ADCx_CHANNEL_PIN15;
	HAL_GPIO_Init(ADCx_CHANNEL_PORT15, &gpio);

    hdma_adc.Instance = ADCx_DMA_STREAM;

	hdma_adc.Init.Channel  				= ADCx_DMA_CHANNEL;
	hdma_adc.Init.Direction 			= DMA_PERIPH_TO_MEMORY;
	hdma_adc.Init.PeriphInc 			= DMA_PINC_DISABLE;
	hdma_adc.Init.MemInc 				= DMA_MINC_ENABLE;
	hdma_adc.Init.PeriphDataAlignment 	= DMA_PDATAALIGN_HALFWORD;
	hdma_adc.Init.MemDataAlignment 		= DMA_MDATAALIGN_HALFWORD;
	hdma_adc.Init.Mode 					= DMA_CIRCULAR;
	hdma_adc.Init.Priority 				= DMA_PRIORITY_HIGH;
	hdma_adc.Init.FIFOMode 				= DMA_FIFOMODE_DISABLE;
	hdma_adc.Init.FIFOThreshold 		= DMA_FIFO_THRESHOLD_HALFFULL;
	hdma_adc.Init.MemBurst 				= DMA_MBURST_SINGLE;
	hdma_adc.Init.PeriphBurst 			= DMA_PBURST_SINGLE;

	HAL_DMA_DeInit(&hdma_adc);
	HAL_DMA_Init(&hdma_adc);

	__HAL_LINKDMA(hadc, DMA_Handle, hdma_adc);

    HAL_NVIC_SetPriority( ADCx_DMA_IRQn
                        , ADCx_DMA_IRQ_PRIORITY
                        , 0
                        );
    HAL_NVIC_EnableIRQ( ADCx_DMA_IRQn );
}
void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
{
	ADCx_FORCE_RESET();
	ADCx_RELEASE_RESET();
}
void ADCx_DMA_IRQHandler(void)
{
    // actually not using this interrupt in our program
	HAL_DMA_IRQHandler(AdcHandle.DMA_Handle);
}

float LL_Adc_Get( ADC_t chan )
{
    const int adc_chan_mapping[] = { 2 //ADC_RAW
                                   , 0 //ADC_RAWn
                                   , 3 //ADC_amp_RAW
                                   , 1 //ADC_amp_RAWn
                                   , 9 //ADC_12
                                   , 5 //ADC_12n
                                   , 8 //ADC_5
                                   , 7 //ADC_5n
                                   , 6  //ADC_Left
                                   , 4  //ADC_Right
                                   , 10 //ADC_Crease
                                   , 11 //ADC_Fullwave
                                   , 15 //ADC_Integral
                                   , 14 //ADC_Follow
                                   , 13 //ADC_Peak
                                   , 12 //ADC_Trough
                                   };
    return (1.0 - 2.0 * awin_get_out( &adc_smooth[adc_chan_mapping[chan]] ));
}
// add smoothing & return float here
#define i12bit (1.0/(float)(1<<12))
float find_averages( volatile uint16_t* adc_buf )
{
	const float scaler = i12bit / (float)ADC_VEC_AVG;
	float avg = 0.0;
	for( uint8_t a=0; a<ADC_VEC_AVG; a++ ){
		avg += scaler * (float)adc_buf[a * ADC_CHANNEL_COUNT];
	}
	return avg;
}

void LL_Adc_Process( void )
{
    for( uint8_t i=0; i<ADC_CHANNEL_COUNT; i++ ){
        awin_step( &adc_smooth[i]
                 , find_averages( &adc_ivector[i] )
                 );
    }
}
