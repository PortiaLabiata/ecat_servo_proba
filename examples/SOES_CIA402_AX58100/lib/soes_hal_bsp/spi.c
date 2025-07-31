#include "spi.h"

SPI_HandleTypeDef hspi1;

void spi_gpio_setup(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_SPI1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void spi_setup(void)
{
    spi_gpio_setup();
    
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 7;
    if (HAL_SPI_Init(&hspi1) != HAL_OK)
    {
        Error_Handler();
    }
}

void spi_select (int8_t board)
{
    // Soft CSM
    #if SCS_ACTIVE_POLARITY == SCS_LOW
    HAL_GPIO_WritePin(ESC_GPIOX_CS, ESC_GPIO_Pin_CS, GPIO_PIN_RESET);
    #elif SCS_ACTIVE_POLARITY == SCS_HIGH
    HAL_GPIO_WritePin(ESC_GPIOX_CS, ESC_GPIO_Pin_CS, GPIO_PIN_SET);
    #endif
}

void spi_unselect (int8_t board)
{
    #if SCS_ACTIVE_POLARITY == SCS_LOW
    HAL_GPIO_WritePin(ESC_GPIOX_CS, ESC_GPIO_Pin_CS, GPIO_PIN_SET);
    #elif SCS_ACTIVE_POLARITY == SCS_HIGH
    HAL_GPIO_WritePin(ESC_GPIOX_CS, ESC_GPIO_Pin_CS, GPIO_PIN_RESET);
    #endif
}

inline static uint8_t spi_transfer(uint8_t byte)
{
    uint8_t rec_byte = 0;
    HAL_SPI_TransmitReceive(&hspi1, &byte, &rec_byte, 1, HAL_MAX_DELAY);
    return rec_byte;
}

void spi_write (int8_t board, uint8_t *data, uint8_t size)
{
    for(int i = 0; i < size; ++i)
    {
        spi_transfer(data[i]);
    }
}

void spi_read (int8_t board, uint8_t *result, uint8_t size)
{
	for(int i = 0; i < size; ++i)
    {
        result[i] = spi_transfer(DUMMY_BYTE);
    }
}


void spi_bidirectionally_transfer (int8_t board, uint8_t *result, uint8_t *data, uint8_t size)
{
	for(int i = 0; i < size; ++i)
    {
        result[i] = spi_transfer(data[i]);
    }
}
