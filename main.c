#include "stm32f10x.h"
#include <stdio.h>
#include "stmflash.h"
#include "string.h"
#include "SysSet.h"
#define  FLASH_ADR   	0x0800FC00  		/* 存储数据Flash页首地址 128K *///63K
#define  FLASH_SOURCE	0x08008800      //34K
#define  FLASH_DEST		0x08001400      //5K   主程序执行的首地址
void GPIO_Configuration(void);
void DelectOverFlag(void);
unsigned char SysParBuffer[1024]={0};
unsigned char tt=0;
void  FeedDog()
{
	if(tt)
	{
		tt=0;
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
	}
	else		
	{
		tt=1;
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
	}
}
void  Delay (uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
typedef  void (*pFunction)(void);
pFunction Jump_To_Application;
uint32_t JumpAddress;
int main(void)
{
	uint32_t FlashData;
	GPIO_Configuration();//初始化IO口。
	FlashData=*(vu8*)(FLASH_ADR+40);	 /* 读取地址中的16位数据 */
	if(FlashData==1)
	{
		//再次计算校验和
		int i=0;
		unsigned char check=0;
		unsigned char filelenghtG8=0;
		unsigned char filelenghtD8=0;
		unsigned char SaveTotalcheck=*(vu16*)(FLASH_ADR+43);
		unsigned int filelenght=0;
		filelenghtG8=*(vu8*)(FLASH_ADR+41);
		filelenghtD8=*(vu8*)(FLASH_ADR+42);
		filelenght=filelenghtG8*256+filelenghtD8;
		for(i=0;i<filelenght;i++)//
		{
			check+=(*(vu8*)(FLASH_SOURCE+i));
		}
		if(check==SaveTotalcheck)
		{//校验和相等			
			FLASH_Unlock();		/* 每次擦除Flash中数据时得先解锁 */
			/*************************
			全擦除,从5K擦除到34K
			**************************/
			for(i=5;i<34;i++)  
			{
				FeedDog();
				FLASH_ErasePage(1024*i);
			}
			/*************************
			覆盖27K扇区29*1024/4=6912=0x1D00
			**************************/
			for(i=0;i<0x7400;i+=4)   
			{
				FeedDog();
				FlashData=*(vu32*)(FLASH_SOURCE+i);
				FLASH_ProgramWord(i+FLASH_DEST,FlashData);   /* 写16位半字 */
			}
			/*************************
			全擦除,从34K擦除到63K
			**************************/
			for(i=34;i<63;i++)  
			{
				FeedDog();
				FLASH_ErasePage(1024*i);
			}
			DelectOverFlag();
			FLASH_Lock();							   /* 上锁 */
		}
	}
	//上边是完成了程序的检测或者覆盖
	/**************************************************
	以下一段为汇编语句跳转主程序的执行地址其中
	FLASH_DEST为主程序的首地址
	****************************************************/
	if (((*(__IO uint32_t*)FLASH_DEST) & 0x2FFE0000 ) == 0x20000000)
	{
		Delay(1000);
		FeedDog();
		JumpAddress = *(__IO uint32_t*) (FLASH_DEST+4);//5K+4
		Jump_To_Application = (pFunction) JumpAddress;
		__set_MSP(*(__IO uint32_t*) FLASH_DEST);
		Jump_To_Application();
	}
	while (1)
	{//如果程序无效，则在此处死循环，等待看门狗复位
	}
}
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE); 						 							 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}
void DelectOverFlag()
{
    STMFLASH_Read(FLASH_ADR,(u16 *)SysParBuffer, 512);
    memset(&SysParBuffer[40],0,4) ;
	  STMFLASH_Write(FLASH_ADR, (u16 *)&SysParBuffer, 512);  
}
