# Setup of Timers in STM32

### Table of Contents <!-- omit from toc -->
- [Setup of Timers in STM32](#setup-of-timers-in-stm32)
  - [Objectives](#objectives)
  - [Requirements](#requirements)
  - [Resources](#resources)
  - [Procedure](#procedure)
    - [1. Setting up .ioc file](#1-setting-up-ioc-file)
    - [2. Basic example](#2-basic-example)

## Objectives
1. Configure the .ioc file to use internal timer
2. Understand prescalar values
3. Understand set points
4. Operate timer for data collection time stamping

## Requirements
1. Understanding of CubeMX environment.
2. STM32L4 microcontroller board
3. USB-A to USB-C cable

## Resources 
- STM32 Timers Explained: https://www.steppeschool.com/pages/blog?p=stm32-timer-stm32cubemx

## Procedure

The scope of this document is to setup a timer to collect time stamps for data collection. This means that we do not need any external clock lines to be utilised to trigger timing events. Meaning we will only be accessing the internal clock speed for reference.<br><br>Looking at the reference in [resources](#resources) about timers, it can be found that the operating frequency of a timer can be configured using the following equation:<br><br>operating_frequency = clock_frequency / (prescaler + 1)<br><br>The operating frequency you need will be determined by the sensor you are using. You can select a prescaler based upon this. For this example we will be using a prescaler of 3, to obtain an operating frequency of 20MHz. We know that our clock speed is 80MHz, based on the configuration we did in [session 2.1](/2.%20Payload%20Software%20Development/2.1.%20Getting%20Started%20with%20STM32%20Microcontroller%20Programming/Readme.md).

### 1. Setting up .ioc file

1. In the .ioc file we need to select the 'Timers' tab and select the 'TIM1' option. Here we will select the Clock source to be 'Internal Clock'. ![Alt text](/6.%20Payload%20Breakdowns/6.1%20Acclerometer/Images/image-21.png)
<br>

2. We need to look to our 'Parameter Settings' now. Here you'll be able to find the 'Prescaler' and 'Counter Period' settings. For this example, we will set the prescaler to 3, to obtain our 20MHz operating frequency. This means that every 0.00000005 seconds the timer will complete 1 tick.
        <details>
        <summary>**How did you find the seconds??**</summary>
        <br>
        To convert between seconds and Hz is easy as:<br><br>*seconds = 1 / frequency*<br><br>Which means that:<br><br>*frequency = 1 / seconds*
        </details>
<br>

3. In this example we will be keeping the counter period at the default, which is 65535. This number is the maximum value the timer counts up to, which is found by us knowing that the timer in the STM32 is a 16-bit timer. Hence: 2<sup>16</sup> - 1 = 65535 bits. Changing this value will set the ticks to occur quick, and you will need to take this into account.
<br>

4. You have now set up the timer in the .ioc file and can regenerate your code.

### 2. Basic example

These are the variables I have set up to create a working tick count.

```C++
/* USER CODE BEGIN PD */

#define timer_cap 65535

/* USER CODE END PD */
```

<br>

```C++
/* USER CODE BEGIN 0 */

int timer_value1 = 0;
int timer_value2 = 0;
int timer_ticks = 0;
//is uint32_t because the ticks will never be negative and its going to be a massive number
uint32_t timer_total = 0;

/* USER CODE END 0 */
```

<br>

```C++
/* USER CODE BEGIN 2 */

  HAL_TIM_Base_Start(&htim1); // starting the timer

  /* USER CODE END 2 */
```

<br>

```C++
/* USER CODE BEGIN WHILE */
  while (1)
  {

	    //ticks at the beginning of the read
	    timer_value1 = __HAL_TIM_GET_COUNTER(&htim1);

		/**
        *
        READING YOUR DATA AND SENDING IT TO THE SD CARD
        *
        **/ 

		//ticks at the end of the read
		timer_value2 = __HAL_TIM_GET_COUNTER(&htim1);

        //final time - inital time gives you a change in time (in ticks)
		timer_ticks = timer_value2 - timer_value1;

		//ensures that overlap is converted correctly e.i 0 - timer_cap
		if(timer_ticks <= 0)
		{
			timer_ticks =  timer_cap + timer_ticks;
		}

	    //difference between the end of the last read and the start of this read added to a running counter
		timer_total = timer_total + timer_ticks;

	}

/* USER CODE END WHILE */
```

<br>

For further details refer to your subject matter experts (SME). You will need to consider the first time you read data that the ticks achieved will be zero. This should be handled by this code but keep this in mind when troubleshooting.