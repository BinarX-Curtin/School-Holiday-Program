# Session 5: Reading Sensors With a Microcontroller <!-- omit from toc -->

### Table of Contents <!-- omit from toc -->
- [Objectives](#objectives)
- [Requirements](#requirements)
- [Resources](#resources)
- [Procedure](#procedure)
- [1.0 STM32IDE Project Initialisation](#10-stm32ide-project-initialisation)
- [2.0 C Code Fundamentals](#20-c-code-fundamentals)
- [3.0 ADC Implementation](#30-adc-implementation)
- [4.0 Extension](#40-extension)

## Objectives
1. Download zip file with partially completed STM32 initialisation
1. Initialise internal temperature sensor
1. Understand the role of an ADC in data acquisition 
1. Read and display raw ADC data

## Requirements
1. STM32CubeIDE installed on device
1. Pen and paper
1. STM32L432KC Microcontroller with usb a to micro usb cable

## Resources
- STM32L4 Datasheet link: https://www.st.com/resource/en/reference_manual/rm0394-stm32l41xxx42xxx43xxx44xxx45xxx46xxx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
- What is an ADC: https://learn.sparkfun.com/tutorials/analog-to-digital-conversion/all#:~:text=An%20Analog%20to%20Digital%20Converter,the%20analog%20world%20around%20us.
- Coding in STM32CubeIDE: https://wiki.st.com/stm32mcu/wiki/STM32StepByStep:Getting_started_with_STM32_:_STM32_step_by_step

## Procedure
## 1.0 STM32IDE Project Initialisation
To begin our journey into learning how these complex devices are used, we first need to set up our devices for the correct functionality. Hence, we use STMCubeIDE to modify the pin functionalities.
