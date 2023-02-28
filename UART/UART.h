#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    UART0,
    UART1,
    UART2,
    UART3,
    UART4,
    UART5,
    UART6,
    UART7
} UART_Peripheral;

typedef struct {
    UART_Peripheral peripheral;
    uint32_t baud_rate;
    uint32_t data_bits;
    uint32_t stop_bits;
    uint32_t parity;
} UART_Config;

void UART_Init(UART_Config *config);

void UART_Write(UART_Peripheral peripheral, uint8_t data);

void UART_WriteString(UART_Peripheral peripheral, const char *string);

uint8_t UART_Read(UART_Peripheral peripheral);

#endif /* UART_H_ */
