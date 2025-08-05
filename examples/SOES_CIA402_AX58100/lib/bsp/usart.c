#include "usart.h"


UART_HandleTypeDef huart1;

int __io_putchar(int c) {
  //while (__HAL_USART_GET_FLAG(&huart1, USART_FLAG_TXE) == RESET) {}
  HAL_UART_Transmit(&huart1, &c, 1, HAL_MAX_DELAY);

	return c;
}


int _write(int file, char *ptr, int len)
{
	int DataIdx;

	for (DataIdx = 0; DataIdx < len; DataIdx++)
	{
		__io_putchar(*ptr++);
	}
	return len;
}


// Hack to hide botched printf("%d") on platformio SPL
// We can printf up to 5 integers wrapped in this
#define INTS_IN_PRINTF 5
char buffer[INTS_IN_PRINTF][INTS_IN_PRINTF];
// This is ...not great, but works for now and its planned to switch to different libraries
// that hopefully dont have this problem

char* STR5(uint32_t val) {
    #define ASCII_0  48
    static uint8_t index = 0;

    char* s = buffer[index ++ % INTS_IN_PRINTF];

    s[0] = (val / 1000) + ASCII_0;
    s[1] = ((val % 1000) / 100) + ASCII_0;
    s[2] = ((val % 100) / 10) + ASCII_0;
    s[3] = ((val % 10)) + ASCII_0;
    s[4] = '\0';

    return s;
}

char* DEC4B(uint32_t val) {
    #define ASCII_0  48
    static uint8_t index = 0;

    char* s = buffer[index ++ % INTS_IN_PRINTF];

    s[0] = (val / 10000) + ASCII_0;
    s[1] = ((val % 10000) / 1000) + ASCII_0;
    s[2] = ((val % 1000) / 100) + ASCII_0;
    s[3] = ((val % 100) / 10) + ASCII_0;
    s[4] = ((val % 10)) + ASCII_0;
    s[5] = '\0';

    return s;
}


char buffer_hex[INTS_IN_PRINTF][7];

char C(uint8_t i) {
  
  switch (i){
    case 0xF: return 'F';
    case 0xE: return 'E';
    case 0xD: return 'D';
    case 0xC: return 'C';
    case 0xB: return 'B';
    case 0xA: return 'A';
    default: return (i % 0xA) + ASCII_0;    
  }
}

char* HEX4B(uint32_t val) {
    #define ASCII_0  48
    static uint8_t index = 0;

    char* s = buffer_hex[index ++ % INTS_IN_PRINTF];

    s[0] = '0';
    s[1] = 'x';
    s[2] = C(val / 0x1000);
    s[3] = C((val % 0x1000) / 0x100);
    s[4] = C((val % 0x100) / 0x10);
    s[5] = C((val % 0x10));
    s[6] = '\0';

    return s;
}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(huart->Instance==USART1)
  {
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
}


void APP_USART_Init() {
 /* USARTx configured as follow:
  - BaudRate = 115200 baud
  - Word Length = 8 Bits
  - One Stop Bit
  - No parity
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled
  */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
}


void APP_USART_Send(char *c)
{
	int i = 0;
	while(c[i] != '\0')
	{
		 __io_putchar((int)c[i]);
		i++;
	}
}
