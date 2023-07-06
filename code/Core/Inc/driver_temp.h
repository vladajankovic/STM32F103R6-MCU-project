/*
 * driver_temp.h
 *
 *  Created on: Jan 6, 2022
 *      Author: Marko Micovic
 */

#ifndef CORE_INC_DRIVER_TEMP_H_
#define CORE_INC_DRIVER_TEMP_H_

extern float TEMP_GetCurrentValue();
extern uint32_t WIND_GetCurrentValue();

extern void TEMP_WIND_Init();

#endif /* CORE_INC_DRIVER_TEMP_H_ */
