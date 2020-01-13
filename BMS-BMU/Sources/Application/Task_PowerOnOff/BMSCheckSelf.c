/*=======================================================================
 *Subsystem:   裸机
 *File:        BMSCheckSelf.C
 *Author:      WenYuhao
 *Description: 通信：
               接口：
               波特率：
               只有自检成功后才能闭合响应的开关
 * ========================================================================
 * History:    修改历史记录列表；
 * 1. Date:    
      Author:  
      Modification: 
========================================================================*/
#include  "includes.h"

Flt_BMSCheckSelf_T  g_Flt_BMSCheckSelf;
/*=======================================================================
 *函数名:      放电自检判断项 
 *功能:        初始化后进行自检,自检只要一次通过则闭合继电器
 *参数:        参照各个函数       
 *返回：       参照各个函数
 *说明：       
========================================================================*/
/************************放电系统总压过低********************************/
static
uint32 CheckSelf_SysVoltLow_DischagTemp(uint8 Temp)
{
  if(Temp>=NTEMP_BOUNDARY)//常温(>=0℃)
  { 
    return (g_BMSMonitor_Volt.Volt_Sys_Low2);
  }
  else //低温
  {
    return (g_BMSMonitor_New_LT.Voll_Sys_Low2_LT);
  }
}
static
uint8  CheckSelf_SysVoltLow_DisCharge(uint32 totalvolt, uint8 temp)
{
   static uint8 cnt=0;
   if(totalvolt/1000.0 > CheckSelf_SysVoltLow_DischagTemp(temp))
   {
      cnt = 0;
      return 0;  //自检成功
      
   }
   else
   {
      if(cnt >= 2)
      {
        cnt = 2;
      }
      if(++cnt>=2)
      {
        return 1; //自检故障
      }
      
   }
   return 2;
}

/************************放电单体电压过低********************************/
static
uint16 CheckSelf_CellVoltLow_Temp(uint8 Temp)
{
  if(Temp>=NTEMP_BOUNDARY)//常温(>=0℃)
  { 
    return (g_BMSMonitor_Volt.Volt_Cell_Low2);//可换成标定变量(分辨率：0.001V/位)
  }
  else //低温
  {
    return (g_BMSMonitor_New_LT.Volt_Cell_Low2_LT);//可换成标定变量(分辨率：0.001V/位)
  }
}

static
uint8  CheckSelf_CellVoltLow_DisCharge(uint16 Voltmin, uint8 temp)
{
   static uint8 cnt=0;
   if(Voltmin > CheckSelf_CellVoltLow_Temp(temp))
   {
      cnt = 0;
      return 0;  //自检成功
   }
   else
   {
      if(cnt >= 2)
      {
        cnt = 2;
      }
      if(++cnt>=2)
      {
        return 1; //自检故障
      }
      
   }
   return 2;
}

/************************放电单体温度过高********************************/
static
uint8  CheckSelf_CellTempHigh_DisCharge(uint8 Temp)
{
   static uint8 cnt=0;
   if(Temp < g_BMSMonitor_Temp.Temp_DisCharge_High2)
   {
      cnt = 0;
      return 0;  //自检成功
   }
   else
   {
      if(cnt >= 2)
      {
        cnt = 2;
      }
      if(++cnt>=2)
      {
        return 1; //自检故障
      }
      
   }
   return 2;
}

/************************放电单体温度过低********************************/
static
uint8  CheckSelf_CellTempLow_DisCharge(uint8 Temp)
{
   static uint8 cnt=0;
   if(Temp > g_BMSMonitor_Temp.Temp_DisCharge_Low2)
   {
      cnt = 0;
      return 0;  //自检成功
   }
   else
   {
      if(cnt >= 2)
      {
        cnt = 2;
      }
      if(++cnt>=2)
      {
        return 1; //自检故障
      }
      
   }
   return 2;
}

/************************放电电流过过高********************************/
static
uint8  CheckSelf_CurrentOver_DisCharge(float curr)
{
   static uint8 cnt=0;
   if(curr < (g_BMSMonitor_Curr.Current_DisCharge_High2*0.1-750))
   {
      cnt = 0;
      return 0;  //自检成功
   }
   else
   {
      if(cnt >= 2)
      {
        cnt = 2;
      }
      if(++cnt>=2)
      {
        return 1;//自检故障
      }
      
   }
   return 2;
}

/************************绝缘故障********************************/
static
uint8  CheckSelf_InsulDetect(uint8 insul)
{
   static uint8 cnt=0;
   if(insul > (g_BMSMonitor_Insul.Insulation_Resis2*0.1)) //0.1kΩ/V
   {
      cnt = 0;
      return 0;  //自检成功
   }
   else
   {
      if(cnt >= 2)
      {
        cnt = 2;
      }
      if(++cnt>=2)
      {
        return 1;//自检故障
      }
      
   }
   return 2;
}

/*=======================================================================
 *函数名:      充电自检判断项 
 *功能:        初始化后进行自检,自检只要一次通过则闭合继电器
 *参数:        参照各个函数       
 *返回：       参照各个函数
 *说明：       
========================================================================*/
/************************充电系统总压过高********************************/
static
uint8  CheckSelf_SysVoltHigh_Charge(uint32 totalvolt)
{
   static uint8 cnt=0;

   if(totalvolt/1000.0 < g_BMSMonitor_Volt.Volt_Sys_High2)//移植时注意分辨率和uint16是否合适
   {
      cnt = 0;
      return 0;  //自检成功
      
   }
   else
   {
      if(cnt >= 2)
      {
        cnt = 2;
      }
      if(++cnt>=2)
      {
        return 1;//自检故障
      }
      
   }
   return 2;
}
/************************充电单体电压过高********************************/
static
uint8  CheckSelf_CellVoltHigh_Charge(uint16 Voltmax)
{
   static uint8 cnt=0;
   if(Voltmax< g_BMSMonitor_Volt.Volt_Cell_High2)     //问题同上
   {
      cnt = 0;
      return 0;  //自检成功
   }
   else
   {
      if(cnt >= 2)
      {
        cnt = 2;
      }
      if(++cnt>=2)
      {
        return 1;//自检故障
      }
      
   }
   return 2;
}
/********************************************************************/
static
uint8  CheckSelf_CellTempHigh_Charge(uint8 Temp)
{
   static uint8 cnt=0;
   if(Temp < g_BMSMonitor_Temp.Temp_Charge_High2)  //注意偏移量
   {
      cnt = 0;
      return 0;  //自检成功
   }
   else
   {
      if(cnt >= 2)
      {
        cnt = 2;
      }
      if(++cnt>=2)
      {
        return 1;//自检故障
      }
   }
   return 2;
}
/********************************************************************/
static
uint8  CheckSelf_CellTempLow_Charge(uint8 Temp)
{
   static uint8 cnt=0;
   if(Temp > g_BMSMonitor_Temp.Temp_Charge_Low2) //注意偏移量
   {
      cnt = 0;
      return 0;  //自检成功
   }
   else
   {
      if(cnt >= 2)
      {
        cnt = 2;
      }
      if(++cnt>=2)
      {
        return 1;
        //自检故障
      }
      
   }
   return 2;
} 

/*=======================================================================
 *函数名:      CheckSelf_OpenWireDetect 
 *功能:        绝缘检测的状态判断
 *参数:        state:绝缘检测输入状态       
 *返回：       uint8:是否存在绝缘故障,0:正常;1:故障
 *说明：       初始化作为自检使用
========================================================================*/
static
uint8  CheckSelf_OpenWireDetect(uint8 state)
{
  if(state == 0)
  {
     return 0;  //自检成功
  }
  return 1;
}

/*=======================================================================
 *函数名:      CheckSelf_Discharge() 
 *功能:        放电时自检的状态判断
 *参数:        无      
 *返回：       uint8:是否存在自检故障,0:正常;1:故障
 *说明：       初始化作为自检使用
========================================================================*/
static
uint8 CheckSelf_Discharge(Flt_BMSCheckSelf_T*ptr)
{
   uint8 state=0;
   memset(ptr, 0x00, sizeof(Flt_BMSCheckSelf_T));
   
   ptr->SysVolt_Low    = CheckSelf_SysVoltLow_DisCharge(g_VoltInfo.SysVolt_Total, g_TempInfo.CellTemp_Ave);
   state = state|ptr->SysVolt_Low;
   
   ptr->CellVolt_Low   = CheckSelf_CellVoltLow_DisCharge(g_VoltInfo.CellVolt_Min, g_TempInfo.CellTemp_Ave);
   state = state|ptr->CellVolt_Low;
   
   ptr->CellTemp_Over  = CheckSelf_CellTempHigh_DisCharge(g_TempInfo.CellTemp_Max);
   state = state|ptr->CellTemp_Over;
   
   ptr->CellTemp_Low   = CheckSelf_CellTempLow_DisCharge(g_TempInfo.CellTemp_Min);
   state = state|ptr->CellTemp_Low;
   
   ptr->SysCurr_Over   = CheckSelf_CurrentOver_DisCharge(g_DataColletInfo.DataCollet_Current_Filter);
   state = state|ptr->SysCurr_Over;
   
   ptr->SysInsul_Flt   = CheckSelf_InsulDetect(g_IsoDetect.insulation_resist);
   state = state|ptr->SysInsul_Flt;
   
   ptr->OpenWire_Flt  = CheckSelf_OpenWireDetect(g_OpenWireInfo.OpenWire_Status);
   state = state|ptr->OpenWire_Flt;
   return state;
}

/*=======================================================================
 *函数名:      CheckSelf_Charge() 
 *功能:        充电时自检的状态判断
 *参数:        无      
 *返回：       uint8:是否存在自检故障,0:正常;1:故障
 *说明：       初始化作为自检使用
========================================================================*/
static
uint8 CheckSelf_Charge(Flt_BMSCheckSelf_T*ptr)
{
   uint8 state=0;
   memset(ptr, 0x00, sizeof(Flt_BMSCheckSelf_T));
   
   ptr->SysVolt_Over   = CheckSelf_SysVoltHigh_Charge(g_VoltInfo.SysVolt_Total);
   state = state|ptr->SysVolt_Over;
   
   ptr->CellVolt_Over  = CheckSelf_CellVoltHigh_Charge(g_VoltInfo.CellVolt_Max);
   state = state|ptr->CellVolt_Over;
   
   ptr->CellTemp_Over  = CheckSelf_CellTempHigh_Charge(g_TempInfo.CellTemp_Max);
   state = state|ptr->CellTemp_Over;
   
   ptr->CellTemp_Low   = CheckSelf_CellTempLow_Charge(g_TempInfo.CellTemp_Min);
   state = state|ptr->CellTemp_Low;
   
   ptr->SysInsul_Flt   = CheckSelf_InsulDetect(g_IsoDetect.insulation_resist);
   state = state|ptr->SysInsul_Flt;
   
   ptr->OpenWire_Flt  = CheckSelf_OpenWireDetect(g_OpenWireInfo.OpenWire_Status);
   state = state|ptr->OpenWire_Flt;
   return state;
}

/*=======================================================================
 *函数名:      CheckSelf_DelayTime() 
 *功能:        自检函数
 *参数:        time:激励时钟
               mode:工作模式
 *返回：       无
 *说明：       BMS自检过程中，只要BMS工作状态改变那么保证自检只进行1次
========================================================================*/
 static
 void CheckSelf_DelayTime(uint16 ts)
 {
    uint16 i,j;
    for(i=0; i<100; i++)
    {
      for(j=0; j<ts; j++);
    }
 }
 /*=======================================================================
 *函数名:      Checkself_BattState() 
 *功能:        BMS自检前的数据采集及处理
 *参数:        不同的总线频率需要设置不同的延时      
 *返回：       无
 *说明：       任务时间到标志位给1，10ms执行一次
========================================================================*/

static
void Checkself_BattState()
{ 
  Task_OpenWireDetect();          //导线开路
  CheckSelf_DelayTime(5000);

  Task_VoltCMDSend();             //电压寄存器清零
  CheckSelf_DelayTime(5000);

  Task_VoltCollect();             //电压读取函数
  CheckSelf_DelayTime(5000);

  Task_TempCMDSend();             //温度清零函数
  CheckSelf_DelayTime(5000);

  Task_TempCollect();             //温度采集函数
  CheckSelf_DelayTime(5000);
  
  Task_InsulationDetect();        //均衡检测
  CheckSelf_DelayTime(1000);

  Task_DataProcess();             //数据处理
}
/*=======================================================================
 *函数名:      BMS_CheckSelf() 
 *功能:        自检总函数
 *参数:        time:激励时间
               workmode：工作状态
               sysinitstate:物理层初始化状态      
 *返回：       uint8:是否存在自检故障,0:正常;1:故障
 *说明：       初始化中,若自检成功则执行任务,否则一直自检
========================================================================*/
static
uint8 CheckSelf_Process(uint8 workmode, uint8 sysinitstate)
{
   Checkself_BattState();//自检进行数据采集    
   switch(workmode)
   {
     case MODE_DISCHARGE :
       if((CheckSelf_Discharge(&g_Flt_BMSCheckSelf) == 0) && (sysinitstate == 0)) //故障判断与初始化状态0:正常
       {
          return 0;
       }
     break; 
     
     case MODE_CHARGE:
       if((CheckSelf_Charge(&g_Flt_BMSCheckSelf) == 0) && (sysinitstate == 0)) //故障判断与初始化状态0:正常
       {
          return 0;
       }
     break; 
     
     default://可能包含慢充自检
     break;                                                                
   }
   return 1;
}

/*=======================================================================
 *函数名:      Task_BMSWorkModeCheckself() 
 *功能:        自检函数
 *参数:        time:激励时钟
               mode:工作模式
 *返回：       无
 *说明：       BMS自检过程中，只要BMS工作状态改变那么保证自检只进行1次
========================================================================*/
void BMS_WorkModeCheckself(void)
{
   static uint8 workmode = 1;
   
   if(workmode != g_WorkStateJudge.WorkState)//此处确保自检只在状态转换后进行
   {
      workmode = g_WorkStateJudge.WorkState; 
      memset(&g_Roll_Tick, 0x00, sizeof(Roll_Tick_T)); 
      Light_Control(LED2_PORT, LED2_pin, Light_OFF);//关闭自检成功显示灯
      DisablePIT(0);//关PIT0时间中断(只进行自检)
      
      while((CheckSelf_Process(workmode, g_SysInitState.AllInit_State)!=0)\
             &&(Boot_Data.OnlineUpdateCheck == 0))//自检轮询直到成功跳出While,也可以直接进行升级
      {  
         BMSCheckself_UpMonitor(&g_SysInitState, &g_Flt_BMSCheckSelf);
         CheckSelf_DelayTime(100);
         if(workmode != WokeModeJudgment())        //状态转换后重新自检
         {
            workmode = !workmode;
            continue;
         }     
      }
      //while之后代表自检成功,将重新计时,并消除自检故障
      Light_Control(LED2_PORT, LED2_pin, Light_ON);
      memset(&PIT_TimePeriod, 0x00, sizeof(PIT_TimePeriod_T));
      memset(&g_Flt_BMSCheckSelf, 0x00, sizeof(Flt_BMSCheckSelf_T));
      EnablePIT(0);//自检成功之后打开中断PIT0
   }
   g_WorkStateJudge.WorkState = WokeModeJudgment();
}

