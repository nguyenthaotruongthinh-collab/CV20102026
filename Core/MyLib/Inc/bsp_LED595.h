#ifndef APPLICATION_USER_MYLIB_INC_BSP_LED595_H_
#define APPLICATION_USER_MYLIB_INC_BSP_LED595_H_

#include "main.h"
#include "spi.h"
#include "stm32f4xx_hal.h"

typedef enum {
	BSP_BLOCK_ID_DIGIT = 0,
	BSP_BLOCK_ID_SEG,
	BSP_BLOCK_ID_SINGLED1,
	BSP_BLOCK_ID_SINGLED2,
	BSP_BLOCK_ID_SINGLED3,
	BSP_BLOCK_ID_COUNT
} BSP_BlockID_t;

void bsp_LED595_Init(void);
void bsp_LED595_SpiTransmit(uint8_t *data, uint16_t len);
void bsp_LED595_PulseLatch(BSP_BlockID_t ID);

#endif /* APPLICATION_USER_MYLIB_INC_BSP_LED595_H_ */
