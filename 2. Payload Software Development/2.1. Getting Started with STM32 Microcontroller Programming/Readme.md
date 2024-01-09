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

A while loop allows for a piece of code to be continuously looped until a preset condition is met.
A simple demonstration of a while loop is incrementing a number:
```c++
x = 0;
while (x <= 10)
{
    x = x + 1;
}
```
The previous code block first sets x to 0 then enters into the while loop where it checks if the statement (x < 10) is true. If the statement is true then the code inside the while loop will be run, which in this case is to
increment x by one. Once the program reaches the bottom of the while loop it will jump back to the top and check if the previously mentioned statement is still true now that changes have been made. The while loop will continue to run and x will continue to be incremented until the while statement is no longer true; when x is greater than 10. If the while loop statment is not true the while loop will be skipped over. 

**DO WHILE**

A do while loop is very similar to while loop with the key difference being that a do while loop will always run through the loop at least once even if the loop statement is not true/met. 
For example:
```c++
x = 15;
do
{
    x = x + 1;
} while (x <= 10);
```
In the above code block example the program will first set x to 15, increment x by 1 and then check if the statement (x <= 10) is true. In this case as the do while statement is false the loop will only be ran once. 

**FOR**

A for loop is a method of looping code that allows the program to be looped a specific amount of times. Where a while loop and a do while loop can run on continuously until the loop's statements/conditions are met, a for loop can have a chosen number of loops.
For example:
```c++
for (int x = 0; x < 5; x = x + 1)
{
    printf("Hello There");
}
```
The for loop can be broken down into 3 main statements/conditions.

   for (_statement1_; _statement2_; _statement3)

statement1 is the starting condition of the loop, in the previous example x was intialised as an integer number and set to 0.

statement2 is the ending condition of the lopp, as seen in the previous example the loop will continue until x is greater than 3.

statement3 is the incrementing value of the loop, once the loop reaches the bottom it will increment by the amount set by statement3, in the previous example x will be increased by one once it reaches the bottom of the loop.

Therefore the previous for loop will print Hello There 5 times. 

Although the previous for loop has been written to show an incremental change in x. The loop can be modified to allow for a decremental change in x.

```c++
for (int x = 10; x > 5; x = x - 1)
{
    printf("Hello There");
}
```

