/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_Charge.C
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

BMSCharge_T        g_BMSCharge;
BMSCharge_State_T  BMSCharge_State;

/*=======================================================================
 *函数名:      CAN_ToChargeInit(void) 
 *功能:        与充电桩通信的CAN通道初始化
 *参数:        无       
 *返回：       无
 
 *说明：       
========================================================================*/ 
uint8 CAN_ToChargeInit(void)
{
  uint8 state;
  
  memset(&g_BMSCharge,      0, sizeof(BMSCharge_T));
  memset(&BMSCharge_State,  0, sizeof(BMSCharge_State_T));
  memset(&ChargePileBMS,    0, sizeof(ChargePileBMS_T));
  memset(&g_Charge_State,   0, sizeof(Charge_State_T));
 
  state = CAN1_Init(125);
  return(state);
}

/*=======================================================================
 *函数名:      CAN_BMSToCharge(pCANFRAME canptr) 
 *功能:        BMS information to UpMonitor
 *参数:        无       
 *返回：       无
 *说明：       BMS发送信息给上位机的CAN通道选择；
========================================================================*/ 
static
uint8 CAN_BMSToCharge(pCANFRAME canptr)
{
  if(!CAN1_SendMsg(canptr))
  {
     return 0;  //正常
  }  
  return 1;   //报错
}

/*=======================================================================
 *函数名:      Charge_Strategy(void) 
 *功能:        充电控制策略
 *参数:        无       
 *返回：       无
 *说明：       控制充电的电流大小，及收集充电状态，控制充电开关
========================================================================*/ 
static
void Charge_Strategy(void) 
{
  CANFRAME BMS_to_ChargePile;
  static uint8 time=0;
  Charge_VoltCurrRequest();
  
  BMS_to_ChargePile.m_IDE = 0;//标准帧
	BMS_to_ChargePile.m_RTR = 0;
	BMS_to_ChargePile.m_dataLen = 8;
	BMS_to_ChargePile.m_priority = 6;  
	switch(time)
	{                                                                                   
	  case 0:
      //0x111
      BMS_to_ChargePile.m_ID = 0x111;
     
      BMS_to_ChargePile.m_data[0] = (g_BMSCharge.Volt_Max_ChargePile*10) >> 8;    //最高允许充电端电压高字节   0.1V/bit
    	BMS_to_ChargePile.m_data[1] = (uint8)(g_BMSCharge.Volt_Max_ChargePile*10);                              
    	BMS_to_ChargePile.m_data[2] = (g_BMSCharge.Curr_Max_ChargePile*10)>> 8;     //最高允许充充电电流高字节   已经过分辨率(0.1A/bit)处理   
    	BMS_to_ChargePile.m_data[3] = (uint8)(g_BMSCharge.Curr_Max_ChargePile*10); 
    	BMS_to_ChargePile.m_data[4] = g_BMSCharge.Control_ChargePile;               //控制充电桩    0:开启    1:关闭
    	BMS_to_ChargePile.m_data[5] = (((0x00)&0x01) + \
    	                               ((BMSCharge_State.TempH_Cell<<1)&0x02) + \
    	                               ((BMSCharge_State.TempL_Cell<<2)&0x04) + \
    	                               ((BMSCharge_State.CurrH_Cell<<3)&0x08) + \
    	                               ((BMSCharge_State.Insul<<4)&0x10) + \
    	                               ((BMSCharge_State.BMSGetMsg<<5)&0x20) + \
    	                               ((BMSCharge_State.FaultFlag<<6)&0x40) + \
    	                               ((0x00)&0x80));
    	BMS_to_ChargePile.m_data[6] = 0xFF;                 
    	BMS_to_ChargePile.m_data[7] = 0xFF; 
      while(CAN_BMSToCharge(&BMS_to_ChargePile));
      time = 1;
    break;
   
	  case 1:
      //0x115
      BMS_to_ChargePile.m_ID = 0x115;
     
      BMS_to_ChargePile.m_data[0] = ((uint16)(g_BMSCharge.VoltC_Max*0.1)) >> 8;      //单体最高电压高字节   
    	BMS_to_ChargePile.m_data[1] = (uint8)(g_BMSCharge.VoltC_Max*0.1);                              
    	BMS_to_ChargePile.m_data[2] = ((uint16)(g_BMSCharge.VoltC_Min*0.1)) >> 8;       //单体最低电压高字节   
    	BMS_to_ChargePile.m_data[3] = (uint8)(g_BMSCharge.VoltC_Min*0.1); 
    	BMS_to_ChargePile.m_data[4] = (uint8)((g_BMSCharge.SOC + 0.005)*250);                        //SOC   已经过分辨率(0.4%/bit)处理
    	BMS_to_ChargePile.m_data[5] = g_BMSCharge.Temp_Max;                   //最高温度     1℃/bit  偏移量:-40℃
    	BMS_to_ChargePile.m_data[6] = ((uint16)(g_BMSCharge.VoltS* 0.001)) >>8;                 //电池组电压高字节     已经过分辨率处理
    	BMS_to_ChargePile.m_data[7] = (uint8)(g_BMSCharge.VoltS* 0.001);       
      while(CAN_BMSToCharge(&BMS_to_ChargePile));
      time = 0;
    break;
	}
}
/*=======================================================================
 *函数名:      Task_Charge(void) 
 *功能:        充电任务
 *参数:        无       
 *返回：       无
 *说明：       由于充电末期是变电流请求充电,因此在状态转换后需要清除充电请求变量
========================================================================*/
void Task_Charge(void)
{
  static uint8 flg;
  switch(g_WorkStateJudge.WorkState)
  {
    case MODE_CHARGE:
      Charge_Strategy();
      flg = 0;
      g_Roll_Tick.Roll_Charge++;
    break;
    
    case MODE_DISCHARGE: //清除充电结构体变量
      if(flg == 0) //保证只清除1次
      {
        flg = 1;
        memset(&g_BMSCharge, 0x00, sizeof(BMSCharge_T));
        memset(&BMSCharge_State, 0x00, sizeof(BMSCharge_State_T));
        memset(&ChargePileBMS, 0x00, sizeof(ChargePileBMS_T));
        memset(&g_Charge_State, 0x00, sizeof(Charge_State_T));
      }
    break;
    
    default:
    break;
  }  
}






     
    
