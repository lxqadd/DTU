#ifndef __SYSSET_H
#define __SYSSET_H
//#include "sys.h"
#include <stdio.h>
#define IDSet	0x81
#define IPSet	0x82
#define PORTSet	0x83
#define Version	1.0
extern unsigned char SysParBuffer[1024];
#define FrameHeadSetAddr  0 
#define FrameHeadSetLength	2
#define IDSetAddr	FrameHeadSetAddr+FrameHeadSetLength
#define IDSetLength	12
#define IPSetAddr	IDSetAddr + IDSetLength
#define IPSetLength	16
#define PORTSetAddr	IPSetAddr + IPSetLength
#define PORTSETLengh	6
#define AD1SetAddr	PORTSetAddr + PORTSETLengh
#define AD1SETLengh	2
#define AD2SetAddr	AD1SetAddr + AD1SETLengh
#define AD2SETLengh	2
#define UpdataedSetAddr	AD2SetAddr + AD2SETLengh
#define UpdataedSETLengh	1
#define Total_DateLenSetAddr	UpdataedSetAddr + UpdataedSETLengh
#define Total_DateLenSETLengh	2
#define Total_checkSetAddr	Total_DateLenSetAddr + Total_DateLenSETLengh
#define Total_checkSETLengh	1
#define GasStaticAddr	Total_checkSetAddr + Total_checkSETLengh
#define GasStaticLengh	2
#define Tail_frameSetAddr  GasStaticAddr+ GasStaticLengh
#define Tail_frameSetLength	2

void SysSetID(unsigned char *str,unsigned char length);
void SysSetIP(unsigned char *str,unsigned char length);
void SysSetPORT(unsigned char *str,unsigned char length);
void SysSetMain(unsigned char *str,unsigned short length);
void SysSetAD(unsigned char *str,unsigned char length);
void parameter5kFlash_Pro(void);
void DelectOverFlag(void);
#endif 

