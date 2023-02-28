#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    UART0,
    UART1,
    UART2,
    UART3
} UART_Peripheral;

typedef struct {
    UART_Peripheral peripheral;
    uint32_t baudrate;
    uint8_t tx_pin;
    uint8_t rx_pin;
} UART_Config;

void UART_Init(UART_Config *config);

void UART_Write(UART_Peripheral peripheral, uint8_t data);

void UART_WriteString(UART_Peripheral peripheral, const char *string);

bool UART_Read(UART_Peripheral peripheral, uint8_t *data);

#endif /* UART_H_ */
