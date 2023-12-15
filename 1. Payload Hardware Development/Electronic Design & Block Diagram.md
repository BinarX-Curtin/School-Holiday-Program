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
1. Familiarise ourselves with the funadmentals of electrical circuits.
2. Familiarise ourselves with how to wire up basic digital ICs (integrated circuits) including decoupling capacitors.
4. Build a basic circuit to turn an LED on and off.
5. Wire up a temperature sensor for future use with the microcontroller.
6. Use a multimeter to measure voltages in our circuits.

## Requirements
1. STM32CubeIDE installed on device
2. Pen and paper
3. STM32L432KC Microcontroller with usb a to micro usb cable

## Resources
- STM32L4 Datasheet link: https://www.st.com/resource/en/reference_manual/rm0394-stm32l41xxx42xxx43xxx44xxx45xxx46xxx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
- What is an ADC: https://learn.sparkfun.com/tutorials/analog-to-digital-conversion/all#:~:text=An%20Analog%20to%20Digital%20Converter,the%20analog%20world%20around%20us.
- Coding in STM32CubeIDE: https://wiki.st.com/stm32mcu/wiki/STM32StepByStep:Getting_started_with_STM32_:_STM32_step_by_step

## Procedure
## 1.0 STM32IDE Project Initialisation
To begin our journey into learning how these complex devices are used, we first need to set up our devices for the correct functionality. Hence, we use STMCubeIDE to modify the pin functionalities.

---

Staff:
Meg
Fergus
Will

Session Objectives:

Understand the basics of electronic design principles and block diagrams.
Create and interpret block diagrams for simple electronic circuits.


Activity outline:

Materials Needed (do a full BOM from the electronics component):

Whiteboard and markers (or a computer with a projector for digital presentations)
Handouts with example block diagrams
Basic electronic components (e.g., resistors, capacitors, LEDs)
Breadboards and jumper wires
Multimeter (optional)

Lesson Outline:

Introduction (15 minutes)

Engagement (5 minutes)

	- Explain
		○ Current divides around branches (water analogy)
		○ Ohms law
		○ Power (voltage x current)
	- How to wire up a basic digital chip
		○ Power
		○ Ground
		○ Decoupling Capacitor (little power supply buffer - look for a good analogy)
		○ Explain 2 circuits - resistor/divider (aka voltage divider) and Op-Amp (the swiss army knife of electrical components) amplifier
	- Explain schematics
	- Explain breadboard
	- Explain protoboard

