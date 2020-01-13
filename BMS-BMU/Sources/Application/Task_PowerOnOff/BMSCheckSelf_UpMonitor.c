 /*=======================================================================
 *Subsystem:   裸机
 *File:        BMSCheckself_UpMonitor.C
 *Author:      Wen Yuhao
 *Description: 通信：
               接口：
               波特率：
 ========================================================================
 * History:    修改历史记录列表
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#include  "includes.h"

/*=======================================================================
 *函数名:      PhysicInitState_FltCode(SysInitState_T*) 
 *功能:        物理层初始化的状态判断
 *参数:        ptr:指向物理层初始化状态变量      
 *返回：       uint16:返回物理层初始的状态,0:代表无故障;其他:故障代码
 *说明：       物理层初始化的状态判断
========================================================================*/
static
uint16 PhysicInitState_FltCode(SysInitState_T*ptr)
{
  uint16 fltcode=0;
  
  fltcode = (ptr->ADC)|(ptr->IIC<<1)|(ptr->PIT0<<2)|(ptr->PLL<<3)|(ptr->Relay_Positvie<<4)|\
            (ptr->EEPROM<<5)|(ptr->CAN1<<6)|(ptr->CAN2<<7)|(ptr->Screen<<8)|(ptr->SPI<<9)|\
            (ptr->Insul<<10);  
  return fltcode;
}
/*=======================================================================
 *函数名:      Level2_FltCode(Flt_BMSCheckSelf_T*) 
 *功能:        初始化中2级断继电器故障的状态判断
 *参数:        ptr:指向2级故障的状态变量      
 *返回：       uint16:返回2级故障的状态,0:代表无故障;其他:故障代码
 *说明：       初始化中2级断继电器故障的状态判断
========================================================================*/
static
uint16 Level2_FltCode(Flt_BMSCheckSelf_T*ptr)
{
   uint16 fltcode=0;
   
   fltcode = (ptr->SysVolt_Over)|(ptr->SysVolt_Low<<1)|(ptr->CellVolt_Low<<2)|(ptr->CellVolt_Over<<3)|\
             (ptr->CellTemp_Low<<4)|(ptr->CellTemp_Over<<5)|(ptr->SysCurr_Over<<6)|\
             (ptr->SysInsul_Flt<<7)|(ptr->OpenWire_Flt<<8);
   
   return fltcode;
}

/*=======================================================================
 *函数名:      BMSCheckself_UpMonitor(Flt_BMSCheckSelf_T*, SysInitState_T*) 
 *功能:        当自检一直不通过时，发送故障代码到上位机上
 *参数:        lev2:指向2级故障的状态变量 
               sysint:指向物理层初始化状态变量
 *返回：       无
 *说明：       当自检一直不通过时，发送故障代码到上位机上
========================================================================*/
void BMSCheckself_UpMonitor(SysInitState_T*sysint, Flt_BMSCheckSelf_T*lev2)
{
  CANFRAME BMS_to_Upmonitor;
  uint8 CANstate;
  uint16 sysfltcode=0,lev2fltcode=0;
  
  BMS_to_Upmonitor.m_IDE = 1;
	BMS_to_Upmonitor.m_RTR = 0;
	BMS_to_Upmonitor.m_dataLen = 6;
	BMS_to_Upmonitor.m_priority = 6;
	//0x1820C0F4 
  BMS_to_Upmonitor.m_ID = 0x1820C0F4;
	
	sysfltcode  = PhysicInitState_FltCode(sysint);
	lev2fltcode = Level2_FltCode(lev2);    
  
  BMS_to_Upmonitor.m_data[0] = 0x02;    //编号 0x02
  BMS_to_Upmonitor.m_data[1] = 0x00;    //电池自检状态  00 自检中，01 成功，02 失败

  BMS_to_Upmonitor.m_data[2] = (uint8)(sysfltcode&0xFF);     
  BMS_to_Upmonitor.m_data[3] = (uint8)(sysfltcode>>8);     
  
  BMS_to_Upmonitor.m_data[4] = (uint8)(lev2fltcode&0xFF);     
  BMS_to_Upmonitor.m_data[5] = (uint8)(lev2fltcode>>8);
  //BMS_to_Upmonitor.m_data[6] = 0xFF;//预留
  //BMS_to_Upmonitor.m_data[7] = 0xFF;//预留
   
  CANstate = CAN_ToUpMonitor(&BMS_to_Upmonitor);
}


