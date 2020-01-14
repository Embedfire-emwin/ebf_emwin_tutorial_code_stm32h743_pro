/**
  ******************************************************************************
  * @file    GUIFont_Create.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   TTF��ʽ����emwin�����ӿ�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 H743 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
//#include "./flash/bsp_spi_flash.h"
#include "GUIFont_Create.h"
#include "ff.h"
/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"

#include <stdlib.h>


/* �洢����ʼ����־ */
static uint8_t Storage_Init_Flag = 0;

/* �ֿ����Խṹ�� */
GUI_TTF_CS cs0, cs1, cs2, cs3, cs4;

/* �ֿ����ݽṹ�� */
GUI_TTF_DATA ttf_data;

/* ����emwin���� */
GUI_FONT FONT_TTF_24;
GUI_FONT FONT_TTF_48;
GUI_FONT FONT_TTF_72;
GUI_FONT FONT_TTF_96;
GUI_FONT FONT_TTF_120;

/* �ֿ����ݻ����� */
char *TTFfont_buffer;
GUI_HMEM hFontMem;

/* �ֿ�洢·�� */
static const char FONT_STORAGE_ROOT_DIR[] = "0:";
static const char FONT_TTF_ADDR[] = "0:/Font/DroidSansFallbackFull.ttf";

/* �ֿ�洢���ļ�ϵͳʱ��Ҫʹ�õı��� */
static FIL fnew;									/* file objects */
static FATFS fs;									/* Work area (file system object) for logical drives */
static FRESULT res; 
static UINT br;            				/* File R/W count */


/**
  * @brief  ��ȡ��������
  * @note ��
  * @param res_name��Ҫ��ȡ���ļ���
  * @retval ��
  */
static void FONT_TTF_GetData(const char *res_name)
{
  if (Storage_Init_Flag == 0)
	{
		/* ����sd���ļ�ϵͳ */
		res = f_mount(&fs,FONT_STORAGE_ROOT_DIR,1);
		Storage_Init_Flag = 1;
	}
  
  /* ���ļ� */
  taskENTER_CRITICAL();
  res = f_open(&fnew , res_name, FA_OPEN_EXISTING | FA_READ);
  taskEXIT_CRITICAL();
  if(res != FR_OK)
	{
		printf("�޷��ҵ��ֿ��ļ�\r\n");
		while(1);
	}
  
  /* ����һ�鶯̬�ڴ�ռ� */
	hFontMem = GUI_ALLOC_AllocZero(fnew.fsize);
	/* ת����̬�ڴ�ľ��Ϊָ�� */
	TTFfont_buffer = GUI_ALLOC_h2p(hFontMem);
  
  /* ��ȡ���� */
  taskENTER_CRITICAL();
	res = f_read( &fnew, TTFfont_buffer, fnew.fsize, &br );
  taskEXIT_CRITICAL();
  if(res != FR_OK)
	{
		printf("�޷���ȡ�ֿ��ļ�\r\n");
		while(1);
	}
  f_close(&fnew);
  
  /* �������� */
  ttf_data.pData = TTFfont_buffer;
  ttf_data.NumBytes = fnew.fsize;
  
  /* ����������� */
  cs0.pTTF = &ttf_data;
  cs0.PixelHeight = 24;
  cs0.FaceIndex = 0;
  
  cs1.pTTF = &ttf_data;
  cs1.PixelHeight = 48;
  cs1.FaceIndex = 0;
  
  cs2.pTTF = &ttf_data;
  cs2.PixelHeight = 72;
  cs2.FaceIndex = 0;
  
  cs3.pTTF = &ttf_data;
  cs3.PixelHeight = 96;
  cs3.FaceIndex = 0;
  
  cs4.pTTF = &ttf_data;
  cs4.PixelHeight = 120;
  cs4.FaceIndex = 0;
}

/**
  * @brief  ����TTF����
  * @note ��
  * @param ��
  * @retval ��
  */
void Create_TTF_Font(void)
{
  /* ��ȡ�������� */
  FONT_TTF_GetData(FONT_TTF_ADDR);
  
  /* ����TTF���� */
  GUI_TTF_CreateFontAA(&FONT_TTF_24,
                       &cs0);
  GUI_TTF_CreateFontAA(&FONT_TTF_48,
                       &cs1);
  GUI_TTF_CreateFontAA(&FONT_TTF_72,
                       &cs2);
  GUI_TTF_CreateFontAA(&FONT_TTF_96,
                       &cs3);
  GUI_TTF_CreateFontAA(&FONT_TTF_120,
                       &cs4);
}
