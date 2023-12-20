# Session 18: Payload Hardware Testing and Debugging <!-- omit from toc -->

### Table of Contents <!-- omit from toc -->
- [Objectives](#objectives)
- [Requirements](#requirements)
- [Resources](#resources)
- [Procedure](#procedure)


## Objectives
1. Identify problems within the flight software code.
2. Systematically debug the identified problems.

## Requirements
1. STM32CubeIDE installed on device
1. Pen and paper
1. STM32L432KC Microcontroller with usb a to micro usb cable

## Resources
- STM32L4 Datasheet link: https://www.st.com/resource/en/reference_manual/rm0394-stm32l41xxx42xxx43xxx44xxx45xxx46xxx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
- Coding in STM32CubeIDE: https://wiki.st.com/stm32mcu/wiki/STM32StepByStep:Getting_started_with_STM32_:_STM32_step_by_step

## Procedure

### Software debugging tips <!-- omit from toc -->
- Observe bug, what is the signs or symptoms of the bug.
- ​**Isolate.** Separate code into sections and verify each section works as intended.
- Create test case, smallest test case which replicates the bug.
- Inspect and analyse the bug to find solution.

### Common things to remember <!-- omit from toc -->
- Data types, check no data type conflicts or integer overflow is occurring.
- Use print statements to check program states.
- Log debugging test cases and results.
- Check hardware.