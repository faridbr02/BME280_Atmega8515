#include "spi.h"
#include <util/delay.h>


// SPI Initialization
void SPI_Init(void) {
    // Set MOSI, SCK, CS as outputs
    DDRB |= (1 << PB5) | (1 << PB7) | (1 << PB4);
    // Set MISO as input
    DDRB &= ~(1 << PB6);
    
    // Enable SPI, Master mode, Clock rate = F_CPU/16
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
    
    // Start with CS HIGH (inactive)
    CS_PORT |= (1 << CS_PIN);
}

// SPI Data Transfer
uint8_t SPI_Transfer(uint8_t data) {
    SPDR = data;
    while (!(SPSR & (1 << SPIF)));
    return SPDR;
}

// Read spi Register
uint8_t SPI_ReadReg(uint8_t reg_addr) {
    uint8_t received_data;
    
    // Activate sensor (CS LOW)
    CS_PORT &= ~(1 << CS_PIN);
    _delay_us(10);
    
    // Send register address WITH read bit (0x80)
    SPI_Transfer(reg_addr | 0x80);
    
    // Send dummy byte to receive data
    received_data = SPI_Transfer(0x00);
    
    _delay_us(10);
    // Deactivate sensor (CS HIGH)
    CS_PORT |= (1 << CS_PIN);
    
    return received_data;
}
// Write SPI Register
void SPI_WriteReg(uint8_t reg_addr, uint8_t data) {
    CS_PORT &= ~(1 << CS_PIN);
    _delay_us(10);
    SPI_Transfer(reg_addr & 0x7F);  // Write bit is 0
    SPI_Transfer(data);
    _delay_us(10);
    CS_PORT |= (1 << CS_PIN);
}
