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
#include "MainTask.h"

/*******************************************************************************
 * ȫ�ֱ���
 ******************************************************************************/
char acText[] = "This example demostrates text wrapping";
GUI_RECT rect = {178, 290, 310, 410};
GUI_WRAPMODE aWm[] = {GUI_WRAPMODE_NONE, GUI_WRAPMODE_CHAR, GUI_WRAPMODE_WORD};

/*******************************************************************************
 * ����
 ******************************************************************************/
/**
  * @brief GUI������
  * @note ��
  * @param ��
  * @retval ��
  */
void MainTask(void)
{
	U8 i;
	
	/* ���ñ���ɫ */
	GUI_SetBkColor(GUI_BLUE);	
	GUI_Clear();
	
//	/* ������� */
//	GUI_CURSOR_Show();
	
	/* ���������С */
	GUI_SetFont(GUI_FONT_32_1);
	GUI_DispStringAt("STemWIN@EmbedFire STM32H743", 10, 10);
	
	/* ���� */
	GUI_SetPenSize(10);
	GUI_SetColor(GUI_RED);
	GUI_DrawLine(272, 120, 528, 240);
	GUI_DrawLine(272, 240, 528, 120);
	
	/* �����ı� */
	GUI_SetBkColor(GUI_BLACK);
	GUI_SetColor(GUI_WHITE);
	GUI_SetFont(GUI_FONT_24B_ASCII);
	/* ����ģʽ */
	GUI_SetTextMode(GUI_TM_NORMAL);
	GUI_DispStringHCenterAt("GUI_TM_NORMAL" , 400, 120);
	/* ��ת��ʾ */
	GUI_SetTextMode(GUI_TM_REV);
	GUI_DispStringHCenterAt("GUI_TM_REV" , 400, 120 + 24);
	/* ͸���ı� */
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_DispStringHCenterAt("GUI_TM_TRANS" , 400, 120 + 24 * 2);
	/* ����ı� */
	GUI_SetTextMode(GUI_TM_XOR);
	GUI_DispStringHCenterAt("GUI_TM_XOR" , 400, 120 + 24 * 3);
	/* ͸����ת�ı� */
	GUI_SetTextMode(GUI_TM_TRANS | GUI_TM_REV);
	GUI_DispStringHCenterAt("GUI_TM_TRANS | GUI_TM_REV", 400, 120 + 24 * 4);
	
	/* �ھ�����������ʾ�ı� */
	GUI_SetFont(GUI_FONT_24B_ASCII);
	GUI_SetTextMode(GUI_TM_TRANS);
	for(i = 0;i < 3;i++)
	{
		GUI_SetColor(GUI_WHITE);
		GUI_FillRectEx(&rect);
		GUI_SetColor(GUI_RED);
		GUI_DispStringInRectWrap(acText, &rect, GUI_TA_LEFT, aWm[i]);
		rect.x0 += 156;
		rect.x1 += 156;
	}
	
	while(1)
	{
		GUI_Delay(100);
	}
}
