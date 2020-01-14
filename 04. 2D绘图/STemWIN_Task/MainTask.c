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

#include <stdlib.h>

#include "./usart/bsp_debug_usart.h"

/*******************************************************************************
 * ȫ�ֱ���
 ******************************************************************************/
GUI_RECT BasicRect = {10, 10, 100, 105};
static const unsigned aValues[] = {100, 135, 190, 240, 340, 360};
static const GUI_COLOR aColor[] = {GUI_BLUE, GUI_GREEN, GUI_RED,
                                   GUI_CYAN, GUI_MAGENTA, GUI_YELLOW};
static const char QR_TEXT[] = "http://www.firebbs.cn";
static const GUI_POINT _aPointArrow[] = {
  {  0,   0 },
  {-40, -30 },
  {-10, -20 },
  {-10, -70 },
  { 10, -70 },
  { 10, -20 },
  { 40, -30 },
};
static const GUI_POINT DashCube_BackPoint[] = {
		{ 76 , 104 },
		{ 176, 104 },
		{ 176,   4 },
		{  76,   4 }
};
static const GUI_POINT DashCube_LeftPoint[] = {
		{ 40, 140 },
		{ 76, 104 },
		{ 76,   4 },
		{ 40,  40 }
};
static const GUI_POINT DashCube_BottonPoint[] = {
		{  40, 140 },
		{ 140, 140 },
		{ 176, 104 },
		{  76, 104 }
};
static const GUI_POINT DashCube_TopPoint[] = {
		{  40, 40 },
		{ 140, 40 },
		{ 176,  4 },
		{  76,  4 },
};
static const GUI_POINT DashCube_RightPoint[] = {
		{ 140, 140 },
		{ 176, 104 },
		{ 176,   4 },
		{ 140,  40 },
};
static const 	GUI_POINT DashCube_FrontPoint[] = {
		{  40, 140},
		{ 140, 140},
		{ 140,  40},
		{  40,  40},
};

/*******************************************************************************
 * ����
 ******************************************************************************/
/**
  * @brief ��ͼ��ͼ����
  * @note ��
  * @param x0����ͼԲ�ĵ�x����
  *        y0����ͼԲ�ĵ�y����
  *        r����ͼ�뾶
  * @retval ��
  */
static void Pie_Chart_Drawing(int x0, int y0, int r)
{
	int i, a0 = 0, a1 = 0;
	
	for(i = 0; i < GUI_COUNTOF(aValues); i++)
	{
		if(i == 0) a0 = 0;
		else a0 = aValues[i - 1];
		a1 = aValues[i];	
		GUI_SetColor(aColor[i]);
		GUI_DrawPie(x0, y0, r, a0, a1, 0);
	}
}

/**
  * @brief ��ά������
  * @note ��
  * @param pText����ά������
  *        PixelSize����ά������ɫ��Ĵ�С����λ������
  *        EccLevel��������뼶��
  *        x0����ά��ͼ����LCD������x
  *        y0����ά��ͼ����LCD������y
  * @retval ��
  */
static void QR_Code_Drawing(const char *pText, int PixelSize, int EccLevel, int x0, int y0)
{
	GUI_HMEM hQR;
	
	/* ������ά����� */
	hQR = GUI_QR_Create(pText, PixelSize, EccLevel, 0);
	/* ���ƶ�ά�뵽LCD */
	GUI_QR_Draw(hQR, x0, y0);
	/* ɾ����ά����� */
	GUI_QR_Delete(hQR);
}

/**
  * @brief 2D��ͼ����
  * @note ��
  * @param ��
  * @retval ��
  */
/* ���ڴ�Ŷ������ת��ĵ��б� */ 
GUI_POINT aArrowRotatedPoints[GUI_COUNTOF(_aPointArrow)];
static void _2D_Graph_Drawing(void)
{
	I16 aY[125] = {0};
	int i;
  float pi = 3.1415926L;
  float angle = 0.0f;
	
	/* ���Ƹ��־��� */
	GUI_SetColor(GUI_GREEN);
	GUI_DrawRectEx(&BasicRect);
	BasicRect.x0 += 116;
	BasicRect.x1 += 116;
	GUI_FillRectEx(&BasicRect);
  GUI_SetColor(GUI_RED);
	GUI_DrawRoundedRect(240, 10, 330, 105, 10);
	GUI_DrawRoundedFrame(352, 10, 442, 105, 10, 10);
	GUI_FillRoundedRect(468, 10, 558, 105, 10);
	GUI_DrawGradientRoundedH(584, 10, 674, 105, 10, GUI_LIGHTMAGENTA, GUI_LIGHTCYAN);
	GUI_DrawGradientRoundedV(700, 10, 790, 105, 10, GUI_LIGHTMAGENTA, GUI_LIGHTCYAN);
	
	/* �������� */
	GUI_SetPenSize(10);
  GUI_SetColor(GUI_YELLOW);
	GUI_DrawLine(10, 140, 100, 240);
	
	/* ���ƶ���� */
	GUI_SetColor(GUI_RED);
	GUI_FillPolygon(_aPointArrow, 7, 190, 205);
  /* ��ת����� */
	angle = pi / 2;
	GUI_RotatePolygon(aArrowRotatedPoints,
	                  _aPointArrow, 
                    (sizeof(_aPointArrow) / sizeof(_aPointArrow[0])),
										angle);
	GUI_FillPolygon(&aArrowRotatedPoints[0], 7, 220, 250);
  
  /* �����߿������� */
  GUI_SetPenSize(1);
	GUI_SetColor(0x4a51cc);
	GUI_SetLineStyle(GUI_LS_DOT);
	GUI_DrawPolygon(DashCube_BackPoint, 4, 210, 145);
  GUI_DrawPolygon(DashCube_LeftPoint, 4, 210, 145);
  GUI_DrawPolygon(DashCube_BottonPoint, 4, 210, 145);
  GUI_SetPenSize(2);
  GUI_SetLineStyle(GUI_LS_SOLID);
  GUI_DrawPolygon(DashCube_TopPoint, 4, 210, 145);
  GUI_DrawPolygon(DashCube_RightPoint, 4, 210, 145);
  GUI_DrawPolygon(DashCube_FrontPoint, 4, 210, 145);
                    
	/* ����Բ */
	GUI_SetColor(GUI_LIGHTMAGENTA);
	for(i = 10; i <= 70; i += 10)
	{
		GUI_DrawCircle(560, 217, i);
	}
	GUI_SetColor(GUI_LIGHTCYAN);
	GUI_FillCircle(713, 217, 70);
	
	/* ������Բ */
	GUI_SetColor(GUI_BLUE);
	GUI_FillEllipse(80, 393, 50, 70);
	GUI_SetPenSize(2);
	GUI_SetColor(GUI_WHITE);
	GUI_DrawEllipse(80, 393, 50, 10);
	
	/* ����Բ�� */
	GUI_SetPenSize(10);
	GUI_SetColor(GUI_GRAY_3F);
	GUI_DrawArc(240, 393, 80, 80, -30, 210);
	
	/* ��������ͼ */
	for(i = 0; i< GUI_COUNTOF(aY); i++)
	{
		aY[i] = rand() % 100;
	}
	GUI_SetColor(GUI_BLACK);
	GUI_DrawGraph(aY, GUI_COUNTOF(aY), 350, 340);
	
	/* ���Ʊ�ͼ */
	Pie_Chart_Drawing(560, 393, 60);
	
	/* ���ƶ�ά�� */
	QR_Code_Drawing(QR_TEXT, 5, GUI_QR_ECLEVEL_M, 650, 330);
}

/**
  * @brief Alpha���
  * @note ��
  * @param ��
  * @retval ��
  */
static void Alpha_Blending(void)
{
  /* ��ʾ�ַ� */
	GUI_SetColor(GUI_BLACK);
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetFont(GUI_FONT_32B_ASCII);
	GUI_DispStringHCenterAt("Alpha blending", 223, 203);

  /* �����Զ�Alpha��� */
  GUI_EnableAlpha(1);
	/* ��Alpha��ֵ��ӵ���ɫ�в���ʾ */
	GUI_SetColor((0xC0uL << 24) | 0xFF0000);
	GUI_FillRect(20, 20, 235, 235);
	GUI_SetColor((0x80uL << 24) | 0x00FF00);
	GUI_FillRect(110, 110, 325, 325);
	GUI_SetColor((0x40uL << 24) | 0x0000FF);
	GUI_FillRect(210, 210, 425, 425);
  /* �ر��Զ�Alpha��� */
  GUI_EnableAlpha(0);
}

/**
  * @brief GUI������
  * @note ��
  * @param ��
  * @retval ��
  */
void MainTask(void)
{
	/* ���ñ���ɫ */
	GUI_SetBkColor(GUI_WHITE);
	GUI_Clear();
	
	/* 2D��ͼ */
	_2D_Graph_Drawing();
	
	GUI_Delay(5000);
	GUI_Clear();

	/* Alpha��� */
	Alpha_Blending();
	
	while(1)
	{
		GUI_Delay(100);
	}
}
