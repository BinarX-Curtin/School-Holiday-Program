# Session 5: Reading Sensors With a Microcontroller

## Objectives
1. Download zip file with partially completed STM32 initialisation
2. Initialise internal temperature sensor
3. Understand the role of an ADC in data acquisition 
4. Read and display raw ADC data

## Requirements
1. STM32CubeIDE installed on device
2. Pen and paper
3. STM32L432KC Microcontroller with usb a to micro usb cable

## Resources
- STM32L4 Datasheet link: https://www.st.com/resource/en/reference_manual/rm0394-stm32l41xxx42xxx43xxx44xxx45xxx46xxx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
- What is an ADC: https://learn.sparkfun.com/tutorials/analog-to-digital-conversion/all#:~:text=An%20Analog%20to%20Digital%20Converter,the%20analog%20world%20around%20us.
- Coding in STM32CubeIDE: https://wiki.st.com/stm32mcu/wiki/STM32StepByStep:Getting_started_with_STM32_:_STM32_step_by_step

## Procedure
### 1.0 STM32IDE Project Initialisation
To begin our journey into learning how these complex devices are used, we first need to set up our devices for the correct functionality. Hence, we use STMCubeIDE to modify the pin functionalities.

1.	Download the *ReadSensWithMicro.proj.zip* file from the repository.

**Insert image of file with red box around it and another after it with a right clicked version with a red box around the download.**

![Alt text](placeholder.jpeg)

2.	Unzip the STM32CubeIDE project file.

**Insert image of right clicked file with unzip highlighted**

![Alt text](placeholder.jpeg)

3.	Open the STM32CubeIDE and navigate to File -> Open Project from File System… -> Directory

**Insert Image showing this location**

![Alt text](placeholder.jpeg)

4.	Find the project file in your download location commonly in “Downloads” folder.

**Insert image showing the selection of the project downloaded**

![Alt text](placeholder.jpeg)

5.	Expand the “Analog” category on the left side of the window and click on “ADC1”.

    This is the configuration for the built in analogue to digital converter “ADC1”. This hardware peripheral measures analogue voltages and digitises them, storing the conversion result in a register for use in software. This allows you to write software that can respond to changing voltages or values from sensors that output their result as an analogue voltage level.

6.	Scroll through the ADC1 “Mode and Configuration” tab that just appeared and search for “IN17”. Click on the drop-down     menu and select “Temperature Sensor Channel”.

    We’re going to read from the L4’s built in internal temperature sensor instead of connecting external circuity to the one of the ADC input pins.

7.	We’re also going to use a GPIO pin as an output to turn drive the Nucleo board’s built in LED. In the “Pinout view” left click on the pin connected to “PB3” LED and select “GPIO_Output”.

8. Right click on the “PB3” pin, and give it a user label: “LED”.

   Now we're going to set up the UART (universal asynchronous receiver-transmitter) serial communication peripheral which we will use to send messages to the computer to help with software development. 
    
 The “VCP” (virtual com port) TX (transmit) and RX (receive) lines in the built in ST-LINK debugger are connected are connected to the “PA2” and “PA15” pins on the L4.

9.	Left click on “PA2” and select “USART_2TX”. 

10.	Left click on “PA15” and select “USART_2RX”.

11.	Now look to the left hand side and select the “connectivity” drop down menu and change the “mode” drop down the “Asynchronous”.

    The “PA2” and “PA15” pins in the pinout view should now turn green as they have a valid configuration. 

    You’re pinout view should look like this:

**Insert image of correct green pinout map**

![Alt text](placeholder.jpeg)

12.	Look down in the “Parameter Settings” (Middle bottom pane) for “USART2” and make a note of the “Baud Rate” in your notebook. We’ll need to set our serial console viewer to the same settings to receive the messages later. 

    You’re now finished configuring STM32 for the rest of the session!

### 2.0 C Code Fundamentals

This section aims to complete the "Hello World" of micro controllers which is blinking an LED. 

1.	Look at the left hand project file explorer tab and open *PROJECT NAME* -> core -> src -> main.c.

**Image of path and what you expect to see when you open it.**

![Alt text](placeholder.jpeg)

2.	Find the while loop within the main function!

**Image of what we're looking for**

![Alt text](placeholder.jpeg)

3.	Using a combination of these commands and variables sequence code within the while loop to blink the led. 
    *Remember to comment each line to explain it's functionality*

| Function/ Variable | Description |
| ----------- | ----------- |
| HAL_GPIO_WritePin(*GPIO PORT* , *PIN*, *PIN_STATE*) | Set the state of a GPIO PIN |
| HAL_Delay (*TIME*) | Delay function in milliseconds |
| LED_GPIO_Port | Variable for LED GPIO port |
| LED_Pin | Variable for LED pin |
| GPIO_PIN_RESET | Variable for OFF state of a GPIO pin |
| GPIO_PIN_SET | Variable for ON state of a GPIO pin |

**NOTE: DON'T FORGET YOUR SEMICOLONS!!!**

<details>
  <summary>Example Solution</summary>

```c
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET); // Set LED to OFF
    HAL_Delay (1000);   // Insert delay 1000 ms
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET); // Set LED to ON
    HAL_Delay (1000);   // Insert delay 1000 ms 
```
</details>

## 3.0 ADC Implementation

1. Comment out the LED blink code in the while loop using:
```c
    /*
    Code in here
    /*
```
2. Insert a line to start the ADC using the blocking HAL function: 
```c
    HAL_ADC_Start(&hadc1);
```



