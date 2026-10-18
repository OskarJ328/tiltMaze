/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "adc.h"
#include "basicTypes.h"
#include "dma.h"
#include "game.h"
#include "i2c.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "renderer.h"
#include "inputConfig.h"
#include "my_uart.h"
#include "gameplay.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LOOP_PERIOD_MS 10
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
assetManager_t assets;
renderer_t renderer;
ili9341_t ili = {
  .csPort = CS_GPIO_Port,
  .csPin = CS_Pin,
  .resPort = RESET_GPIO_Port,
  .resPin = RESET_Pin,
  .dcPort = DC_GPIO_Port,
  .dcPin = DC_Pin,
  .spiHandle = &hspi1,
  .height = ILI9341_HEIGHT,
  .width = ILI9341_WIDTH,
  .colorFormat = BGR
};
input_t input;
button_t resumeButton;
button_t pauseButton;


uart_t uart = {
  .uart_handle = &huart2
};
game_t game;

uint32_t currentTime = 0;
uint32_t previousTime = 0;

uint16_t last50fps[1000];
uint16_t fpsIdx = 0;
uint32_t lastFpsTime = 0;

uint32_t beforeLoopTime_ms = 0;
uint32_t afterLoopTime_ms = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */

  my_uart_init(&uart);

  assetManager_init(&assets);
  renderer_init(&renderer, &ili, &assets, ILI9341_WHITE);

  input_init(&input);
  inputConfig_init(&input);
  game_init(&game, &input, &renderer, &assets);


  button_t *button;
  joystick_t *joystick;
  floatVector2_t joystick_previousPosition = {0, 0};


  joystick = input_getJoystick(&input, LEFT_ANALOG);
  joystick_invertAxis_x(joystick);
  beforeLoopTime_ms = HAL_GetTick();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    currentTime = HAL_GetTick();
    if(currentTime - previousTime > LOOP_PERIOD_MS){
      input_update(&input, LOOP_PERIOD_MS);
      game_update(&game, LOOP_PERIOD_MS);
      game_render(&game);
      /*
      button = input_getButton(&input, BUTTON_RESUME);
      if(button_wasPressed(button)){
        my_uart_printf(&uart, "buttonResume was pressed\n");
      }

      button = input_getButton(&input, BUTTON_PAUSE);
      if(button_wasPressed(button)){
        my_uart_printf(&uart, "buttonPause was pressed\n");
      }

      joystick = input_getJoystick(&input, LEFT_ANALOG);

      floatVector2_t joystick_currentPosition;
      joystick_currentPosition.x = joystick_getPosition_x(joystick);
      joystick_currentPosition.y = joystick_getPosition_y(joystick);

      if(joystick_currentPosition.x > 60.0f && joystick_previousPosition.x <= 60.0f){
        my_uart_printf(&uart, "joystick: LEFT\n");
      }
      else if(joystick_currentPosition.x < -60.0f && joystick_previousPosition.x >= -60.0f){
        my_uart_printf(&uart, "joystick: RIGHT\n");
      }
      if(joystick_currentPosition.y > 60.0f && joystick_previousPosition.y <= 60.0f){
        my_uart_printf(&uart, "joystick: UP\n");
      }
      else if(joystick_currentPosition.y < -60.0f && joystick_previousPosition.y >= -60.0f){
        my_uart_printf(&uart, "joystick: DOWN\n");
      }
      joystick_previousPosition.x = joystick_currentPosition.x;
      joystick_previousPosition.y = joystick_currentPosition.y;
      */

      previousTime = HAL_GetTick();
    }
    afterLoopTime_ms = HAL_GetTick();
    
    uint16_t fps;
    if(afterLoopTime_ms == beforeLoopTime_ms){
      fps = 999;
    }
    else{
      fps = 1000 / (afterLoopTime_ms - beforeLoopTime_ms);
      if(fps > 999){
        fps = 999;
      }
    }
    last50fps[fpsIdx] = fps;
     
    if(fpsIdx == ARRAY_SIZE(last50fps) - 1){
      uint32_t sum = 0;
      for(int idx = 0; idx <= fpsIdx; idx++){
        sum += last50fps[idx];
      }
      uint16_t avgFps = sum / ARRAY_SIZE(last50fps);
      char text[10];
      sprintf(text, "%d fps", avgFps);
      vector2_t pos = {0, 0};
      renderer_drawText(&renderer, FONT_BIG, text, pos, ILI9341_BLACK);
      fpsIdx = 0;
    }
    else{
      fpsIdx++;
    }
    beforeLoopTime_ms = HAL_GetTick();
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
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
#ifdef USE_FULL_ASSERT
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
