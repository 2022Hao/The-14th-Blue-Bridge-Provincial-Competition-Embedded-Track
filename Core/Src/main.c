/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include "lcd.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
struct keys
{
	uint8_t sta;
	uint8_t judge;
	uint8_t flag;
	uint32_t time;
	uint8_t lflag;
	uint8_t kind;
};
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
char text[30];
struct keys key[4]={0,0,0,0,0,0};

uint32_t adc_value;
double adc_v;

uint32_t ccrl=0;
uint32_t frq=0;

uint8_t view=0;
uint8_t mode=0;
uint8_t kind=0;

uint8_t R=1;
uint8_t K=1;
uint8_t N=0;

float v=0;

uint32_t p=10;

uint8_t ke=0;
uint8_t kel=0;

uint32_t t=0;
uint32_t t0=0;
uint32_t t1=0;
uint32_t t2=0;
uint8_t t1f=0;
uint8_t t2f=0;

uint8_t change=0;

uint8_t h=0;

uint16_t arr=499;

float MH=0;
float ML=65525;

uint8_t arrf=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
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
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM16_Init();
  MX_ADC2_Init();
  MX_TIM17_Init();
  MX_TIM15_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim15);
	HAL_TIM_Base_Start_IT(&htim16);
	HAL_TIM_Base_Start_IT(&htim17);
	__HAL_TIM_DISABLE(&htim15);
	LCD_Init();
	LCD_Clear(Black);
  LCD_SetBackColor(Black);
  LCD_SetTextColor(White);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	__HAL_TIM_SetCounter(&htim2,0);
	__HAL_TIM_SET_AUTORELOAD(&htim2,499);
	HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_2);
	
	sprintf(text,"        DATA        ");
	LCD_DisplayStringLine(Line1, (unsigned char *)text);
	sprintf(text,"     M=L            ");
	LCD_DisplayStringLine(Line3, (unsigned char *)text);
	sprintf(text,"     P=0            ");
	LCD_DisplayStringLine(Line4, (unsigned char *)text);
	sprintf(text,"     V=0            ");
	LCD_DisplayStringLine(Line5, (unsigned char *)text);
	
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		HAL_ADC_Start(&hadc2);
		adc_value=HAL_ADC_GetValue(&hadc2);
		adc_v=(double)adc_value*3.3/4096;
//		sprintf(text,"%.2fV",adc_v);
//		LCD_DisplayStringLine(Line8, (unsigned char *)text);
		if(adc_v<=1)
		{
			p=10;
		}
		else if(adc_v>1 && adc_v<3)
		{
			p=(uint32_t)(adc_v*75.0/2.0-27.5);
		}
		else
		{
			p=85;
		}
		
		if(view!=1)
		{
			v=(float)frq*2.0*3.14*(float)R/100.0/(float)K;
		}
		
		if(v>MH)
		{
			if(t1f==0)
			{
				t1=t;
				t1f=1;
			}
			if(t-t1>=2)
			{
				MH=v;
				t1f=0;
			}
		}
		if(v<ML)
		{
			if(t2f==0)
			{
				t2=t;
				t2f=1;
			}
			if(t-t2>=2)
			{
				ML=v;
				t2f=0;
			}
		}
		
		if(key[0].flag==1)
		{
			view++;
			LCD_Clear(Black);
			if(view==3)
			{
				view=0;
			}
			if(view==1)
			{
				kind=0;
			}
			if(view==0)
			{
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
				if(mode==1)
				{
					sprintf(text,"     M=H            ");
					LCD_DisplayStringLine(Line3, (unsigned char *)text);
				}
				else
				{
					sprintf(text,"     M=L            ");
					LCD_DisplayStringLine(Line3, (unsigned char *)text);
				}
			}
			
			
			key[0].flag=0;
		}
		
		if(key[1].flag==1)
		{
			if(t-t0>=5)
			{
				change=0;
			}
			if(view==0 && change==0)
			{
				mode=!mode;
				N++;
				h=1;
				change=1;
				t0=t;
			}
			if(view==1)
			{
				kind=!kind;
			}
			key[1].flag=0;
		}
		
		
		if(view==1)
		{
			if(key[2].flag==1)
			{
				if(kind==0)
				{
					R++;
					if(R==11)
					{
						R=1;
					}
				}
				else
				{
					K++;
					if(K==11)
					{
						K=1;
					}
				}
				key[2].flag=0;
			}
			
			if(key[3].flag==1)
			{
				if(kind==0)
				{
					R--;
					if(R==0)
					{
						R=10;
					}
				}
				else
				{
					K--;
					if(K==0)
					{
						K=10;
					}
				}
				key[3].flag=0;
			}
//			else if(key[3].flag==1 && ke==1)
//			{
//				key[3].flag=0;
//			}
		}
		if(view==0 && change==0)
		{
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
		}
		if(view==0)
		{
		  if(arr%5==0 && arr/5%2==1 && ke==0)
			{
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET);
//				HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_9);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
				arrf=1;
			}
			else if(arr%5==0 && arr/5%2==0 && ke==0)
			{
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
//				HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_9);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
				arrf=1;
			}
			else if(arr%5==0 && arr/5%2==1 && ke==1)
			{
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET);
//				HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_9);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
				arrf=1;
			}
			else if(arr%5==0 && arr/5%2==0 &&ke==1)
			{
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
//				HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_9);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
				arrf=1;
			}
		}
		if(view==0 && ke==1 && arrf==0)
		{
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
		}
		
		if(view==0 && kel==1 && ke==0)
		{
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
			kel=0;
		}
		
		if(view==1)
		{
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
		}
		
		if(view==2)
		{
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
		}
		
		if((view!=0 && key[3].lflag==1) || (view==0 && ke==1 && key[3].lflag==1))
		{
			key[3].lflag=0;
		}
		
		if(view==2 || (view==0 && ke==0))
		{
			key[3].flag=0;
		}
		
		
		if(view==0)
		{
			
			if(key[3].lflag==1)
			{
				ke=1;
				key[3].lflag=0;
			}
			if(ke==1)
			{
				if(key[3].flag==1)
				{
					ke=0;
					kel=1;
					key[3].flag=0;
				}
			}
			sprintf(text,"        DATA        ");
			LCD_DisplayStringLine(Line1, (unsigned char *)text);
			if(mode==1)
			{
				
//				__HAL_TIM_DISABLE(&htim2);
				if(h==1)
				{
					__HAL_TIM_ENABLE(&htim15);
					arr=499;
					h=0;
				}
				if(ke==0)
				{
				__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,arr*p/100);
				}
//				__HAL_TIM_ENABLE(&htim2);
			}
			else
			{
				
//				__HAL_TIM_DISABLE(&htim2);
				if(h==1)
				{
					__HAL_TIM_ENABLE(&htim15);
					arr=249;
					h=0;
				}
				if(ke==0)
				{
					__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,arr*p/100);
				}
//				__HAL_TIM_ENABLE(&htim2);
			}
			sprintf(text,"     P=%d%%     ",p);
			LCD_DisplayStringLine(Line4, (unsigned char *)text);
			sprintf(text,"     V=%.1f       ",v);
			LCD_DisplayStringLine(Line5, (unsigned char *)text);
		}
		else if(view==1)
		{
			sprintf(text,"        PARA        ");
			LCD_DisplayStringLine(Line1, (unsigned char *)text);
			sprintf(text,"     R=%d            ",R);
			LCD_DisplayStringLine(Line3, (unsigned char *)text);
			sprintf(text,"     K=%d            ",K);
			LCD_DisplayStringLine(Line4, (unsigned char *)text);
		}
		else
		{
			sprintf(text,"        RECD        ");
			LCD_DisplayStringLine(Line1, (unsigned char *)text);
			sprintf(text,"     N=%d           ",N);
			LCD_DisplayStringLine(Line3, (unsigned char *)text);
			sprintf(text,"     MH=%.1f     ",MH);
			LCD_DisplayStringLine(Line4, (unsigned char *)text);
			sprintf(text,"     ML=%.1f     ",ML);
			LCD_DisplayStringLine(Line5, (unsigned char *)text);
		}
		
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV6;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
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
  /** Initializes the peripherals clocks
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC12;
  PeriphClkInit.Adc12ClockSelection = RCC_ADC12CLKSOURCE_SYSCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM16)
	{
		key[0].sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);
		key[1].sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);
		key[2].sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2);
		key[3].sta=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
		for(int i=0;i<4;i++)
		{
			switch(key[i].judge)
			{
				case 0:
				{
					if(key[i].sta==0)
					{
						key[i].judge=1;
					}
				}
				break;
				case 1:
				{
					if(key[i].sta==0)
					{
							key[i].judge=2;
					}
					else
					{
						key[i].judge=0;
					}
				}
				break;
				case 2:
				{
					key[i].time++;
					if(key[i].time<200)
					{
						key[i].kind=1;
					}
					
					else if(key[i].time>=200)
					{
						key[i].kind=2;
					}
					if(key[i].sta==1)
					{
						if(key[i].kind==1)
						{
							key[i].flag=1;
						}
						else if(key[i].kind==2)
						{
							key[i].lflag=1;
						}
						key[i].judge=0;
						key[i].kind=0;
						key[i].time=0;
					}
				}
				break;
			}
		}
	}
	
	if(htim->Instance==TIM17)
	{
		t++;
		if(t==4294967295)
		{
			t=0;
		}
	}
	
	if(htim->Instance==TIM15)
	{
		if(mode==0)
		{
			arr++;
			__HAL_TIM_SetCounter(&htim2,0);
			__HAL_TIM_SET_AUTORELOAD(&htim2,arr);
			if(arr==499)
			{
				__HAL_TIM_DISABLE(&htim15);
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
				sprintf(text,"     M=L            ");
				LCD_DisplayStringLine(Line3, (unsigned char *)text);
				arrf=0;
			}
			
			
		}
		else if(mode==1)
		{
			arr--;
			__HAL_TIM_SetCounter(&htim2,0);
			__HAL_TIM_SET_AUTORELOAD(&htim2,arr);
			if(arr==249)
			{
				__HAL_TIM_DISABLE(&htim15);
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
				arrf=0;
				sprintf(text,"     M=H            ");
				LCD_DisplayStringLine(Line3, (unsigned char *)text);
			}
			
		}
	}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM3)
	{
		ccrl=HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_2);
		__HAL_TIM_SetCounter(&htim3,0);
		frq=(170000000/170)/ccrl;
		HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_2);
	}
}
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
