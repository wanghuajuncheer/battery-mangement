                                              /*=======================================================================
 *Subsystem:   裸机
 *File:        ChargeToBMS.C
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
                                              
ChargePileBMS_T  ChargePileBMS;
Charge_State_T   g_Charge_State;
/*=======================================================================
 *函数名:      ChargePile_to_Bms(pCANFRAME data)
 *功能:        充电机发送至BMS
 *参数:        无       
 *返回：       无
 *说明：       
========================================================================*/
void CAN_ChargetoBMS(pCANFRAME data)
{
  ChargePileBMS.Volt_ChargePileOut = (((uint16)(data -> m_data[0]))<<8) + (data -> m_data[1]);   
  ChargePileBMS.Curr_ChargePileOut = (((uint16)(data -> m_data[2]))<<8) + (data -> m_data[3]);  

  g_Charge_State.Hard             = (data -> m_data[4])&0x01;       //充电桩硬件故障
  g_Charge_State.TempH_ChargePile = ((data -> m_data[4])>>1)&0x01;  //充电机过温
  g_Charge_State.VoltL_ChargePile = ((data -> m_data[4])>>2)&0x01;  //充电机输入电压过低
  g_Charge_State.On_Line          = ((data -> m_data[4])>>3)&0x01;  //充电机启动状态
  g_Charge_State.GetMsg           = ((data -> m_data[4])>>4)&0x01;  //充电桩接收BMS信息超时

  if((data -> m_data[4])&0x1F)
  {
    g_Charge_State.FltState = 1;
  }
  else
  {
    g_Charge_State.FltState = 0;
  }
   
}         

