#pragma once

//#define ONE_SENSE_IRQ_PRIORITY 0 // power-out detect (MAX!)
//#define I2Cx_Priority          1 // i2c. don't block the bus
//#define TIMx_IRQ_Priority      2 // light-framerate. higher than AUDIO
//#define AUDIO_DMA_IRQ_PRIORITY 3 // audio. most important!
//#define SDx_Priority           4
//#define ADCx_DMA_IRQ_PRIORITY  5 // need a constant stream (once per block)
#define USARTx_IRQ_PRIORITY    6 // debug. lowest
