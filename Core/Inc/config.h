/*
 * ecliptar_steering.h
 *
 *  Created on: Mar 15, 2021
 *      Author: merlin
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "main.h"
#include "adc.h"
#include "crc.h"
#include "dma.h"
#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

#include "system.h"
#include "control.h"
#include "drive.h"
#include "rc.h"

#include "../../RTOS/rtos.h"

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_TIM;
  PeriphClkInitStruct.TIMPresSelection = RCC_TIMPRES_ACTIVATED;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM12 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM12) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

HAL_StatusTypeDef	HALFramework_Config()
{
	  /* Configure Flash prefetch, Instruction cache, Data cache */
	#if (INSTRUCTION_CACHE_ENABLE != 0U)
	  __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
	#endif /* INSTRUCTION_CACHE_ENABLE */

	#if (DATA_CACHE_ENABLE != 0U)
	  __HAL_FLASH_DATA_CACHE_ENABLE();
	#endif /* DATA_CACHE_ENABLE */

	#if (PREFETCH_ENABLE != 0U)
	  __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
	#endif /* PREFETCH_ENABLE */

	  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	  HAL_InitTick(TICK_INT_PRIORITY);

	  /* Init the low level hardware */
	  HAL_MspInit();

	  return HAL_OK;
}

void 	Peripherals_Config()
{
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_UART5_Init();
  MX_I2C1_Init();
  MX_CRC_Init();
  MX_UART4_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_RTC_Init();
  MX_ADC3_Init();
}

void 	DriveSystem_Config()
{
	Drive->LeftEcliptor.DefineAreas(580, 620, 690, 600, 735, 860, 735);
	Drive->LeftEcliptor.DefineRotator(640, 2340, 1490, 20, 180, normal);
	Drive->LeftEcliptor.Attach(LeftEcliptor, new PwmTimer(&htim1, TIM_CHANNEL_1),
							   new PwmTimer(&htim3, TIM_CHANNEL_1),
							   new AdcData(&hadc1, ADC_CHANNEL_10, 10),
							   625, 870, 660, 760, 15, 100, 2999);

	Drive->RightEcliptor.DefineAreas(580, 620, 690, 600, 735, 860, 735);
	Drive->RightEcliptor.DefineRotator(620, 2320, 1470, 20, 180, reverse);
	Drive->RightEcliptor.Attach(RightEcliptor, new PwmTimer(&htim1, TIM_CHANNEL_2),
								new PwmTimer(&htim3, TIM_CHANNEL_2),
								new AdcData(&hadc2, ADC_CHANNEL_11, 10),
								625, 870, 660, 760, 15, 100, 2999);

	Drive->Motor.Attach(new PwmTimer(&htim2, TIM_CHANNEL_3), 30, 100, 4999);

	uint32_t chnls[2]	{ ADC_CHANNEL_10, ADC_CHANNEL_11 };
	Drive->Init(new AdcData(&hadc3, 2, chnls, 128));
}

void 	RemoteController_Config()
{
	uint16_t rcMinPulses[10] { 910, 910, 910, 910, 910, 910, 1100, 1100, 1100, 1100 };
	uint16_t rcMidPulses[10] { 1500, 1500, 910, 1500, 910, 910, 1100, 1100, 1100, 1100 };
	uint16_t rcMaxPulses[10] { 2090, 2090, 2090, 2090, 2090, 2090, 1900, 1900, 1900, 1900 };
	RC->Attach(10, &huart5, rcMinPulses, rcMidPulses, rcMaxPulses);
}

void ControlSystem_Config()
{
	RemoteControl* rc = new RemoteControl(RC);

	EcliptorController* leftEcliptCtrl = new EcliptorController(&Drive->LeftEcliptor, 1);
	leftEcliptCtrl->assign(0, rc->Channels[2], rc->Channels[3], left, rc->Channels[1],	rc->Channels[0], right);

	EcliptorController* rightEcliptCtrl = new EcliptorController(&Drive->RightEcliptor, 2);
	rightEcliptCtrl->assign(0, rc->Channels[2], rc->Channels[3], right,	rc->Channels[1], rc->Channels[0], left);

	MotorController *motor = new MotorController(&Drive->Motor, 3);
	motor->assign(0, rc->Channels[4]);

	LightsController *lights = new LightsController(FBEAM_GPIO_Port, TBEAM_GPIO_Port);
	lights->assign(1, FBEAM_Pin, rc->Channels[1], TBEAM_Pin, rc->Channels[0]);

	FpvController *fpv = new FpvController(FPV_GPIO_Port);
	fpv->Assign(0, FPV_Pin, rc->Channels[9]);

	ControlSystem *cs = new ControlSystem(rc, 7, 9);

	ControlSystems->Add(cs, 0);
}

void 	SerialUSB_Config()
{

	if(SerialUSB == NULL)
	{
		SerialUSB = new SerialDevice(&huart2);
		SerialUSB->Open();
	}

	setPrinter(SerialUSB);
	setScanner(SerialUSB);
}

void 	System_Config()
{
	SystemClock_Config();

	PeriphCommonClock_Config();

	HALFramework_Config();

	Peripherals_Config();

	SerialUSB_Config();

	__disable_interrupts();

	DriveSystem_Config();

	RemoteController_Config();

	ControlSystem_Config();

	__enable_interrupts();

	//MX_ThreadX_Init();
}

void 	Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
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


#endif /* _CONFIG_H_ */
