/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_PowerOnOff.c
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:  
========================================================================*/
#include  "includes.h"
/*=======================================================================
 *函数名:      PositiveRelay_Init
               CSSUPowerRelay_Init
               ScreenPowerRelay_Init
 *功能:        分别对主正继电器、CSSU电源开关、显示屏开关的初始化
 *参数:        switchstate:开关状态(Relay_OFF:开关关,Relay_ON:开关开)
               
 *返回：       无
 *说明：       分别对主正继电器、CSSU电源开关、显示屏开关的初始化
========================================================================*/
uint8 Init_Relay(void)//主正继电器
{
  uint8 state;
  
  //工作状态标记位清零
  memset(&g_WorkStateJudge,     0, sizeof(WorkStateJudge_T));    //任务工作状态清零
  
  //自检数据清零
  memset(&g_Flt_BMSCheckSelf,   0, sizeof(Flt_BMSCheckSelf_T));     
    
  state = Port_Init(Relay_Positive_PORT, Relay_Positive_pin, Relay_OFF);
  state = state | Port_Init(Relay_CSSUPower_PORT, Relay_CSSUPower_pin, Relay_ON);
  state = state | Port_Init(Relay_ScreenPower_PORT, Relay_ScreenPower_pin, Relay_ON);
  state = state | Light_Init(LED1_PORT, LED1_pin, Light_OFF);//作均衡警示灯
  state = state | Light_Init(LED2_PORT, LED2_pin, Light_OFF);//自检成功灯
  return state;
}
/*=======================================================================
 *函数名:      PositiveRelay_Control
               CSSUPowerRelay_Control
               ScreenPowerRelay_ON
 *功能:        分别对主正继电器、CSSU电源开关、显示屏开关的控制
 *参数:        switchstate:开关状态(Relay_OFF:开关关,Relay_ON:开关开)
               
 *返回：       无
 *说明：       分别对主正继电器、CSSU电源开关、显示屏开关的控制
========================================================================*/
void PositiveRelay_Control(uint8 switchstate)//主正继电器
{
  switch(switchstate)
  {
    case Relay_ON:
      if(Port_StateGet(Relay_Positive_PORT, Relay_Positive_pin) == Relay_OFF)
      {
         Port_Control(Relay_Positive_PORT, Relay_Positive_pin, Relay_ON);
      }
      break;
    
    case Relay_OFF:
      if(Port_StateGet(Relay_Positive_PORT, Relay_Positive_pin) == Relay_ON)
      {
         Port_Control(Relay_Positive_PORT, Relay_Positive_pin, Relay_OFF);
      }
      break;
    
    default:
      break; 
  }

}
//CSSU电源开关 
static
void CSSUPowerRelay_Control(uint8 switchstate)
{
  switch(switchstate)
  {
    case Relay_ON:
      if(Port_StateGet(Relay_CSSUPower_PORT, Relay_CSSUPower_pin) == Relay_OFF)
      {
        Port_Control(Relay_CSSUPower_PORT, Relay_CSSUPower_pin, Relay_ON); 
      }
      break;   
    
    case Relay_OFF:
      if(Port_StateGet(Relay_CSSUPower_PORT, Relay_CSSUPower_pin) == Relay_ON)
      {
        Port_Control(Relay_CSSUPower_PORT, Relay_CSSUPower_pin, Relay_OFF); 
      }
      break;
      
    default:
      break; 
  }
}
//显示屏电源开关 
static
void ScreenPowerRelay_Control(uint8 switchstate)
{
  switch(switchstate)
  {
    case Relay_ON:
      if(Port_StateGet(Relay_ScreenPower_PORT, Relay_ScreenPower_pin) == Relay_OFF)
      {
        Port_Control(Relay_ScreenPower_PORT, Relay_ScreenPower_pin, Relay_ON); 
      }
      break;   
    
    case Relay_OFF:
      if(Port_StateGet(Relay_ScreenPower_PORT, Relay_ScreenPower_pin) == Relay_ON)
      {
        Port_Control(Relay_ScreenPower_PORT, Relay_ScreenPower_pin, Relay_OFF); 
      }
      break;
      
    default:
      break; 
  }
}

/*=======================================================================
 *函数名:      PowerOnOff_Control
 *功能:        上下电的控制
 *参数:        FltLevel:故障等级为2时进行判断
               FltSeconds:发生2级故障延时的时间,单位:s
               StaticCurrSet:即将进入休眠的静态电流大小设置
               StaticTimeSet:即将进入休眠的静态时间大小设置
 *返回：       无
 *说明：       根据条件控制主正、CSSU、显示屏开关的状态
========================================================================*/
static
void PowerOnOff_Control(uint8 FltLevel, uint8 FltSeconds, uint8 StaticCurrSet, uint8 StaticTimeSet)
{
   static uint16 cnt;
   static uint8  relaystate;
   BMS_WorkModeCheckself();            //自检为死循环,自检成功之后才能进行上下电
   
   if(FltLevel == 1)//到达二级故障
   {  
      if((++cnt)*PEWERONOFF_PERIO/1000.0>FltSeconds) //延时Times秒之后断开继电器
      {
        cnt = 0;
        PositiveRelay_Control(Relay_OFF); 
      }
   }
   else//非2级故障,只要满足电流小于2A就任务其即将进入休眠状态
   {  
      cnt = 0;
      //当连续静态(电流小于2A时)进入继电器关闭的状态
      if(Sleep_StaticTime(Read_IIC_Time.IIC_Read_Hour, Read_IIC_Time.IIC_Read_Minute,\
         g_DataColletInfo.DataCollet_Current_Filter, StaticCurrSet, StaticTimeSet))
      {
         PositiveRelay_Control(Relay_OFF);
         CSSUPowerRelay_Control(Relay_OFF);
         ScreenPowerRelay_Control(Relay_OFF);
      }
      else
      {
         if(relaystate == 0)
         {
            relaystate = 1;
            PositiveRelay_Control(Relay_ON); //上电继电器只开启1次
         }
      }
   }    
}
/*=======================================================================
 *函数名:      Task_PowerOnOff
 *功能:        上下电的控制
 *参数:        无
 *返回：       无
 *说明：       设定进入2级故障后断开继电器的延时时长为:30s
               设定BMS进入休眠条件的静态电流为:2A
               设定BMS进入休眠的静态时间为:12h                
========================================================================*/
void Task_PowerOnOff(void)
{
   PowerOnOff_Control(g_Flt_DisChg.Level_DisCharge_SwitchOff_flag|g_Flt_Charge.Level_Charge_SwitchOff_flag,\
                      30, 5, 1);

   g_Roll_Tick.Roll_Power++;
}


