#include "UART.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"

static const uint32_t UART_BASE_ADDRESSES[] = {
    UART0_BASE,
    UART1_BASE,
    UART2_BASE,
    UART3_BASE,
    UART4_BASE,
    UART5_BASE,
    UART6_BASE,
    UART7_BASE
};

void UART_Init(UART_Config *config) {
    uint32_t uart_base = UART_BASE_ADDRESSES[config->peripheral];
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0 + config->peripheral);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA + (config->peripheral * 2));
    GPIOPinConfigure(GPIO_PA0_U0RX + (config->peripheral * 2));
    GPIOPinConfigure(GPIO_PA1_U0TX + (config->peripheral * 2));
    GPIOPinTypeUART(GPIO_PORTA_BASE + (config->peripheral * 2), GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(uart_base, SysCtlClockGet(), config->baud_rate, config->data_bits | config->stop_bits | config->parity);
    UARTEnable(uart_base);
}

void UART_Write(UART_Peripheral peripheral, uint8_t data) {
    uint32_t uart_base = UART_BASE_ADDRESSES[peripheral];
    UARTCharPut(uart_base, data);
}

void UART_WriteString(UART_Peripheral peripheral, const char *string) {
    uint32_t uart_base = UART_BASE_ADDRESSES[peripheral];
    while (*string) {
        UARTCharPut(uart_base, *string++);
    }
}

uint8_t UART_Read(UART_Peripheral peripheral) {
    uint32_t uart_base = UART_BASE_ADDRESSES[peripheral];
    return UARTCharGet(uart_base);
}
