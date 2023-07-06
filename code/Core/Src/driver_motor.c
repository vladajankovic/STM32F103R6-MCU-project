/*
 * driver_motor.c
 *
 *  Created on: Jan 6, 2022
 *      Author: Marko Micovic
 */

#include "tim.h"

#define ARR 9

void MOTOR_NoSpeed()
{
	htim3.Instance->CCR4 = 0;
}

void MOTOR_HalfSpeed()
{
	htim3.Instance->CCR4 = ARR/2;
}

void MOTOR_FullSpeed()
{
	htim3.Instance->CCR4 = ARR;
}

void MOTOR_Init()
{
	HAL_TIM_OC_Start(&htim3, TIM_CHANNEL_4);
}
