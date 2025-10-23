
#include <avr/io.h>
// SPI Pins
#define CS_PORT PORTB
#define CS_PIN  PB4

void SPI_Init(void);
uint8_t SPI_Transfer(uint8_t data);
uint8_t SPI_ReadReg(uint8_t reg_addr);
void SPI_WriteReg(uint8_t reg_addr, uint8_t data);