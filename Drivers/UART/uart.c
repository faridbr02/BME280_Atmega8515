#include <xc.h>





// UART for debugging
void UART_Init() {
    UBRRH = 0;
    UBRRL = 51; // 9600 baud @ 8MHz
    UCSRB = (1 << RXEN) | (1 << TXEN);
    UCSRC = (1 << URSEL) | (3 << UCSZ0);
}

void UART_Send(char data) {
    while (!(UCSRA & (1 << UDRE)));
    UDR = data;
}

void UART_Print(const char *str) {
    while (*str) UART_Send(*str++);
}

void UART_PrintHex(uint8_t num) {
    char hex[] = "0123456789ABCDEF";
    UART_Send(hex[(num >> 4) & 0x0F]);
    UART_Send(hex[num & 0x0F]);
}
// Print integer with sign handling
void UART_PrintNumber(int16_t number) {
    if (number < 0) {
        UART_Send('-');          // Print minus sign
        number = -number;        // Make it positive
    }
    
    // print the positive number
    if (number >= 10000) UART_Send((number / 10000) + '0');
    if (number >= 1000) UART_Send(((number / 1000) % 10) + '0');
    if (number >= 100) UART_Send(((number / 100) % 10) + '0');
    if (number >= 10) UART_Send(((number / 10) % 10) + '0');
    UART_Send((number % 10) + '0');
}
