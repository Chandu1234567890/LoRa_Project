/*
 * DS18B20_Sensor.c
 *
 *  Created on: Mar 1, 2024
 *      Author: Chandu, Sai, Aniket
 */

#include "sensors.h"
#include "stm32wlxx_hal.h"
#include "sys_app.h"
#include<stdio.h>

/***************   DS18B20_VARAIBLES__***************/


uint8_t Presence1 = 0;
uint8_t Temp_byte11 = 0, Temp_byte22 = 0;
float temp_data = 0;

uint8_t dsb_sen[10] = {0};

uint16_t DS18B20_Get_Value()
{
	  uint16_t temperature1 = 0;
	  Presence1 = DS18B20_Start ();
	  HAL_Delay (1);
	  DS18B20_Write (0xCC);  // skip ROM
	  DS18B20_Write (0x44);  // convert t
	  HAL_Delay (800);

	  Presence1 = DS18B20_Start ();
	  HAL_Delay(1);
	  DS18B20_Write (0xCC);  // skip ROM
	  DS18B20_Write (0xBE);  // Read Scratch-pad

	  Temp_byte11 = DS18B20_Read();
	  Temp_byte22 = DS18B20_Read();

	  temp_data = (float)(((Temp_byte22<<8)|(Temp_byte11))*RESOLUTION_FACTOR_FOR_12_BIT);
	  temperature1 = temp_data  * 100;

	  sprintf(dsb_sen,"%0.2f", temp_data);

	//  temperature1 = temp_data  * 100;
	 APP_LOG(TS_ON, VLEVEL_M, "DS18B20 Temperature ~%s\n\r", dsb_sen);
	  return temperature1;
}

uint8_t DS18B20_Start(void)
{
	Tim1_Config();
	uint8_t Response = 0;
	Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);		//set pin as o/p
	HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, 0);		//pin pulled to low
	delay(480);		//acc to datasheet

	Set_Pin_Input(DS18B20_PORT, DS18B20_PIN);		//set pin as i/p
	delay(80);

	if(!(HAL_GPIO_ReadPin(DS18B20_PORT, DS18B20_PIN)))
		Response = 1;		//pin is low==presence pulse detected
	else
		Response =-1;

	delay(400);		//480us total

	return Response;
}

void DS18B20_Write(uint8_t data)
{
	Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);		//set pin as o/p
	for(int i=0;i<8;i++)
	{
		if((data&(1<<i))!=0)		//if bit high
		{
			//write 1
			Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);		//set pin as o/p
			HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, 0);		//pull pin low
			delay(1);

			Set_Pin_Input(DS18B20_PORT, DS18B20_PIN);		//set pin as i/p
			delay(50);		//wait for 60 us
		}
		else		//if bit low
		{
			//write 0
			Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);		//set pin as i/p
			HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, 0);		//pull pin low
			delay(50);		//wait for 60 us

			Set_Pin_Input(DS18B20_PORT, DS18B20_PIN);		//set pin as i/p
		}
	}
}

uint8_t DS18B20_Read(void)
{
	uint8_t Value = 0;
	Set_Pin_Input(DS18B20_PORT, DS18B20_PIN);		//set pin as i/p
	for(int i=0;i<8;i++)
	{
		Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);		//set pin as o/p
		HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, 0);		//pull data pin low
		delay(2);		//2us delay

		Set_Pin_Input(DS18B20_PORT, DS18B20_PIN);		//set pin as i/p
		if(HAL_GPIO_ReadPin(DS18B20_PORT, DS18B20_PIN))		//if pin high
		{
			Value |= 1<<i;		//read =1
		}
		delay(60);		//wait 60us
	}
	return Value;
}

