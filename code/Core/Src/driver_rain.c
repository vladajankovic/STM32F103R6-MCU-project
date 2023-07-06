/*
 * driver_rain.c
 *
 *  Created on: 06.07.2023.
 *      Author: Vladimir Jankovic
 */

#include "driver_rain.h"
#include "gpio.h"

#include <math.h>

#include "FreeRTOS.h"
#include "timers.h"

TimerHandle_t rain_timer;

uint32_t milis = 0;
uint32_t volatile rainfall = 0;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_14)
	{
		rainfall = 36000 / milis;
		milis = 0;
	}
}

void timerCallback(TimerHandle_t timer)
{
	milis++;
	xTimerStart(rain_timer, portMAX_DELAY);
}

uint32_t RAIN_getCurrentValue()
{
	return rainfall;
}

void RAIN_Init()
{
	rain_timer = xTimerCreate("rain", pdMS_TO_TICKS(1), pdFALSE, NULL, timerCallback);
	xTimerStart(rain_timer, portMAX_DELAY);
}


