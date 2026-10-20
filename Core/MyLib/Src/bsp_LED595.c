#include "bsp_LED595.h"

extern SPI_HandleTypeDef hspi1;

static const GPIO_TypeDef* s_Block_ports[BSP_BLOCK_ID_COUNT] = {
	[BSP_BLOCK_ID_DIGIT]	= _o_Display_Digit_GPIO_Port,
	[BSP_BLOCK_ID_SEG]		= _o_Display_Segments_GPIO_Port,
	[BSP_BLOCK_ID_SINGLED1]	= _o_Display1_GPIO_Port,
	[BSP_BLOCK_ID_SINGLED2]	= _o_Display2_GPIO_Port,
	[BSP_BLOCK_ID_SINGLED3]	= _o_DisplayWarning_GPIO_Port
};

static const uint16_t s_Block_pins[BSP_BLOCK_ID_COUNT] = {
	[BSP_BLOCK_ID_DIGIT]	= _o_Display_Digit_Pin,
	[BSP_BLOCK_ID_SEG]		= _o_Display_Segments_Pin,
	[BSP_BLOCK_ID_SINGLED1]	= _o_Display1_Pin,
	[BSP_BLOCK_ID_SINGLED2]	= _o_Display2_Pin,
	[BSP_BLOCK_ID_SINGLED3]	= _o_DisplayWarning_Pin
};

static void SPI_Send(uint8_t *data, uint16_t len)
{
    HAL_SPI_Transmit(&hspi1, data, len, HAL_MAX_DELAY);
}

static void pulsePin(GPIO_TypeDef *port, uint16_t pin)
{
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
    for (volatile uint32_t i = 0U; i < 5U; i++) {
        __NOP();
    }
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
}

void bsp_LED595_Init(void){
	for (uint8_t i = 0; i < BSP_BLOCK_ID_COUNT; i++){
		HAL_GPIO_WritePin((GPIO_TypeDef*)s_Block_ports[i], s_Block_pins[i], GPIO_PIN_RESET);
	}
}

void bsp_LED595_SpiTransmit(uint8_t *data, uint16_t len)
{
	SPI_Send(data, len);
}

void bsp_LED595_PulseLatch(BSP_BlockID_t ID)
{
    pulsePin((GPIO_TypeDef*)s_Block_ports[ID], s_Block_pins[ID]);
}
