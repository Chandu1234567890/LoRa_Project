/*
 * sensors.h
 *   *********All Sensors Declarations********
 *   1.BME280------->It recognizes Temperature, Humidity, Pressure.
 *   2.DHT11-------->It recognizes Temperature and Humidity
 *   3.DS18B20------>It recognizes Temperature
 *   4.MQ6   ------>It recognizes  LPG.
 *  Created on: Mar 1, 2024
 *      Author: Delta
 */

#ifndef APPLICATION_USER_INC_SENSORS_H_
#define APPLICATION_USER_INC_SENSORS_H_

#include "stm32wlxx_hal.h"
#include "stdint.h"


#define ACTIVATE_BME     0x10
#define ACTIVATE_DHT11   0x20
#define ACTIVATE_DS18B20 0x40
#define ACTIVATE_MQ6     0x80


typedef struct sensor_data{
	uint16_t Sens_Temperature ;
	uint16_t Sens_Humidity ;
	uint16_t Sens_Pressure ;

}SensorDataTypedef;

/**********************************************************************************/
/*                            BME280 Declarations                                  */
/**********************************************************************************/
void BME_280_Init(void);
int BME280_Config (uint8_t osrs_t, uint8_t osrs_p, uint8_t osrs_h, uint8_t mode, uint8_t t_sb, uint8_t filter);


// Read the Trimming parameters saved in the NVM ROM of the device
void TrimRead(void);

/* To be used when doing the force measurement
 * the Device need to be put in forced mode every time the measurement is needed
 */
void BME280_WakeUP(void);
void BME_280_Init(void);

/* measure the temp, pressure and humidity
 * the values will be stored in the parameters passed to the function
 */
int BME280_Measure (void);


// Oversampling definitions
#define OSRS_OFF    	0x00
#define OSRS_1      	0x01
#define OSRS_2      	0x02
#define OSRS_4      	0x03
#define OSRS_8      	0x04
#define OSRS_16     	0x05

// MODE Definitions
#define MODE_SLEEP      0x00
#define MODE_FORCED     0x01
#define MODE_NORMAL     0x03

// Standby Time
#define T_SB_0p5    	0x00
#define T_SB_62p5   	0x01
#define T_SB_125    	0x02
#define T_SB_250    	0x03
#define T_SB_500    	0x04
#define T_SB_1000   	0x05
#define T_SB_10     	0x06
#define T_SB_20     	0x07

// IIR Filter Coefficients
#define IIR_OFF     	0x00
#define IIR_2       	0x01
#define IIR_4       	0x02
#define IIR_8       	0x03
#define IIR_16      	0x04


// REGISTERS DEFINITIONS
#define ID_REG      	0xD0
#define RESET_REG  		0xE0
#define CTRL_HUM_REG    0xF2
#define STATUS_REG      0xF3
#define CTRL_MEAS_REG   0xF4
#define CONFIG_REG      0xF5
#define PRESS_MSB_REG   0xF7



/**********************************************************************************/
/*                            DHT11 Declarations                                  */
/**********************************************************************************/
#define DHT11_PORT GPIOB
#define DHT11_PIN GPIO_PIN_4

void delay(uint32_t us);

void Set_Pin_Output (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

void Set_Pin_Input (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void DHT11_Start (void);
uint8_t DHT11_Check_Response (void);
uint8_t DHT11_Read (void);


/**********************************************************************************/
/*                            DS18B20 Declarations                                */
/**********************************************************************************/
#define DS18B20_PORT    				GPIOB
#define DS18B20_PIN						GPIO_PIN_7
#define RESOLUTION_FACTOR_FOR_12_BIT	0.0625

uint16_t DS18B20_Get_Value(void);

uint8_t DS18B20_Start(void);
void DS18B20_Write(uint8_t data);
uint8_t DS18B20_Read(void);


/**********************************************************************************/
/*                           MQ6 Declarations                                     */
/**********************************************************************************/
uint16_t My_ADC_ReadChannels(uint32_t channel);
int16_t MQ6_sensor(void);


void Get_sensor_values(SensorDataTypedef *Data, uint8_t value);

void Tim1_Config(void);

void delay_us(uint16_t  us);
void delay_ms(uint16_t  ms);
#endif /* APPLICATION_USER_INC_SENSORS_H_ */
