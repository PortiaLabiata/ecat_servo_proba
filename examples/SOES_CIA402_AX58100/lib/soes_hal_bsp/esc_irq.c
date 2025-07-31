#include "esc_irq.h"
#include <stdint.h>

void EXTILine1_Config(void)
{
    GPIO_InitTypeDef   GPIO_InitStruct;
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_SYSCFG_CLK_ENABLE();

    /* Configure PA1 pin as input floating */
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_NVIC_SetPriority(EXTI1_IRQn, 0x0F, 0x0F);
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);
}

void EXTILine1_Disable(void)
{
    HAL_NVIC_DisableIRQ(EXTI1_IRQn);
}

void EXTILine3_Config(void)
{
    GPIO_InitTypeDef   GPIO_InitStruct;

    /* Enable GPIOA clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /* Enable SYSCFG clock */
    __HAL_RCC_SYSCFG_CLK_ENABLE();

    /* Configure PA3 pin as input floating */
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_NVIC_SetPriority(EXTI3_IRQn, 0x0F, 0x0F);
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);
}

void EXTILine3_Disable(void)
{
    HAL_NVIC_DisableIRQ(EXTI3_IRQn);
}
