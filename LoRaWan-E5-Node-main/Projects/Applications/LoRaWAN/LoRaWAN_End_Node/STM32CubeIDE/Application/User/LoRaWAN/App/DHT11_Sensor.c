/*
 * DHT11_Sensor.c
 *
 *  Created on: Mar 1, 2024
 *      Author: Delta
 */
#include "sensors.h"
#include "stm32wlxx_hal.h"
/*********************************** DHT11 FUNCTIONS ********************************************/
void delay(uint32_t us)
{
	volatile int i = 0;
	for(i= 0;i < (4*us);i++);
}


void Tim1_Config(void)
{
	RCC->APB2ENR |= (1 << 11);     // Tim1 clock enable
	TIM1->PSC = 47;               // Prescaller value to divide the freq.
	TIM1->ARR =0XFFFF;            // max value auto reload value
	TIM1->CR1 |= (1<<0);
	while(!(TIM1->SR &(1<<0)));
}
void delay_us(uint16_t  us)
{
	TIM1->CNT=0;
	while(TIM1->CNT < us);
}

void delay_ms(uint16_t  ms)
{
	TIM1->CNT=0;
	for(int i=0;i<ms;i++)
	{
		delay_us(1000);
	}
}
void Set_Pin_Output (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}


void Set_Pin_Input (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}


void DHT11_Start (void)
{
	Tim1_Config();
	Set_Pin_Output (DHT11_PORT, DHT11_PIN);  // set the pin as output
	HAL_GPIO_WritePin (DHT11_PORT, DHT11_PIN, 0);   // pull the pin low
	delay_us (18000);   // wait for 18ms
    HAL_GPIO_WritePin (DHT11_PORT, DHT11_PIN, 1);   // pull the pin high
    delay_us (20);   // wait for 20us
    Set_Pin_Input(DHT11_PORT, DHT11_PIN);    // set as input
}

uint8_t DHT11_Check_Response(void)
{
	uint8_t Response = 0;
	delay_us (40);
	if (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)))
	{
		delay_us (80);
		if ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN))) Response = 1;
		else Response = -1; // 255
	}
	while ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));   // wait for the pin to go low
	return Response;
}

uint8_t DHT11_Read(void)
{
	uint8_t i=0,j=0;
	for (j=0;j<8;j++)
	{
		while (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));   // wait for the pin to go high
		delay (40);   // wait for 40 us
		if (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)))   // if the pin is low
		{
			i&= ~(1<<(7-j));   // write 0
		}
		else i|= (1<<(7-j));  // if the pin is high, write 1
		while ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));  // wait for the pin to go low
	}
	return i;
}
