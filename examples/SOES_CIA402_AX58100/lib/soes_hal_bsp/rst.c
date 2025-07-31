#include "rst.h"


void rst_setup(void)
{
    /* Setup NRST as GPIO out and pull it high */
    GPIO_InitTypeDef GPIO_InitStruct;
    __HAL_RCC_GPIOA_CLK_ENABLE();
 
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    rst_high();
}

void rst_low(void)
{    /* Set RSTN line low */
    HAL_GPIO_WritePin(ESC_GPIOX_RSTN, ESC_GPIO_Pin_RSTN, GPIO_PIN_RESET);
}

void rst_high(void)
{
    /* Set RSTN line high */
    HAL_GPIO_WritePin(ESC_GPIOX_RSTN, ESC_GPIO_Pin_RSTN, GPIO_PIN_SET);
}

void rst_check_start(void)
{
    /* Setup NRST as GPIO input and pull it high */
    GPIO_InitTypeDef GPIO_InitStruct;
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

uint8_t is_esc_reset(void)
{
    /* Check if ESC pulled RSTN line up */ 
    return HAL_GPIO_ReadPin(ESC_GPIOX_RSTN, ESC_GPIO_Pin_RSTN) == GPIO_PIN_SET;
}

