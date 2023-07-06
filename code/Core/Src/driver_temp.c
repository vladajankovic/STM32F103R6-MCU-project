/*
 * driver_temp.c
 *
 *  Created on: Jan 6, 2022
 *      Author: Marko Micovic
 */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "adc.h"
#include "gpio.h"

// MAX VOLTAGE = 5V
#define MAX_VOLTAGE 5.0
// ADC RESOLUTION = 2^12 = 4096 (12-bit resolution)
#define RESOLUTION 4096.0

static TaskHandle_t TEMP_WIND_TaskHandle;
static QueueHandle_t TEMP_MailboxHandle, WIND_MailboxHandle;

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	BaseType_t woken = pdFALSE;
	vTaskNotifyGiveFromISR(TEMP_WIND_TaskHandle, &woken);
	portYIELD_FROM_ISR(woken);
}

static void TEMP_WIND_Task(void *parameter)
{
	while (1)
	{
		GPIOA->ODR &= ~(0x01 << 6);
		HAL_ADC_Start_IT(&hadc1);
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

		float temperature = HAL_ADC_GetValue(&hadc1); // get raw conversion result
		temperature *= MAX_VOLTAGE / RESOLUTION; // translate into the voltage value
		temperature *= 100; // translate into temperature value (problem specific)

		xQueueOverwrite(TEMP_MailboxHandle, &temperature);

		GPIOA->ODR |= (0x01 << 6);
		HAL_ADC_Start_IT(&hadc1);
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

		uint32_t wind_direction = HAL_ADC_GetValue(&hadc1);

		xQueueOverwrite(WIND_MailboxHandle, &wind_direction);

		vTaskDelay(pdMS_TO_TICKS(200));
	}
}

float TEMP_GetCurrentValue()
{
	float result = 0.0;
	xQueuePeek(TEMP_MailboxHandle, &result, portMAX_DELAY);
	return result;
}

uint32_t WIND_GetCurrentValue()
{
	uint32_t result = 0;
	xQueuePeek(WIND_MailboxHandle, &result, portMAX_DELAY);
	return result;
}

void TEMP_WIND_Init()
{
	TEMP_MailboxHandle = xQueueCreate(1, sizeof(float));
	WIND_MailboxHandle = xQueueCreate(1, sizeof(uint32_t));
	xTaskCreate(TEMP_WIND_Task, "TEMP_Task", 64, NULL, 2, &TEMP_WIND_TaskHandle);
}
