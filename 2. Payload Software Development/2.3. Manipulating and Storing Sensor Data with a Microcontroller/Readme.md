# Saving Data to A MicroSD Card

# Objectives
1. Configure the SPI subsystem on the microcontroller to communicate with an SD card.
2. Write sensor data to the SD card in a format that will be readable by the data processing software you will develop in the future.


# Resources:
- Tutorial by kiwih: "Tutorial: An SD card over SPI using STM32CubeIDE and FatFS" https://01001000.xyz/2020-08-09-Tutorial-STM32CubeIDE-SD-card/.
- "SPI Signal Names" by SparkFun Electronics: https://www.sparkfun.com/spi_signal_names

# Required Software & Equipment
- BinarX Rocket Payload Microcontroller Board
- MicroSD Card
- STM32CubeIDE
- STLink STM32 Debugger & Programmer

# Procedure

## 1.0 Introduction to SPI & MicroSD Card Connections

SPI or serial peripheral interface is a common serial interface for connecting sensors and peripherals to a processor or two processors or controllers to each other. It's called a serial interface as the bits of information are sent one after another down a single line or single pair of lines (instead of bits alongside each other which is called a parallel interface).

On our rocket payload microcontroller board, the MicroSD card slot is connected to the ```SPI2``` serial peripheral interface. (This microcontroller has three, ```SPI1```, ```SPI2``` and ```SPI3``` peripherals, although the board is only configured for ```SPI1```and ```SPI2```.) This SPI connection is how the payload data is transferred to the microSD card for us to process on the ground after the flight.

You can see the connections on the MicroSD card holder:
![sd card connections](./figures/micro_sd_card_connections.png)

You can also see that SPI1 and SPI2 are broken out to the female header connectors that go to your payload prototyping board:
![header connections](./figures/header_connections.png)

It is possible to connect multiple devices to one SPI peripheral by using a separate GPIO pin for each device's CS line, but for your payloads we recommend you keep things simple, and use ```SPI1``` for any sensors and leave ```SPI2``` for just the microSD card.

You can see how all these lines connect to the microcontroller itself below. We call this assignment the "pin mapping":

![microcontroller io connections](./figures/micro_io_connections.png)

> It's important that the pin mapping in your software (specifically the STM32CubeMX .ioc file inside STM32CubeIDE) match up with the board, otherwise your software won't work as expected.

The reason that we have to do this step is because the STM32 microcontrollers offer a lot of flexibility in terms of which pins the built in peripherals are connected to. They can be remapped in software, which helps maximise the usefulness of the constrained number of pins, but does mean that you have to keep track of how you have have connected them in hardware and make sure the software pin mapping matches up.

The fully mapped out pin assignment in CubeMX for the BinarX Rocket Payload Microcontroller Board looks like this:
![cubemx full pin mapping](cubemx_full_pin_mapping.png)

It's not required that you assign all of these pins in CubeMX (the intialisation code generator that is part of STM32CubeIDE), however. You only need to assign the pins that are used by your design. At a minimum, this would be:

- the HSE oscillator pins as the HSE oscillator is being used as your clock source,
- the SWD pins for programming and debugging,
- the USART1 pins for the debug UART so you can get debugging messages from your board during software development,
- the SPI2 & card detect pins for the microSD card holder, so you can save your payload data to the microSD card, and
- whatever pins are required for your payload sensor (likely ADC, I2C or SPI pins).

You should have configured these pins in CubeMX in the previous sessions: [2.1. Getting Started with STM32 Microcontroller Programming](/2.%20Payload%20Software%20Development/2.1.%20Getting%20Started%20with%20STM32%20Microcontroller%20Programming/Readme.md) and [2.2. Reading Sensors With a Microcontroller](/2.%20Payload%20Software%20Development/2.2.%20Reading%20Sensors%20With%20a%20Microcontroller/Readme.md).

## 1.0 Configure the SPI Hardware for operation with a MicroSD Card

1. Configure the SPI2 periphral for "Full Duplex Controller" (formerly known as "Full Duplex Master") with no chip select ("CS") signal (sometimes formerly referred to as slave select, "SS" or "NSS")

    ![SPI2 Pins for MicroSD](figures/SPI2_pins_for_microSD.png)

   You can find an article explaining SPI signal names and why they have changed recently here if you are interested: https://www.sparkfun.com/spi_signal_names.

2. Set the "Data Size" to "8 bits":

    ![8 Bit Data Size](figures/8_bit_data_size.png)

3. Configure PB12 as a "GPIO Output" and give it the label "uSD_CS":

    ![uSD CS Pin GPIO Mode](figures/uSD_CS_pin_GPIO_mode.png)

4. Set the prescaler to 128, which should give you a baud rate of 125 KBits/s (with the 16 MHz, HSE oscillator):

    ![SPI Prescaler](figures/SPI_prescaler.png)

5. If you need to Move over to the "Clock Configuration" tab, and check that you're click "Resolve Clock Issues" if needed.

6. In the left hand side, scroll down to "Middleware and Software Packs", click on FATFS:

    ![FATFS Middleware](figures/FATFS_middleware.png)

    then enable it by ticking "User-defined" in the middle pane:

    ![FATFS User Defined Tick Box](figures/FATFS_user_defined.png)

7. Save, and generate code.

<video loop src="https://github.com/BinarX-Curtin/School-Holiday-Program/assets/12658669/448ba34d-c790-4e87-bf11-20757312fbbc">  video </video> 


https://github.com/BinarX-Curtin/School-Holiday-Program/assets/12658669/448ba34d-c790-4e87-bf11-20757312fbbc


