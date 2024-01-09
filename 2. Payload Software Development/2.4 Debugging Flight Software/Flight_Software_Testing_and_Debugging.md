# Session 17: Flight Software Testing and Debugging <!-- omit from toc -->

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

### General Debugging Process <!-- omit from toc -->
- **Observe bug.** Record the signs and/or symptoms of the bug.
  - What is observed?
  - What happened prior?
  - What was changed.
- **Reproduce Bug.** Reproduce the bug after a reset, record what state causes the bug.
  - Turn off and on again does bug occur again?
  - What needs to happen for the bug to occur again?
- **Test.** Create smallest test case which reproduces the bug.
  - Can you undo changes untill bug no longer occurs?
- **Isolate.** Separate code into sections and verify each section works as intended.
  - What code can be re-added, re-changed before bug occurs?
  - What section causes the error?
  - Can you verify other sections are not sources of error?
  - Can you trace where the origin of the error is?
- **Inspect.** Analyse the bug to find solution.
  - Can you link the information recorded to source of the bug?
  - Repeat above steps until bug is eliminated.
  - Can you confirm bug is in software or hardware?

### Software debugging tips <!-- omit from toc -->
- Analogue reading does not match expected.
  1. Check data types, ensure the types are the same and large enough to store data.
  2. Make sure mathematical operations are correct (BIMDAS).
  3. Ensure print statements are correct (string format, %d for integers, correct variable order).
  4. Ensure ADCs are configure correctly. Parameter settings including channels, conversion mode, DMA settings and continuous conversion.
  5. Check the values from ADCs individually (isolate one channel at a time).
- Communication issues
  1. Ensure USART is in asynchronous mode
  2. Check baud rate is valid (default 115200) for both serial setup (pinout configuration) and the serial console.
  3. Check serial port is correct (in command shell console), use device manager (windows).
- General IO errors
  1. Check pinout matches hardware (pinout & configuration) matches the hardware connected.
  2. Ensure pull-up / pull-down is correct for pin use (digital input).
  3. If supplying a LED ensure direction is correct.
     
### Common Errors <!-- omit from toc -->
- Use Comments
  - Comment code to explain the function of each line
  - Use logical names for variables
- Build Errors
  - Multiple build errors can often be attributed to one mistake
  - Use *"problems"* tab to view all errors
  - Check for missing semicolons - Simple Errors can look a lot more complex than they are.
  - Look at all build errors, the earlier errors may be symptoms of later errors and not the error source.
  - *error: expected ';' before '}' token* : indicates a semicolon is missing
  - *error: expected ')' before ';' token* : indicates a bracket is missing
  - If both of these are shown most likely only one of the two is missing.
  - Undeclared indicates wrong variable or function name, check spelling and case is correct
  - *invalid class* or *expected declaration or statement at end of input* likely indicates a missing brace/curly bracket
  - Don't ignore warnings, can be as important as errors. EG. Implicit declaration can indicate missing includes.
- Use print statements to check program states.
  - sprintf() to format a string to view the value of variables at different locations in code.
  - Then view value using HAL_UART_Transmit();
- Conditional statement errors
  - Break complex logic into simpler steps
  - Check logic results of logic statements
  - Check logical operation inputs are expected values
  - Use breakpoints to inspect logic flow 
- Device configuration errors
  - Ensure each pin does not have conflicting uses. EG PA2 used for USART2_TX cannot also be used for ADC1_IN7
  - Ensure clock configuration has no issues (use the Resolve Clock Issues tool)
- Check hardware.

### Hardware debugging tips <!-- omit from toc -->
  - Draw circuit diagrams to describe circuit
  - Ensure circuit matches diagram
    1. When using coloured wire use informative colours (Red power, Black ground)
    2. Keep space between components to avoid accidental shorts
    3. Ensure connections in prototyping boards is correct (not shorted)
 - Using a multimeter to check voltage
   1. Set multimeter to DC voltage (V and straight line not Squiggle)
   2. Connect leads to COM (center) and V/R
   3. Check multimeter by measuring between a ground (GND) and power (Vcc) pin (should read 5 or 3 volts)
   4. Measure voltage to pins which need testing, between GND and pin
   5. Make sure not to cause accidental shorts between pins
- Check no voltage occurs on any pins which are supposed to be grounded
- Check orientation of ICs using pin 1 indicator
- LEDs direction (cathode, short/flat side, towards ground)
