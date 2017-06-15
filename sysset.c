#include "SysSet.h"
#include "string.h"
#include "stmflash.h"
#define RemoteUpdataParaSaveAddr	0x800FC00
unsigned char SysParBuffer[1024]={0};
u16 Device_value1;
u16 Device_value2;
unsigned char  Updataed =0;
u8 id[12]={0};
u32 Total_DateLen=0;//总文件长度
u8 Total_check=0;
u16 GasStatic=0;
typedef struct
{
	unsigned char IP[16];
	unsigned char port[6];
	unsigned char DateLocation;
}SIM800_Socket;
SIM800_Socket Socket_Date;
void parameter5kFlash_Pro()
{
           SysParBuffer[FrameHeadSetAddr]=0xAA;
					 SysParBuffer[FrameHeadSetAddr+1]=0x55;
					 SysSetID(id,11);
					 SysSetIP(Socket_Date.IP, 16);
					 SysSetPORT(Socket_Date.port, 6);
					 
	         SysParBuffer[AD1SetAddr]=Device_value1/256;
					 SysParBuffer[AD1SetAddr+1]=Device_value1%256;  
					 SysParBuffer[AD2SetAddr]=Device_value2/256;
					 SysParBuffer[AD2SetAddr+1]=Device_value2%256;
	
					 SysParBuffer[UpdataedSetAddr]=Updataed; 
					 SysParBuffer[Total_DateLenSetAddr]=Total_DateLen/256;
					 SysParBuffer[Total_DateLenSetAddr+1]=Total_DateLen%256;
					 SysParBuffer[Total_checkSetAddr]=Total_check;
					 SysParBuffer[GasStaticAddr]=GasStatic;
					 SysParBuffer[Tail_frameSetAddr]=0x55;
					 SysParBuffer[Tail_frameSetAddr+1]=0xAA;
           STMFLASH_Write(RemoteUpdataParaSaveAddr, (u16 *)&SysParBuffer, 25);
}
void SysSetID(unsigned char *str,unsigned char length)
{
	memset((char *)&SysParBuffer[IDSetAddr],0,IDSetLength);
	memcpy((char *)&SysParBuffer[IDSetAddr],str,length);
}
void SysSetIP(unsigned char *str,unsigned char length)
{
	memset((char *)&SysParBuffer[IPSetAddr],0,IPSetLength);
	memcpy((char *)&SysParBuffer[IPSetAddr],str,length);
}
void SysSetPORT(unsigned char *str,unsigned char length)
{
	memset((char *)&SysParBuffer[PORTSetAddr],0,PORTSETLengh);
	memcpy((char *)&SysParBuffer[PORTSetAddr],str,length);
}
//void DelectOverFlag()
//{
//	  unsigned char i;
//    STMFLASH_Read(RemoteUpdataParaSaveAddr,(u16 *)SysParBuffer, 25);  
//  	memcpy(&Socket_Date.IP, &SysParBuffer[IPSetAddr], 16);
//    memcpy(&Socket_Date.port, &SysParBuffer[PORTSetAddr], 6); 
//		memcpy(id, &SysParBuffer[IDSetAddr], 12); 
//		Device_value1=SysParBuffer[AD1SetAddr]*256+SysParBuffer[AD1SetAddr+1];
//    Device_value2=SysParBuffer[AD2SetAddr]*256+SysParBuffer[AD2SetAddr+1];	
//	  GasStatic=SysParBuffer[GasStaticAddr]*256+SysParBuffer[GasStaticAddr+1];
//		for(i=63;i<64;i++)  
//		{
//			
//				FLASH_ErasePage(1024*i);
//		}
//		Updataed=0;
//		Total_DateLen=0;
//		Total_check=0;
//		parameter5kFlash_Pro();
//}

void DelectOverFlag()
{
    STMFLASH_Read(RemoteUpdataParaSaveAddr,(u16 *)SysParBuffer, 25);  
 
}
