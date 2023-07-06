/*
 * homework.c
 *
 *  Created on: Jan 6, 2022
 *      Author: Marko Micovic
 */

#include "homework.h"

#include "FreeRTOS.h"
#include "task.h"

#include "gpio.h"
#include <string.h>

#include "driver_lcd.h"
#include "driver_uart.h"
#include "driver_motor.h"
#include "driver_temp.h"
#include "driver_rain.h"
#include "driver_anemo.h"

uint32_t toAngleValue(uint32_t val)
{
	switch(val)
	{
	case 952:
		return 0;
	case 2471:
		return 22;
	case 2250:
		return 45;
	case 3760:
		return 67;
	case 3723:
		return 90;
	case 3831:
		return 112;
	case 3356:
		return 135;
	case 3589:
		return 157;
	case 2946:
		return 180;
	case 3116:
		return 202;
	case 1575:
		return 225;
	case 1698:
		return 247;
	case 315:
		return 270;
	case 786:
		return 292;
	case 547:
		return 315;
	case 1284:
		return 337;
	default:
		return 0;
	}
}

static void homeworkTask(void *parameters)
{

	uint32_t curr_azimuth = 0;
	uint32_t prev_azimuth = 0;
	uint8_t aarrow = 0x01;

	char aarr[3] = "/a=";

	uint32_t curr_speed = 0;
	uint32_t prev_speed = 0;
	uint8_t sarrow = 0x01;

	char sarr[3] = "/s=";

	uint8_t curr_temperature = 0;
	uint8_t prev_temperature = 0;
	uint8_t tarrow = 0x01;

	char tarr[3] = "/t=";

	uint32_t curr_rainfall = 0;
	uint32_t prev_rainfall = 0;
	uint8_t rarrow = 0x01;

	char rarr[3] = "/r=";

	while(1)
	{
		//*************************************************************
		// WIND DIRECTION MEASURING

		curr_azimuth = toAngleValue(WIND_GetCurrentValue());

		// uart
		for(uint8_t i = 0; i < 3; i++)
		{
			UART_AsyncTransmitCharacter(aarr[i]);
		}
		UART_AsyncTransmitDecimal(curr_azimuth);

		// lcd
		LCD_CommandEnqueue(LCD_INSTRUCTION, LCD_SET_DD_RAM_ADDRESS_INSTRUCTION | 0x00);
		if (prev_azimuth < curr_azimuth)
		{
			aarrow = 0x01;
		}
		if (prev_azimuth > curr_azimuth)
		{
			aarrow = 0x00;
		}
		LCD_CommandEnqueue(LCD_DATA, aarrow);

		LCD_CommandEnqueue(LCD_INSTRUCTION, LCD_SET_DD_RAM_ADDRESS_INSTRUCTION | 0x02);
		LCD_CommandEnqueue(LCD_DATA, 'A');
		LCD_CommandEnqueue(LCD_DATA, 'z');
		LCD_CommandEnqueue(LCD_DATA, 'i');
		LCD_CommandEnqueue(LCD_DATA, 'm');
		LCD_CommandEnqueue(LCD_DATA, ':');

		LCD_CommandEnqueue(LCD_INSTRUCTION, LCD_SET_DD_RAM_ADDRESS_INSTRUCTION | 0x08);
		char wind_decimal[5] = "000.5";
		wind_decimal[0] = (curr_azimuth / 100) + '0';
		wind_decimal[1] = ((curr_azimuth / 10) % 10) + '0';
		wind_decimal[2] = (curr_azimuth % 10) + '0';
		if (wind_decimal[0] != '0')
		{
			LCD_CommandEnqueue(LCD_DATA, wind_decimal[0]);
		}
		if (wind_decimal[1] != '0' || (wind_decimal[0] != '0' && wind_decimal[1] == '0'))
		{
			LCD_CommandEnqueue(LCD_DATA, wind_decimal[1]);
		}
		LCD_CommandEnqueue(LCD_DATA, wind_decimal[2]);
		if ((curr_azimuth % 10 == 2) || (curr_azimuth % 10 == 7))
		{
			UART_AsyncTransmitString(".5");
			LCD_CommandEnqueue(LCD_DATA, wind_decimal[3]);
			LCD_CommandEnqueue(LCD_DATA, wind_decimal[4]);
		}

		prev_azimuth = curr_azimuth;

		//*************************************************************
		// WIND SPEED MEASURING

		curr_speed = (uint32_t)ANEMO_GetCurrentValue();

		// uart
		for(uint8_t i = 0; i < 3; i++)
		{
			UART_AsyncTransmitCharacter(sarr[i]);
		}
		UART_AsyncTransmitDecimal(curr_speed);

		// lcd
		LCD_CommandEnqueue(LCD_INSTRUCTION, LCD_SET_DD_RAM_ADDRESS_INSTRUCTION | 0x40);
		if (prev_speed < curr_speed)
		{
			sarrow = 0x01;
		}
		if (prev_speed > curr_speed)
		{
			sarrow = 0x00;
		}
		LCD_CommandEnqueue(LCD_DATA, sarrow);

		LCD_CommandEnqueue(LCD_INSTRUCTION, LCD_SET_DD_RAM_ADDRESS_INSTRUCTION | 0x42);
		LCD_CommandEnqueue(LCD_DATA, 'W');
		LCD_CommandEnqueue(LCD_DATA, 'S');
		LCD_CommandEnqueue(LCD_DATA, 'p');
		LCD_CommandEnqueue(LCD_DATA, 'd');
		LCD_CommandEnqueue(LCD_DATA, ':');

		LCD_CommandEnqueue(LCD_INSTRUCTION, LCD_SET_DD_RAM_ADDRESS_INSTRUCTION | 0x48);
		char speed_decimal[3];
		speed_decimal[0] = (curr_speed / 100) + '0';
		speed_decimal[1] = ((curr_speed / 10) % 10) + '0';
		speed_decimal[2] = (curr_speed % 10) + '0';
		if (speed_decimal[0] != '0')
		{
			LCD_CommandEnqueue(LCD_DATA, speed_decimal[0]);
		}
		if (speed_decimal[1] != '0' || (speed_decimal[0] != '0' && speed_decimal[1] == '0'))
		{
			LCD_CommandEnqueue(LCD_DATA, speed_decimal[1]);
		}
		LCD_CommandEnqueue(LCD_DATA, speed_decimal[2]);

		prev_speed = curr_speed;


		//*************************************************************
		// TEMPERATURE MEASURING

		curr_temperature = (uint32_t)TEMP_GetCurrentValue();

		// uart
		for(uint8_t i = 0; i < 3; i++)
		{
			UART_AsyncTransmitCharacter(tarr[i]);
		}
		UART_AsyncTransmitDecimal(curr_temperature);

		// lcd
		LCD_CommandEnqueue(LCD_INSTRUCTION, LCD_SET_DD_RAM_ADDRESS_INSTRUCTION | 0x10);
		if (prev_temperature < curr_temperature)
		{
			tarrow = 0x01;
		}
		if (prev_temperature > curr_temperature)
		{
			tarrow = 0x00;
		}
		LCD_CommandEnqueue(LCD_DATA, tarrow);

		LCD_CommandEnqueue(LCD_INSTRUCTION, LCD_SET_DD_RAM_ADDRESS_INSTRUCTION | 0x12);
		LCD_CommandEnqueue(LCD_DATA, 'T');
		LCD_CommandEnqueue(LCD_DATA, 'e');
		LCD_CommandEnqueue(LCD_DATA, 'm');
		LCD_CommandEnqueue(LCD_DATA, 'p');
		LCD_CommandEnqueue(LCD_DATA, ':');

		LCD_CommandEnqueue(LCD_INSTRUCTION, LCD_SET_DD_RAM_ADDRESS_INSTRUCTION | 0x18);
		char temp_decimal[2];
		temp_decimal[0] = (curr_temperature / 10) + '0';
		temp_decimal[1] = (curr_temperature % 10) + '0';
		if (temp_decimal[0] != '0')
		{
			LCD_CommandEnqueue(LCD_DATA, temp_decimal[0]);
		}
		LCD_CommandEnqueue(LCD_DATA, temp_decimal[1]);

		// motor
		if (curr_temperature >= 0 && curr_temperature < 30)
		{
			MOTOR_NoSpeed();
		}
		if (curr_temperature >= 30 && curr_temperature < 35)
		{
			MOTOR_HalfSpeed();
		}
		if (curr_temperature >= 35 && curr_temperature <= 60)
		{
			MOTOR_FullSpeed();
		}

		prev_temperature = curr_temperature;

		//*************************************************************
		// RAINFALL MEASURING

		curr_rainfall = (uint32_t)RAIN_getCurrentValue();

		// uart
		for(uint8_t i = 0; i < 3; i++)
		{
			UART_AsyncTransmitCharacter(rarr[i]);
		}
		UART_AsyncTransmitDecimal(curr_rainfall);

		// lcd
		LCD_CommandEnqueue(LCD_INSTRUCTION, LCD_SET_DD_RAM_ADDRESS_INSTRUCTION | 0x50);
		if (prev_rainfall < curr_rainfall)
		{
			rarrow = 0x01;
		}
		if (prev_rainfall > curr_rainfall)
		{
			rarrow = 0x00;
		}
		LCD_CommandEnqueue(LCD_DATA, rarrow);

		LCD_CommandEnqueue(LCD_INSTRUCTION, LCD_SET_DD_RAM_ADDRESS_INSTRUCTION | 0x52);
		LCD_CommandEnqueue(LCD_DATA, 'R');
		LCD_CommandEnqueue(LCD_DATA, 'a');
		LCD_CommandEnqueue(LCD_DATA, 'i');
		LCD_CommandEnqueue(LCD_DATA, 'n');
		LCD_CommandEnqueue(LCD_DATA, ':');

		LCD_CommandEnqueue(LCD_INSTRUCTION, LCD_SET_DD_RAM_ADDRESS_INSTRUCTION | 0x58);
		char rain_decimal[3];
		rain_decimal[0] = (curr_rainfall / 100) + '0';
		rain_decimal[1] = ((curr_rainfall / 10) % 10) + '0';
		rain_decimal[2] = (curr_rainfall % 10) + '0';
		if (rain_decimal[0] != '0')
		{
			LCD_CommandEnqueue(LCD_DATA, rain_decimal[0]);
		}
		if (rain_decimal[1] != '0' || (rain_decimal[0] != '0' && rain_decimal[1] == '0'))
		{
			LCD_CommandEnqueue(LCD_DATA, rain_decimal[1]);
		}
		LCD_CommandEnqueue(LCD_DATA, rain_decimal[2]);


		prev_rainfall = curr_rainfall;

		//*************************************************************
		// DELAY TASK
		vTaskDelay(pdMS_TO_TICKS(200));

		//*************************************************************
		// CLEAR LCD AND TERMINAL
		for(uint8_t i = 0; i < 26; i++)
		{
			UART_AsyncTransmitCharacter('\b');
		}
		LCD_CommandEnqueue(LCD_INSTRUCTION, LCD_CLEAR_DISPLAY_INSTRUCTION);

		//*************************************************************
	}
}

void homeworkInit()
{
	LCD_Init();
	UART_Init();
	TEMP_WIND_Init();
	MOTOR_Init();
	RAIN_Init();
	ANEMO_Init();
	xTaskCreate(homeworkTask, "homeworkTask", 64, NULL, 5, NULL);
}

