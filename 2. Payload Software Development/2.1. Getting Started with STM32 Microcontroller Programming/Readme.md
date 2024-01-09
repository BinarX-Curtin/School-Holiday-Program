### Table of Contents <!-- omit from toc -->
- [Objectives](#objectives)
- [Requirements](#requirements)
- [Resources](#resources)
- [Procedure](#procedure)
- [1.0 What is a Microcontroller](#10-what-is-a-microcontroller)
  - [Microcontroller vs Microprocessor](#microcontroller-vs-microprocessor)
- [2.0 STM32CubeIDE](#20-stm32cubeide)
  - [Navigating](#navigating)
  - [](#)
- [3.0 Coding in C++](#30-coding-in-c)

## Objectives
1. Understanding the purpose of a Microcontroller
1. Understanding the fundamentals of C++ code
1. Getting started with STM32CubeIDE
1. 

## Requirements
1. STM32CubeIDE installed
2. STM32L4 microcontroller board
3. USB-A to USB-C cable

## Resources
- STM32MCU Basics: https://wiki.st.com/stm32mcu/wiki/STM32StepByStep:STM32MCU_basics
-  

## Procedure
## 1.0 What is a Microcontroller
A microcontroller is a small computer on a single integrated circuit. A microcontroller contains one or more CPUs along with memory and programmable input/output peripherals.

On the other hand, a microprocessor is just an integrated circuit that contains all the functions of a central processing unit of a computer.

### Microcontroller vs Microprocessor
| Heading        |                                                                                                                                     Microcontroller                                                                                                                                      |                                                                                                                                                                                  Microprocessor |
| :------------- | :--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------: | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------: |
| Functionality  | A microcontroller is an integrated circuit designed to perform a specific task in an embedded system. It typically includes a processor, memory and input/output peripherals on a single chip. it's designed to be an all-in-one solution for controlling devices in an embedded system. | A microprocessor is essentially the CPU or processing unit of a computer system. It therefore, doesn't include the additional features a microcontroller has such as memory or I/O peripherals. |
| Applications   |                                                          Used in devices where automation of a specific task is needed. Such as home appliances, automobiles, medical devices, space craft, and other various embedded systems.                                                          |                                                                                                  Utilized when there is a high computing demand in devices such as, PC's, smartphones, servers. |
| Complexity     |                                                                                             A less complex processor but depending on the additional features could be a more complex chip.                                                                                              |                                                                                               Due to the one dimensional purpose the processing side of this chip is considerably more complex. |
| Memory/Storage |                                                                             Has small amounts of onboard memory and storage, limited but typically sufficient for the application it is designed to perform.                                                                             |                                                                                        Relies on external memory and storage allowing for greater capacity in a larger and more complex system. |


## 2.0 STM32CubeIDE


### Navigating

### 

## 3.0 Coding in C++

### What is code? <!-- omit from toc -->

**BYTES**

### why code? <!-- omit from toc -->

### Variables <!-- omit from toc -->
1. **Integers**
1. **Arrays**
1. **Strings**

### Conditional Statements <!-- omit from toc -->

Conditional statements allow for coders to have dynamic pathways throughout their code. The pathway is dependant on the dynamic variable values. Hence, there needs to be clearly defined rules to ensure consistency throughout the code. 

**Logic Statements**

Logic statements allow for the comparison between two boolean (1 and 0) values.

| Symbol | word |                                            Definition |
| :----- | ---- | ----------------------------------------------------: |
| &&     | AND  |    needs both one variables to be TRUE to output TRUE |
| \|\|   | OR   | needs at least one variable to be TRUE to output TRUE |


**Logic Conditions**

On the other hand, logic conditions allow for the comparison between singular numerical values
| Symbol |               Definition |
| :----- | -----------------------: |
| <      |                Less than |
| <=     |    Less than OR equal to |
| >      |             Greater than |
| >=     | Greater than OR equal to |
| ==     |                 Equal to |
| !=     |             NOT equal to |

1. **```if``` Statements**
    
    If statements are the most common type of conditional statements as they see if the boolean value inside the brackets is true to proceed.

    **Note:** the "if" statement sequentially checks each statement to be TRUE and follows that path first. Meaning if multiple statements would be TRUE it will enter the one closest to the top.
    Example 1
    ```c++
    if(a < b || a > b)
    {
        printf("a and b are not equal");
    }
    ```
    *find which logic condition would simplify the above statement*

    Example 2
    ```c++
    if(a > b)
    {
        printf("a > b");
    }
    else
    {
        printf("a <= b");
    }
    ```

    Example 3
    ```c++
    if(a > b && a > c)
    {
        printf("a is the largest number");
    }
    else if(b > a && b > c)
    {
        printf("b is the largest number");
    }
    else
    {
        printf("c is the largest number");
    }
    
    ```


2. **SWITCH Statements**

These are unlikely to be used within this development but if you're interested they're described effectively [here](https://www.w3schools.com/cpp/cpp_switch.asp).


### Loops <!-- omit from toc -->
loops allow for the coder to further simplify operations that need to completed multiple times. 

**WHILE**

**DO WHILE**

**FOR**
