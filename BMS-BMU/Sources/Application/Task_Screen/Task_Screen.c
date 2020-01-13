/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_Screen.C
 *Author:      WenMing
 *Description: 通信：SCI2.
               接口：PJ0（RXD）；PJ1（TXD）
               波特率：
               调试：观察RS485_Receive数据与显示屏数据是否一致,以及IIC模块时间的显示；
 ========================================================================
 * History:           修改历史记录列表，每条修改记录应包括修改日期、修改者及修改内容简述
 * 1. Date:           
      Author:         
      Modification:   
========================================================================*/
#include  "includes.h"
RS485  RS485_Receive; 

static uint8 RS485_Init(void);        
/*=======================================================================
 *函数名:      Init_Screen(void)
 *功能:        初始化显示屏接口
 *参数:        无       
 *返回：       无
 *说明：       
========================================================================*/
uint8 Init_Screen(void)
{
   uint8 state;
   
   state = RS485_Init();
   state = state|SCI1_Init();
   return state;
}


/*=======================================================================
 *函数名:      Init_Screen(void)
 *功能:        初始化显示屏接口
 *参数:        无       
 *返回：       无
 *说明：       
========================================================================*/
static
void SCI_ScreenTransfer(uint8 numbyte, uint8 *data)
{
   SCI1_Send_NByte(numbyte, data);
}
/*=======================================================================
 *函数名:      RS485_DataMemset(void)
 *功能:        初始化485数据
 *参数:        无       
 *返回：       无
 *说明：       
========================================================================*/
static
uint8 RS485_Init(void) 
{
  RS485_EnableDir = 1;
  RS485_Enable = 1; 
  
  memset(&RS485_Receive,0x00, sizeof(RS485));//清除485的数据
  return 0;
}

/*=======================================================================
 *函数名:      RS485_DataReceive
 *功能:        SCI数据接收
 *参数:        无       
 *返回：       无
 *说明：       
========================================================================*/
static
void RS485_DataReceice(void) 
{ 
 /*----------------------------------------双字节接收-----------------------------------*/
  RS485_Receive.TxData_couple.TX2.BMS_SOH             = (uint16)((g_BMSMonitor_SOH.SOH+0.005)*100); 
  RS485_Receive.TxData_couple.TX2.RunningTime         = (uint16)(g_SysTime.BMS_TotalRun_MiniteTime/60);           
  RS485_Receive.TxData_couple.TX2.BMS_Current         = (uint16)((g_DataColletInfo.DataCollet_Current_Filter + 750)*10);                  
  RS485_Receive.TxData_couple.TX2.BMS_SOC             = (uint16)((g_SOCInfo.SOC_ValueRead+0.005)*100);                                                    
  RS485_Receive.TxData_couple.TX2.Pack_Hightemp       = (uint16)g_TempInfo.CellTemp_Max;               
  RS485_Receive.TxData_couple.TX2.Pack_Lowtemp        = (uint16)g_TempInfo.CellTemp_Min;                
  RS485_Receive.TxData_couple.TX2.Pack_Volt           = (uint16)(g_VoltInfo.SysVolt_Total/1000.0);                 
  RS485_Receive.TxData_couple.TX2.Single_Maxvolt      = (g_VoltInfo.CellVolt_Max +5)/10;      
  RS485_Receive.TxData_couple.TX2.Single_Lowvolt      = (g_VoltInfo.CellVolt_Min +5)/10;        
  RS485_Receive.TxData_couple.TX2.iso_resistance      = g_IsoDetect.insulation_resist;    //绝缘电阻
                       
 /*------------------------------------------单字节接收---------------------------------*/       
  RS485_Receive.TxData_single.TX1.Alam_SOC            = 0x00;
  RS485_Receive.TxData_single.TX1.Alam_VoltSL         = (uint8)g_Flt_DisChg.Level_Volt_Sys_Low;
  RS485_Receive.TxData_single.TX1.Alam_VoltCL         = (uint8)g_Flt_DisChg.Level_Volt_Cell_Low;
  RS485_Receive.TxData_single.TX1.Alam_TempH_DisChg   = (uint8)g_Flt_DisChg.Level_Temp_High;
  RS485_Receive.TxData_single.TX1.Alam_TempL_DisChg   = (uint8)g_Flt_DisChg.Level_Temp_Low;
  RS485_Receive.TxData_single.TX1.Alam_CurrH_DisChg   = (uint8)g_Flt_DisChg.Level_Current_DisCharge_High;
  RS485_Receive.TxData_single.TX1.Alam_VoltCD_DisChg  = (uint8)g_Flt_DisChg.Level_Volt_Cell_Diff_High;
  RS485_Receive.TxData_single.TX1.Alam_TempD_DisChg   = (uint8)g_Flt_DisChg.Level_Temp_Diff_High;
  
  RS485_Receive.TxData_single.TX1.Alam_VoltSH         = (uint8)g_Flt_Charge.Level_Volt_Sys_High;
  RS485_Receive.TxData_single.TX1.Alam_VoltCH         = (uint8)g_Flt_Charge.Level_Volt_Cell_High;
  RS485_Receive.TxData_single.TX1.Alam_TempH_Charge   = (uint8)g_Flt_Charge.Level_Temp_High;
  RS485_Receive.TxData_single.TX1.Alam_TempL_Charge   = (uint8)g_Flt_Charge.Level_Temp_Low;
  RS485_Receive.TxData_single.TX1.Alam_CurrH_Charge   = (uint8)g_Flt_Charge.Level_Current_Charge_High;
  RS485_Receive.TxData_single.TX1.Alam_VoltCD_Charge  = (uint8)g_Flt_Charge.Level_Volt_Cell_Diff_High;
  RS485_Receive.TxData_single.TX1.Alam_TempD_Charge   = (uint8)g_Flt_Charge.Level_Temp_Diff_High;
  
  RS485_Receive.TxData_single.TX1.Alam_Insul          = (uint8)(g_Flt_DisChg.Level_Insul|g_Flt_Charge.Level_Insul);
  RS485_Receive.TxData_single.TX1.Alam_Checkself      = 0x01;
  
 }   
/*=======================================================================
 *函数名:      Screen_delay(uint8)
 *功能:        显示屏的延时函数
 *参数:        rs:要发送结构体的字节指针       
 *返回：       无
 *说明：       
========================================================================*/
static
void Screen_delay(uint16 ticks)
{
  uint16 i;
  for(i=0; i<ticks; i++);
}
/*=======================================================================
 *函数名:      Task_ScreenTransfer
 *功能:        SACI发送处理
 *参数:        rs:要发送结构体的字节指针       
 *返回：       无
 *说明：       
========================================================================*/
void Task_ScreenTransfer(void) 
{          
   RS485_DataReceice();
   Screen_delay(10);
   SCI_ScreenTransfer(Array_couple, RS485_Receive.TxData_couple.SCI_Content2);  
   SCI_ScreenTransfer(Array_single, RS485_Receive.TxData_single.SCI_Content1); 
   g_Roll_Tick.Roll_Screen++;
}