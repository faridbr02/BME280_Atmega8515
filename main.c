#define F_CPU 8000000UL   

#include <avr/io.h>
#include <util/delay.h>
#include "Drivers/UART/uart.h"
#include "Drivers/BME_280/bme280.h"

int main() {
    UART_Init();
    BME280_Init();
    
    UART_Print("------- System Start ---------\r\n");
    
    // Verify chip,  0xD0 is the register where the id is stored (see datasheet)
    uint8_t chip_id = BME280_ReadChipId(0xD0);
    UART_Print("Chip ID: 0x");
    UART_PrintHex(chip_id);
    UART_Print("\r\n");
    if(chip_id != 0x60){
        UART_Print("BME280 not detected!\r\n");
        return 0;
    }
         
    
    UART_Print("BME280 detected! Reading calibration...\r\n");
    
    // Read calibration data
    BME280_ReadCalibrationData(&bme280_calib);
    UART_Print("Calibration loaded!\r\n");
    
    // CONFIGURE THE SENSOR - THIS IS CRITICAL!
    UART_Print("Configuring BME280...\r\n");
    BME280_ConfigureSensor();
    UART_Print("Sensor configured for normal mode!\r\n");
    
    UART_Print("\r\nStarting measurements:\r\n");
    UART_Print("-----------------------------------------\r\n");
    UART_Print("-----------------------------------------\r\n");
    BME280_Data bme_data;
    
    while(1) {
        // Read all sensor data
        BME280_ReadAllData(&bme280_calib, &bme_data);
        
        // -------Print the data---------
       
        //Pressure
        UART_Print("Press: ");
        int press_int = (int)bme_data.pressure;
        UART_PrintNumber(press_int);
        UART_Print(" hPa | ");
    
        //Temperature
        UART_Print("Temp: ");
        UART_PrintNumber(bme_data.temperature);
        UART_Print(" C | ");
    
  
    
        // Humidity
        UART_Print("Hum: ");
        int hum_int = (int)bme_data.humidity;
        UART_PrintNumber(hum_int);
        UART_Print(" %RH\r\n");
        
        
        
        _delay_ms(2000); // Read every 2 seconds
    }
}