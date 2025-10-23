#include <util/delay.h>
#include "../UART/uart.h"
#include "bme280.h"




// Global calibration data
BME280_CalibData bme280_calib;
BME280_S32_t t_fine = 0;  



void BME280_Init(void){
    SPI_Init();
}

uint8_t BME280_ReadChipId(uint8_t regAdr){
    
    return SPI_ReadReg(regAdr);
}
// Configure BME280 for normal mode operation
void BME280_ConfigureSensor(void) {
    
    
    // Configure humidity oversampling (0xF2)
    // 0x01 = oversampling x1
    SPI_WriteReg(0xF2, 0x01);
    _delay_ms(10);
    
    // Configure temperature and pressure (0xF4)
    // [7:5] = temp oversampling x1 (001)
    // [4:2] = pressure oversampling x1 (001) 
    // [1:0] = normal mode (11)
    // 0x27 = 00100111
    SPI_WriteReg(0xF4, 0x27);
    _delay_ms(10);
    
    // Configure sampling (0xF5)
    // [7:5] = standby time 1000ms (101)
    // [4:2] = filter off (000)
    // [1:0] = SPI 4-wire (00)
    // 0xA0 = 10100000
    SPI_WriteReg(0xF5, 0xA0);
    _delay_ms(10); // Wait for first measurement
}
// COMPLETE CALIBRATION FUNCTION 
// see datasheet Table 16: Compensation parameter storage, naming and data type
void BME280_ReadCalibrationData(BME280_CalibData *cal) {
    // Read Temperature calibration (0x88-0x8D)
    uint8_t t1_l = SPI_ReadReg(0x88);
    uint8_t t1_h = SPI_ReadReg(0x89);
    cal->dig_T1 = (t1_h << 8) | t1_l;
    
    uint8_t t2_l = SPI_ReadReg(0x8A);
    uint8_t t2_h = SPI_ReadReg(0x8B);
    cal->dig_T2 = (t2_h << 8) | t2_l;
    
    uint8_t t3_l = SPI_ReadReg(0x8C);
    uint8_t t3_h = SPI_ReadReg(0x8D);
    cal->dig_T3 = (t3_h << 8) | t3_l;
    
    // Read Pressure calibration (0x8E-0x9F)
    uint8_t p1_l = SPI_ReadReg(0x8E);
    uint8_t p1_h = SPI_ReadReg(0x8F);
    cal->dig_P1 = (p1_h << 8) | p1_l;
    
    uint8_t p2_l = SPI_ReadReg(0x90);
    uint8_t p2_h = SPI_ReadReg(0x91);
    cal->dig_P2 = (p2_h << 8) | p2_l;
    
    uint8_t p3_l = SPI_ReadReg(0x92);
    uint8_t p3_h = SPI_ReadReg(0x93);
    cal->dig_P3 = (p3_h << 8) | p3_l;
    
    uint8_t p4_l = SPI_ReadReg(0x94);
    uint8_t p4_h = SPI_ReadReg(0x95);
    cal->dig_P4 = (p4_h << 8) | p4_l;
    
    uint8_t p5_l = SPI_ReadReg(0x96);
    uint8_t p5_h = SPI_ReadReg(0x97);
    cal->dig_P5 = (p5_h << 8) | p5_l;
    
    uint8_t p6_l = SPI_ReadReg(0x98);
    uint8_t p6_h = SPI_ReadReg(0x99);
    cal->dig_P6 = (p6_h << 8) | p6_l;
    
    uint8_t p7_l = SPI_ReadReg(0x9A);
    uint8_t p7_h = SPI_ReadReg(0x9B);
    cal->dig_P7 = (p7_h << 8) | p7_l;
    
    uint8_t p8_l = SPI_ReadReg(0x9C);
    uint8_t p8_h = SPI_ReadReg(0x9D);
    cal->dig_P8 = (p8_h << 8) | p8_l;
    
    uint8_t p9_l = SPI_ReadReg(0x9E);
    uint8_t p9_h = SPI_ReadReg(0x9F);
    cal->dig_P9 = (p9_h << 8) | p9_l;
    
    // Read Humidity calibration (0xA1, 0xE1-0xE7)
    cal->dig_H1 = SPI_ReadReg(0xA1);

    uint8_t e1 = SPI_ReadReg(0xE1);
    uint8_t e2 = SPI_ReadReg(0xE2);
    cal->dig_H2 = (int16_t)((e2 << 8) | e1);

    cal->dig_H3 = SPI_ReadReg(0xE3);

    uint8_t e4 = SPI_ReadReg(0xE4);
    uint8_t e5 = SPI_ReadReg(0xE5);
    uint8_t e6 = SPI_ReadReg(0xE6);

    cal->dig_H4 = (int16_t)((e4 << 4) | (e5 & 0x0F));
    cal->dig_H5 = (int16_t)((e6 << 4) | (e5 >> 4));
    cal->dig_H6 = (int8_t)SPI_ReadReg(0xE7);
}

// Read raw sensor data
void BME280_ReadRawData(BME280_S32_t *raw_temp, BME280_S32_t *raw_press, BME280_S32_t *raw_hum) {
    uint8_t data[8];
    
    CS_PORT &= ~(1 << CS_PIN);
    _delay_us(5);
    SPI_Transfer(0xF7 | 0x80);
    
    for(int i = 0; i < 8; i++) {
        data[i] = SPI_Transfer(0x00);
    }
    _delay_us(5);
    CS_PORT |= (1 << CS_PIN);
    
    *raw_press = ((uint32_t)data[0] << 12) | ((uint32_t)data[1] << 4) | ((uint32_t)data[2] >> 4);
    *raw_temp  = ((uint32_t)data[3] << 12) | ((uint32_t)data[4] << 4) | ((uint32_t)data[5] >> 4);
    *raw_hum   = ((uint32_t)data[6] << 8) | data[7];
}



// please take a look at dataSheet page 25 



BME280_S32_t BME280_compensate_T_int32(BME280_S32_t adc_T, BME280_CalibData *cal) {
    BME280_S32_t var1, var2, T;
    
    var1 = ((((adc_T >> 3) - ((BME280_S32_t)cal->dig_T1 << 1))) * ((BME280_S32_t)cal->dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((BME280_S32_t)cal->dig_T1)) * ((adc_T >> 4) - ((BME280_S32_t)cal->dig_T1))) >> 12) *
           ((BME280_S32_t)cal->dig_T3)) >> 14;
    
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    return T;
}

// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8 fractional bits)
// Output value of "24674867" represents 24674867/256 = 96386.2 Pa = 963.862 hPa
BME280_U32_t BME280_compensate_P_int64(BME280_S32_t adc_P, BME280_CalibData *cal) {
    BME280_S64_t var1, var2, p;
    
    var1 = ((BME280_S64_t)t_fine) - 128000;
    var2 = var1 * var1 * (BME280_S64_t)cal->dig_P6;
    var2 = var2 + ((var1 * (BME280_S64_t)cal->dig_P5) << 17);
    var2 = var2 + (((BME280_S64_t)cal->dig_P4) << 35);
    var1 = ((var1 * var1 * (BME280_S64_t)cal->dig_P3) >> 8) + ((var1 * (BME280_S64_t)cal->dig_P2) << 12);
    var1 = (((((BME280_S64_t)1) << 47) + var1)) * ((BME280_S64_t)cal->dig_P1) >> 33;
    
    if (var1 == 0) {
        return 0; // ensure not to divide by zero
    }
    
    p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((BME280_S64_t)cal->dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((BME280_S64_t)cal->dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((BME280_S64_t)cal->dig_P7) << 4);
    
    return (BME280_U32_t)p;
}




// Returns humidity in %RH as unsigned 32 bit integer in Q22.10 format (22 integer and 10 fractional bits)
// Output value of "47445" represents 47445/1024 = 46.333 %RH
BME280_U32_t BME280_compensate_H_int32(BME280_S32_t adc_H, BME280_CalibData *cal) {
    BME280_S32_t v_x1_u32r;
    
    v_x1_u32r = (t_fine - ((BME280_S32_t)76800));
    v_x1_u32r = (((((adc_H << 14) - (((BME280_S32_t)cal->dig_H4) << 20) - (((BME280_S32_t)cal->dig_H5) * v_x1_u32r)) + 
                  ((BME280_S32_t)16384)) >> 15) * (((((((v_x1_u32r * ((BME280_S32_t)cal->dig_H6)) >> 10) * 
                  (((v_x1_u32r * ((BME280_S32_t)cal->dig_H3)) >> 11) + ((BME280_S32_t)32768))) >> 10) + 
                  ((BME280_S32_t)2097152)) * ((BME280_S32_t)cal->dig_H2) + 8192) >> 14));
    v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * 
                  ((BME280_S32_t)cal->dig_H1)) >> 4));
    
    v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
    v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
    
    return (BME280_U32_t)(v_x1_u32r >> 12);
}



// Read and compensate all data using  Bosch functions
void BME280_ReadAllData(BME280_CalibData *cal, BME280_Data *result) {
    BME280_S32_t raw_temp, raw_press, raw_hum;
    
    BME280_ReadRawData(&raw_temp, &raw_press, &raw_hum);
    
    // Compensate temperature (returns value in 0.01°C units)
    BME280_S32_t temp_comp = BME280_compensate_T_int32(raw_temp, cal);
    result->temperature = temp_comp / 100.0;  // Convert to °C
    
    // Compensate pressure (returns value in Pa with 24.8 format)
    BME280_U32_t press_comp = BME280_compensate_P_int64(raw_press, cal);
    result->pressure = (press_comp / 256.0) / 100.0;  // Convert to hPa
    
    // Compensate humidity (returns value in %RH with 22.10 format)
    BME280_U32_t hum_comp = BME280_compensate_H_int32(raw_hum, cal);
    result->humidity = hum_comp / 1024.0;  // Convert to %RH
}



// Print the sensor data
void BME280_PrintData(BME280_Data *data) {
   
      // Pressure 
    UART_Print("Press: ");
    int press_int = (int)data->pressure;
    UART_PrintNumber(press_int);
    UART_Print(" hPa | ");
    
    
    //Temperature
    UART_Print("Temp: ");
    int temperature_int = (int)data->temperature;
    UART_PrintNumber(temperature_int);
    UART_Print(" C | ");
    
  
    
    // Humidity 
    UART_Print("Hum: ");
    int hum_int = (int)data->humidity;
    UART_PrintNumber(hum_int);
    UART_Print(" %RH\r\n");
}