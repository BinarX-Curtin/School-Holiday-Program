/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h> //for sprintf()
#include <string.h> //for strlen()
//#include <inttypes.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
static char string_buffer[201] = ""; //static character string buffer for use with sprintf serial transmission & .csv file creation

//constants for BMP390 I2C addresses and register addresses
static const uint8_t BMP390_I2C_ADDR_SDO_0 = 0x76<<1; // BMP390 I2C Address when SDO is low, shifted to the left one for use with the STM32 HAL
//static const uint8_t BMP390_I2C_ADDR_SDO_1 = 0x77<<1; // BMP390 I2C Address when SDO is high, shifted to the left one for use with the STM32 HAL
//static const uint8_t BMP390_CHIP_ID_REG_ADDR = 0x00; //BMP390 chip ID register address
static const uint8_t BMP390_PRESSURE_REG_ADDR = 0x04; //BMP390 pressure register (start) address
static const uint8_t BMP390_PRESSURE_NUM_BYTES = 3; //BMP390 number of bytes for pressure data
//static const uint8_t BMP390_TEMP_REG_ADDR = 0x07; //BMP390 temperature register (start) address
static const uint8_t BMP390_TEMP_NUM_BYTES = 3; //BMP390 number of bytes for temperature data
static const uint8_t BMP390_PWR_CTRL_REG_ADDR = 0x1B; //BMP390 PWR_CTRL register for mode setting & pressure & temperature enable bits
static const uint8_t BMP390_SENSORTIME_REG_ADDR = 0x0C; //BMP390 SENSORTIME register
static const uint8_t BMP390_SENSORTIME_NUM_BYTES = 3; //BMP390 number of bytes for sensortime data

//constants for PWR_CTRL register
static const uint8_t BMP390_NORMAL_MODE = 0x30; //bits to enable normal mode operation
static const uint8_t BMP390_PRESS_EN = 0x01; //bits to enable pressure measurements
static const uint8_t BMP390_TEMP_EN = 0x02; //bits to enable temperate measurements

//constants for calibration registers in non-volatile memory
static const uint8_t BMP390_NVM_CALIBRATION_REG_ADDR = 0x31;// BMP390 NVM register address for calibration data
static const uint8_t BMP390_NVM_PAR_CALIBRATIO_NUM_BYTES = 20; // BMP390 number of bytes for calibration data

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  HAL_Delay(2000); //delay to prevent double printing when programming

  sprintf(string_buffer, "STM32L4 Booted\r\n"); //load serial string buffer with serial number
  HAL_UART_Transmit(&huart1, (uint8_t *)string_buffer, strlen(string_buffer), 10); //transmit serial_string with a 10ms timeout using USART1


  uint8_t i2c_buf[22];


//	static uint8_t chip_id = 0xFF;
	static uint32_t pressure = 0;
	static uint32_t temperature = 0;
	static uint32_t sensor_time = 0;

	//wait until I2C device responds
	while(HAL_I2C_IsDeviceReady(&hi2c1, BMP390_I2C_ADDR_SDO_0, 1, HAL_MAX_DELAY));

	//assemble I2C buffer to put sensor in normal mode and enable temperature and pressure sensing
	i2c_buf[0] = BMP390_PWR_CTRL_REG_ADDR; //register address
  i2c_buf[1] = BMP390_NORMAL_MODE | BMP390_PRESS_EN | BMP390_TEMP_EN;
  HAL_I2C_Master_Transmit(&hi2c1, BMP390_I2C_ADDR_SDO_0, i2c_buf, 2, HAL_MAX_DELAY);

  //obtain calibration data from chips non-volatile memory
  HAL_I2C_Master_Transmit(&hi2c1, BMP390_I2C_ADDR_SDO_0, (uint8_t *)&BMP390_NVM_CALIBRATION_REG_ADDR, 1, HAL_MAX_DELAY);
  HAL_I2C_Master_Receive(&hi2c1, BMP390_I2C_ADDR_SDO_0, i2c_buf, BMP390_NVM_PAR_CALIBRATIO_NUM_BYTES, HAL_MAX_DELAY);

  //print calibration data
  sprintf(string_buffer, "nvm_t1 = %u\r\n", i2c_buf[0] | i2c_buf[1]<<8); //load serial string buffer
  HAL_UART_Transmit(&huart1, (uint8_t *)string_buffer, strlen(string_buffer), 10); //transmit serial_string with a 10ms timeout using USART1

  sprintf(string_buffer, "nvm_t2 = %u\r\n", i2c_buf[2] | i2c_buf[3]<<8); //load serial string buffer
  HAL_UART_Transmit(&huart1, (uint8_t *)string_buffer, strlen(string_buffer), 10); //transmit serial_string with a 10ms timeout using USART1

  sprintf(string_buffer, "nvm_t3 = %i\r\n", (int8_t)i2c_buf[4]); //load serial string buffer
  HAL_UART_Transmit(&huart1, (uint8_t *)string_buffer, strlen(string_buffer), 10); //transmit serial_string with a 10ms timeout using USART1

  sprintf(string_buffer, "nvm_p1 = %u\r\n", (int16_t)(i2c_buf[5] | i2c_buf[6]<<8)); //load serial string buffer
  HAL_UART_Transmit(&huart1, (uint8_t *)string_buffer, strlen(string_buffer), 10); //transmit serial_string with a 10ms timeout using USART1

  sprintf(string_buffer, "nvm_p2 = %u\r\n", (int16_t)(i2c_buf[7] | i2c_buf[8]<<8)); //load serial string buffer
  HAL_UART_Transmit(&huart1, (uint8_t *)string_buffer, strlen(string_buffer), 10); //transmit serial_string with a 10ms timeout using USART1

  sprintf(string_buffer, "nvm_p3 = %i\r\n", (int8_t)i2c_buf[9]); //load serial string buffer
  HAL_UART_Transmit(&huart1, (uint8_t *)string_buffer, strlen(string_buffer), 10); //transmit serial_string with a 10ms timeout using USART1

  sprintf(string_buffer, "nvm_p4 = %i\r\n", (int8_t)i2c_buf[10]); //load serial string buffer
  HAL_UART_Transmit(&huart1, (uint8_t *)string_buffer, strlen(string_buffer), 10); //transmit serial_string with a 10ms timeout using USART1

  sprintf(string_buffer, "nvm_p5 = %u\r\n", (uint16_t)(i2c_buf[11] | i2c_buf[12]<<8)); //load serial string buffer
  HAL_UART_Transmit(&huart1, (uint8_t *)string_buffer, strlen(string_buffer), 10); //transmit serial_string with a 10ms timeout using USART1

  sprintf(string_buffer, "nvm_p6 = %u\r\n", (uint16_t)(i2c_buf[13] | i2c_buf[14]<<8)); //load serial string buffer
  HAL_UART_Transmit(&huart1, (uint8_t *)string_buffer, strlen(string_buffer), 10); //transmit serial_string with a 10ms timeout using USART1

  sprintf(string_buffer, "nvm_p7 = %i\r\n", (int8_t)i2c_buf[15]); //load serial string buffer
  HAL_UART_Transmit(&huart1, (uint8_t *)string_buffer, strlen(string_buffer), 10); //transmit serial_string with a 10ms timeout using USART1

  sprintf(string_buffer, "nvm_p8 = %i\r\n", (int8_t)i2c_buf[16]); //load serial string buffer
  HAL_UART_Transmit(&huart1, (uint8_t *)string_buffer, strlen(string_buffer), 10); //transmit serial_string with a 10ms timeout using USART1

  sprintf(string_buffer, "nvm_p9 = %u\r\n", (int16_t)(i2c_buf[17] | i2c_buf[18]<<8)); //load serial string buffer
  HAL_UART_Transmit(&huart1, (uint8_t *)string_buffer, strlen(string_buffer), 10); //transmit serial_string with a 10ms timeout using USART1

  sprintf(string_buffer, "nvm_p10 = %i\r\n", (int8_t)i2c_buf[19]); //load serial string buffer
  HAL_UART_Transmit(&huart1, (uint8_t *)string_buffer, strlen(string_buffer), 10); //transmit serial_string with a 10ms timeout using USART1

  sprintf(string_buffer, "nvm_p11 = %i\r\n", (int8_t)i2c_buf[20]); //load serial string buffer
  HAL_UART_Transmit(&huart1, (uint8_t *)string_buffer, strlen(string_buffer), 10); //transmit serial_string with a 10ms timeout using USART1

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	//	ret = HAL_I2C_Master_Transmit(&hi2c1, BMP390_I2C_ADDR_SDO_0, &BMP390_CHIP_ID_REG_ADDR, 1, HAL_MAX_DELAY);
//	HAL_I2C_Master_Receive(&hi2c1, BMP390_I2C_ADDR_SDO_0, &chip_id, 1, HAL_MAX_DELAY);

  //read pressure and temperature from sensor (register addresses are contiguous, so add the number of bytes to read)
	HAL_I2C_Master_Transmit(&hi2c1, BMP390_I2C_ADDR_SDO_0, (uint8_t *)&BMP390_PRESSURE_REG_ADDR, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&hi2c1, BMP390_I2C_ADDR_SDO_0, i2c_buf, BMP390_PRESSURE_NUM_BYTES+BMP390_TEMP_NUM_BYTES, HAL_MAX_DELAY);
	pressure = i2c_buf[0] | i2c_buf[1]<<8 | i2c_buf[2]<<16;
	temperature = i2c_buf[3] | i2c_buf[4]<<8 | i2c_buf[5]<<16;

	//read the sensor time
	HAL_I2C_Master_Transmit(&hi2c1, BMP390_I2C_ADDR_SDO_0, (uint8_t *)&BMP390_SENSORTIME_REG_ADDR, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&hi2c1, BMP390_I2C_ADDR_SDO_0, i2c_buf, BMP390_SENSORTIME_NUM_BYTES, HAL_MAX_DELAY);
	sensor_time = i2c_buf[0] | i2c_buf[1]<<8 | i2c_buf[2]<<16;

//    sprintf(string_buffer, "BMP390 serial number:%02X.\r\n", chip_id); //load serial string buffer with serial number
    sprintf(string_buffer, "ST:%lu, P:%lu, T:%lu\r\n", sensor_time, pressure, temperature); //load serial string buffer with serial number
    HAL_UART_Transmit(&huart1, (uint8_t *)string_buffer, strlen(string_buffer), 10); //transmit serial_string with a 10ms timeout using USART1

    HAL_Delay(500);

    //    sprintf(string_buffer, "I2C transmit ret: %d\r\n", ret); //load serial string buffer with serial number
    //    HAL_UART_Transmit(&huart1, (uint8_t *)string_buffer, strlen(string_buffer), 10); //transmit serial_string with a 10ms timeout using USART1

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00008BFF;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
