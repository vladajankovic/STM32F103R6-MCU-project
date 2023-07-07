# STM32F103R6-MCU-project

![project!](/assets/proteus_project.PNG)

### Tools used in project:
- Proteus 8 Professional
- STM32CubeMX
- Eclipse IDE for Embedded C/C++
- FreeRTOS

### Project description:
This project uses the *STM32F103R6* microcontroller for communication with various sensors and devices.
Sensors and devices used in this project:
- **LM35** - sensor for measuring temperature
- **RAINGAUGE** - sensor for measuring the amount of liquid precipitation
- **ANEMOMETER** - sensor for measuring wind speed
- **WINDVANE** - sensor for indicating wind direction

Signals received for the sensors are processed in different ways based on the sensor.<br>
The RAINGAUGE sensor sends an impulse when it is filled with water, and is calibrated to send an impulse every 1200 milliseconds on 30 mm/h rainfall.
The ANEMOMETER sensor outputs a square wave signal. The curent wind speed is equal to the frequency of the signal multiplied by a factor 2.4 in km/h units.
The WINDVANE and LM35 are connected to a multiplexer controlled with the MX_SELECT signal. The output signal MX_OUT is connected to the ADC peripheral of the MCU.

A MOTOR module representing a fan is connected in a circuit that is controlled by a PWM signal from the MCU. When the temperature is below 30 degrees Celsius,
the fan is stationary. Above 30 and below 35 degrees Celsius, the fan is rotating at half its maximum speed, and ablove 35 up to 60 degrees Celsius,
the fan spins at maximum speed. This is possible by changing the duty cycle of the PWM signal based on the current temperature.

The oscilloscope is used for displaying the output signals of some sensors (mainly digital signals).
A virtual terminal is used for a simplified output of the sensor values (a - azimuth angle, s - wind speed, t - temperature, r - rainfall amount).
The MCU establishes communication with the virtual terminal via UART serial communication protocol.

The LM041L 16x4 LCD display is used for displaying the sensor values, in each row, along with an arrow indicator.
For each sensor value, the arrow indicates the change from the previous to the current value. 
If the current value is higher than the previous, the arrow points UP.
If the current value is lower than the previous, the arrow points DOWN.
If no change occurs, the arrow does not change.

MCU timers are used for generating a PWM signal, processing ANEMOMETER output signal through Input Capture Channel mode with rising edge trigger,
while an external interrupt is used for capturing the RAINGAUGE impulse.
The MCU also uses FreeRTOS tasks, queues and timers to collect and update sensor values every 200 milliseconds, communicate with and control the LCD display,
virtual terminal, ADC peripheral, fan motor, multiplexer.
