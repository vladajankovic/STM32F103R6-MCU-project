/*
 * driver_anemo.c
 *
 *  Created on: 06.07.2023.
 *      Author: Vladimir Jankovic
 */

#include "driver_anemo.h"

#include "FreeRTOS.h"
#include "timers.h"

#include "tim.h"

TimerHandle_t tick_timer;

extern uint32_t anemo_cnt;
uint32_t first = 0;
uint32_t second = 0;
uint32_t period = 0;
float frequency = 0.0;

uint32_t state = 0;

uint32_t roundFrequency(uint32_t freq)
{
	if (freq > 2.5 && freq < 7.5) return 5;
	if (freq >= 7.5 && freq <= 12.5) return 10;
	if (freq > 12.5 && freq < 17.5) return 15;
	if (freq >= 17.5 && freq <= 22.5) return 20;
	if (freq > 22.5 && freq < 27.5) return 25;
	if (freq >= 27.5 && freq <= 32.5) return 30;
	if (freq > 32.5 && freq < 37.5) return 35;
	if (freq >= 37.5 && freq <= 42.5) return 40;
	if (freq > 42.5 && freq < 47.5) return 45;
	if (freq >= 47.5 && freq <= 52.5) return 50;
	if (freq > 52.5 && freq < 57.5) return 55;
	if (freq >= 57.5 && freq <= 62.5) return 60;
	if (freq > 62.5 && freq < 67.5) return 65;
	if (freq >= 67.5 && freq <= 72.5) return 70;
	if (freq > 72.5 && freq < 77.5) return 75;
	if (freq >= 77.5 && freq <= 82.5) return 80;
	if (freq > 82.5 && freq < 87.5) return 85;
	if (freq >= 87.5 && freq <= 92.5) return 90;
	if (freq > 92.5 && freq < 97.5) return 95;
	if (freq >= 97.5 && freq <= 102.5) return 100;
	if (freq > 102.5 && freq < 107.5) return 105;
	if (freq >= 107.5 && freq <= 111.5) return 110;
	if (freq > 111.5 && freq < 117.5) return 115;
	if (freq >= 117.5 && freq <= 121.5) return 120;
	if (freq > 121.5 && freq < 127.5) return 125;
	if (freq >= 127.5 && freq <= 132.5) return 130;
	if (freq > 132.5 && freq < 137.5) return 135;
	if (freq >= 137.5 && freq <= 142.5) return 140;
	if (freq > 142.5 && freq < 147.5) return 145;
	if (freq >= 147.5) return 150;
	return 0;
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == htim1.Instance && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		switch(state)
		{
		case 0:
		{
			first = HAL_TIM_ReadCapturedValue(&htim1, TIM_CHANNEL_1);
			state = 1;
			break;
		}
		case 1:
		{
			second = HAL_TIM_ReadCapturedValue(&htim1, TIM_CHANNEL_1);
			period = (second + 65535 * anemo_cnt) - first;
			if (period == 0)
			{
				frequency = 0;
			}
			else
			{
				frequency = 8000 / period;
			}
			anemo_cnt = 0;
			first = second;

		}
		}
	}
}

float ANEMO_GetCurrentValue()
{
	return roundFrequency((uint32_t)(frequency * 2.4));
}

void ANEMO_Init()
{
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_1);
	//HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_2);
}


