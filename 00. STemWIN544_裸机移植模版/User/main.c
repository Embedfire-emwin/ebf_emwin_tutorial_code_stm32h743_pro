/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   ���ݴ�����-��������
  ******************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� STM32H743������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************
  */
/* ������Ӳ��bspͷ�ļ� */
#include "stm32h7xx.h"
#include "main.h"
#include "./led/bsp_led.h" 
#include "./usart/bsp_debug_usart.h"
#include "./sdram/bsp_sdram.h" 
#include "./lcd/bsp_lcd.h"
#include "./touch/bsp_i2c_touch.h"
#include "./touch/bsp_touch_gtxx.h"
#include "./touch/palette.h"
#include "./delay/core_delay.h" 
#include "./touch/palette.h"
#include "./mpu/bsp_mpu.h" 
/* STemWINͷ�ļ� */
#include "GUI.h"
#include "DIALOG.h"
#include "MainTask.h"
/* FATFS */
#include "ff.h"
#include "diskio.h"
#include "sd_diskio.h"
/*
 *************************************************************************
 *                             ȫ�ֱ�������
 *************************************************************************
 */
FATFS   fs;								/* FatFs�ļ�ϵͳ���� */
FIL     file;							/* file objects */
UINT    bw;            		/* File R/W count */
FRESULT result; 
FILINFO fno;
DIR dir;

char SDPath[4]; /* SD�߼�������·�� */

extern volatile uint32_t OS_TimeMS;

/*
*************************************************************************
*                             ��������
*************************************************************************
*/

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{   
  HAL_Init();

  /* CRC��emWinû�й�ϵ��ֻ������Ϊ�˿�ı���������
   * ����STemWin�Ŀ�ֻ������ST��оƬ���棬���оƬ���޷�ʹ�õġ�
   */
  __HAL_RCC_CRC_CLK_ENABLE();
  
	/* ϵͳʱ�ӳ�ʼ����480MHz */
	SystemClock_Config();
  SysTick_Config(SystemCoreClock / 1000);
  
  SCB_EnableICache();
  SCB_EnableDCache();
  Board_MPU_Config(0, MPU_Normal_WT, 0x20000000, MPU_REGION_SIZE_128KB);
  Board_MPU_Config(1, MPU_Normal_WT, 0x24000000, MPU_REGION_SIZE_512KB);
  /* �������ΪWT������ʹ��Alpha���ʱ��Ҫ�ֶ����D-Cache����RGB565����ղ�����Ч */
  Board_MPU_Config(2, MPU_Normal_WT, 0xD0000000, MPU_REGION_SIZE_64MB);
  
	/* LED �˿ڳ�ʼ�� */
	LED_GPIO_Config();

	/* ���ô���1Ϊ��115200 8-N-1 */
	DEBUG_USART_Config();

  /* ��ʼ�������� */
  GTP_Init_Panel();
  
	/* LCD �˿ڳ�ʼ�� */ 
	LCD_Init();
  
//  //�����������������̷�
//  FATFS_LinkDriver(&SD_Driver, SDPath);
//	/* �����ļ�ϵͳ������ʱ���SD����ʼ�� */
//  result = f_mount(&fs, "0:", 1);
//	if(result != FR_OK)
//	{
//		printf("SD����ʼ��ʧ�ܣ���ȷ��SD������ȷ���뿪���壬��һ��SD�����ԣ�\n");
//		while(1);
//	}
  
  /* ��ʼ��GUI */
	GUI_Init();
	/* ���������� */
	WM_MULTIBUF_Enable(1);

  while(1)
  {
    MainTask();
  }
}

void HAL_SYSTICK_Callback(void)
{
  static uint16_t i = 0;
  
  OS_TimeMS++;
  i++;
  if(i >= 1000)
  {
    LED3_TOGGLE;
    i = 0;
  }
}

/**
  * @brief  System Clock ����
  *         system Clock ��������: 
	*            System Clock source  = PLL (HSE)
	*            SYSCLK(Hz)           = 480000000 (CPU Clock)
	*            HCLK(Hz)             = 240000000 (AXI and AHBs Clock)
	*            AHB Prescaler        = 2
	*            D1 APB3 Prescaler    = 2 (APB3 Clock  120MHz)
	*            D2 APB1 Prescaler    = 2 (APB1 Clock  120MHz)
	*            D2 APB2 Prescaler    = 2 (APB2 Clock  120MHz)
	*            D3 APB4 Prescaler    = 2 (APB4 Clock  120MHz)
	*            HSE Frequency(Hz)    = 25000000
	*            PLL_M                = 5
	*            PLL_N                = 192
	*            PLL_P                = 2
	*            PLL_Q                = 4
	*            PLL_R                = 2
	*            VDD(V)               = 3.3
	*            Flash Latency(WS)    = 4
  * @param  None
  * @retval None
  */
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** ���õ�Դ���ø���
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** ����������ѹ�������ѹ
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** ��ʼ��CPU��AHB��APB����ʱ��
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
		while(1);
  }
  /** ��ʼ��CPU��AHB��APB����ʱ��
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
		while(1);
  }
}

void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}
/****************************END OF FILE***************************/
