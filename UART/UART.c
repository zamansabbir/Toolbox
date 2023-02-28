#include "UART.h"
#include "em_usart.h"
#include "em_cmu.h"
#include "em_gpio.h"

static const USART_TypeDef *UART_BASE_ADDRESSES[] = {
    USART0,
    USART1,
    USART2,
    USART3
};

void UART_Init(UART_Config *config) {
    USART_TypeDef *uart_base = (USART_TypeDef *) UART_BASE_ADDRESSES[config->peripheral];
    CMU_ClockEnable(cmuClock_GPIO, true);
    CMU_ClockEnable(cmuClock_USART0 + config->peripheral, true);
    GPIO_PinModeSet(gpioPortA, config->tx_pin, gpioModePushPull, 1);
    GPIO_PinModeSet(gpioPortA, config->rx_pin, gpioModeInput, 0);
    USART_InitAsync_TypeDef uart_init = USART_INITASYNC_DEFAULT;
    uart_init.enable = usartDisable;
    uart_init.baudrate = config->baudrate;
    USART_InitAsync(uart_base, &uart_init);
    uart_base->ROUTELOC0 |= USART_ROUTELOC0_TXLOC_LOC0 | USART_ROUTELOC0_RXLOC_LOC0;
    uart_base->ROUTEPEN |= USART_ROUTEPEN_TXPEN | USART_ROUTEPEN_RXPEN;
    USART_Enable(uart_base, usartEnable);
}

void UART_Write(UART_Peripheral peripheral, uint8_t data) {
    USART_TypeDef *uart_base = (USART_TypeDef *) UART_BASE_ADDRESSES[peripheral];
    while (!(uart_base->STATUS & USART_STATUS_TXBL));
    uart_base->TXDATA = data;
}

void UART_WriteString(UART_Peripheral peripheral, const char *string) {
    while (*string != '\0') {
        UART_Write(peripheral, *string++);
    }
}

bool UART_Read(UART_Peripheral peripheral, uint8_t *data) {
    USART_TypeDef *uart_base = (USART_TypeDef *) UART_BASE_ADDRESSES[peripheral];
    if (!(uart_base->STATUS & USART_STATUS_RXDATAV)) {
        return false;
    }
    *data = uart_base->RXDATA;
    return true;
}
