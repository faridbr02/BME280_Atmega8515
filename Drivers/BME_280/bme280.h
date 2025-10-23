#ifndef BME280_H
#define BME280_H

#include <avr/io.h>
#include "../SPI/spi.h"
// Data types matching datasheet
typedef int32_t  BME280_S32_t;
typedef uint32_t BME280_U32_t;
typedef int64_t  BME280_S64_t;

// Calibration data structure
typedef struct {
    // Temperature calibration
    uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;
    
    // Pressure calibration  
    uint16_t dig_P1;
    int16_t  dig_P2;
    int16_t  dig_P3;
    int16_t  dig_P4;
    int16_t  dig_P5;
    int16_t  dig_P6;
    int16_t  dig_P7;
    int16_t  dig_P8;
    int16_t  dig_P9;
    
    // Humidity calibration
    uint8_t  dig_H1;
    int16_t  dig_H2;
    uint8_t  dig_H3;
    int16_t  dig_H4;
    int16_t  dig_H5;
    int8_t   dig_H6;
} BME280_CalibData;

typedef struct {
    float temperature;  // °C
    float pressure;     // hPa  
    float humidity;     // %RH
} BME280_Data;

// Global t_fine as per datasheet
extern BME280_S32_t t_fine;
// External declaration for global calibration data
extern BME280_CalibData bme280_calib;
// Function prototypes
void BME280_Init(void);
uint8_t BME280_ReadChipId(uint8_t regAdr);
void BME280_ConfigureSensor(void);
void BME280_ReadCalibrationData(BME280_CalibData *cal);


// Official Bosch compensation functions
BME280_S32_t BME280_compensate_T_int32(BME280_S32_t adc_T , BME280_CalibData *cal);
BME280_U32_t BME280_compensate_P_int64(BME280_S32_t adc_P, BME280_CalibData *cal);
BME280_U32_t BME280_compensate_H_int32(BME280_S32_t adc_H,  BME280_CalibData *cal);

// Data reading functions
void BME280_ReadRawData(BME280_S32_t *raw_temp, BME280_S32_t* raw_press, BME280_S32_t* raw_hum);
void BME280_ReadAllData(BME280_CalibData *cal, BME280_Data*result);
void BME280_PrintData(BME280_Data *data);



#endif