/**
  *********************************************************************
  * @file    MainTask.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   FreeRTOS v9.0.0 + STM32 ����ģ��
  *********************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 H743 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
/*******************************************************************************
 * ������ͷ�ļ�
 ******************************************************************************/

#include <stdio.h>

#include "GUI.h"
#include "DIALOG.h"
#include "MainTask.h"
/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"

/*******************************************************************
*
*       static code
*
********************************************************************
*/
/**
  * @brief ��ͼ����
  * @note ��
  * @param 
  * @retval ��
  */
static void _Draw(int x0, int y0, int x1, int y1, int i)
{
  char buf[] = {0};

  /* ���ƾ��α��� */
  GUI_SetColor(GUI_BLUE);
	GUI_FillRect(x0, y0, x1, y1);
  
  /* �����ı� */
	GUI_SetFont(GUI_FONT_D64);
  GUI_SetTextMode(GUI_TEXTMODE_XOR);
  sprintf(buf, "%d", i);
	GUI_DispStringHCenterAt(buf, x0 + (x1 - x0)/2, (y0 + (y1 - y0)/2) - 32);
}

/**
  * @brief �ڴ��豸��ʾ����
  * @note ��
  * @param ��
  * @retval ��
  */
static void _DemoMemDev(void)
{
  GUI_MEMDEV_Handle hMem = 0;
	int i = 0;
  
	/* ���ñ���ɫ */
  GUI_SetBkColor(GUI_BLACK);
  GUI_Clear();
  
	/* ��ʾ��ʾ���� */
  GUI_SetColor(GUI_WHITE);
  GUI_SetFont(GUI_FONT_32_ASCII);
  GUI_DispStringHCenterAt("MEMDEV_MemDev - Sample", 400, 5);
  GUI_SetFont(GUI_FONT_24_ASCII);
  GUI_DispStringHCenterAt("Shows the advantage of using a\nmemorydevice", 400, 45);
  GUI_SetFont(GUI_FONT_16_ASCII);
  GUI_DispStringHCenterAt("Draws the number\nwithout a\nmemory device", 100, 90);
  GUI_DispStringHCenterAt("Draws the number\nusing a\nmemory device", 680, 90);
  
  /* �����ڴ��豸 */
  hMem = GUI_MEMDEV_Create(600, 150, 150, 100);
  
	while (1)
	{
    /* ֱ�ӻ��� */
    _Draw(25, 150, 175, 250, i);
    
    /* �����ڴ��豸 */
    GUI_MEMDEV_Select(hMem);
    /* ���ڴ��豸�л���ͼ�� */
    _Draw(600, 150, 750, 250, i);
    /* ѡ��LCD */
    GUI_MEMDEV_Select(0);
    /* ���ڴ��豸�е����ݸ��Ƶ�LCD */
    GUI_MEMDEV_CopyToLCDAt(hMem, 600, 150);

		GUI_Delay(40);
    i++;
		if (i > 999)
		{
			i = 0;
		}
	}
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/**
  * @brief GUI������
  * @note ��
  * @param ��
  * @retval ��
  */
void MainTask(void)
{
	/* �����ڴ��豸��ʾDEMO */
  _DemoMemDev();
}

/*************************** End of file ****************************/
