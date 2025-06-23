/*
 * adc124S051.c
 *
 *  Created on: Jun 23, 2025
 *      Author: yasin.arslan
 */
#include "main.h"
#define ADC1_ADR0 0
#define ADC1_ADR1 8
#define ADC1_ADR2 16

uint8_t spiDataRx[2],dummySpi[2];
uint8_t spiDataTx[2] = {0x00,0x00};
uint8_t debugTmp = 0;
uint8_t adcRead = 0,caseCycle = 0;
uint16_t ADC1_In0_Value = 0,ADC1_In1_Value = 0,ADC1_In2_Value = 0;


extern SPI_HandleTypeDef hspi1;

void Adc124s051_Init(void){
	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi1, spiDataTx, spiDataRx, 2, 1);
	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
}
void Adc124s051_Scan(void)
{
	switch (adcRead)
	{
	case 0:
		caseCycle++;
		adcRead = caseCycle;
		HAL_Delay(10);
		if(caseCycle == 3)
			caseCycle = 0;
		break;
	case 1:
		spiDataTx[0]= ADC1_ADR0;
		HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET);
		HAL_SPI_TransmitReceive(&hspi1, spiDataTx, spiDataRx, 2, 1); // bir önceki adc değerini okuyor.
		HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);
		adcRead = 0;
		break;
	case 2:
		spiDataTx[0]= ADC1_ADR1;
		HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET);
		HAL_SPI_TransmitReceive(&hspi1, spiDataTx, spiDataRx, 2, 1); //bir önceki adc değerini okuyor
		HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);
		adcRead = 0;
		break;
	case 3:
		spiDataTx[0]= ADC1_ADR2;
		HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET);
		HAL_SPI_TransmitReceive(&hspi1, spiDataTx, spiDataRx, 2, 1); //bir önceki adc değerini okuyor
		HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);
		adcRead = 0;
		break;

	}
}
void Adc124s051_Set_Channel(uint8_t IN_CH)
{
	uint8_t inputCh=0;
	switch (IN_CH)
	{
	case 1:
		inputCh = ADC1_ADR0;
		break;
	case 2:
		inputCh = ADC1_ADR1;
		break;
	case 3:
		inputCh = ADC1_ADR2;
		break;
	}
	spiDataTx[0] = inputCh;
	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, spiDataTx,2, 1); //bir önceki adc değerini okuyor
	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);

}

void Adc124s051_Read_Channel(uint8_t IN_CH,uint8_t sampleSize)//sample size 50-200 kps
{
	uint8_t inputCh=0;

	switch (IN_CH)
	{
	case 1:
		inputCh = ADC1_ADR0;
		break;
	case 2:
		inputCh = ADC1_ADR1;
		break;
	case 3:
		inputCh = ADC1_ADR2;
		break;
	}
	spiDataTx[0] = inputCh;
	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi1, spiDataTx,spiDataRx,2, 1); //bir önceki adc değerini okuyor
	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);
	switch (IN_CH)
	{
	case 1:
		ADC1_In0_Value = (spiDataRx[1] << 8)+ (spiDataRx[0]);
		break;
	case 2:
		ADC1_In1_Value = (spiDataRx[1] << 8)+ (spiDataRx[0]);
		break;
	case 3:
		ADC1_In2_Value = (spiDataRx[1] << 8) + (spiDataRx[0]);
		break;
	}

}


