/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_FltLevJudg.c
 *Author:      Wenming
 *Description: 
 ========================================================================
 * History:    修改历史记录列表，修改记录包括修改日期、修改者及修改内容
 * 1. Date:
      Author:
      Modification:
========================================================================*/
#include  "includes.h"

/*=======================================================================
                                  声明
========================================================================*/ 
Fault_DisCharge_T  g_Flt_DisChg; 
Fault_Charge_T     g_Flt_Charge;
State_Offline_T    State_Offline;
HeartBeat_T        HeartBeat;

static void FltLevJudg(uint8 workstate);
//放电故障判断
static uint8 Fault_DisChg_VoltSL(uint32 Volt,uint8 Temp); //放电总压低
static uint8 Fault_DisChg_VoltCL(uint16 Volt,uint8 Temp); //放电单体电压低
static uint8 Fault_DisChg_VoltCD(uint16 V_Diff);         //放电压差
static uint8 Fault_DisChg_TempH(uint8 Temp);             //放电高温
static uint8 Fault_DisChg_TempL(uint8 Temp);             //放电低温
static uint8 Fault_DisChg_TempD(uint8 T_Diff);           //放低温差
static uint8 Fault_DisChg_CurrH(float Current);          //放电过流
static uint8 Fault_DisChg_Insul(uint16 Insul);           //绝缘故障
//充电故障判断
static uint8 Fault_Charge_VoltSH(uint32 Volt);            //充电总压高
static uint8 Fault_Charge_VoltCH(uint16 Volt);           //充电单体高
static uint8 Fault_Charge_VoltCD(uint16 V_Diff);         //充电压差
static uint8 Fault_Charge_TempH(uint8 Temp);             //充电高温
static uint8 Fault_Charge_TempL(uint8 Temp);             //充电低温
static uint8 Fault_Charge_TempD(uint8 T_Diff);           //充电温差
static uint8 Fault_Charge_CurrH(float Current);          //充电过流
static uint8 Fault_Charge_Insul(uint16 Insul);           //绝缘故障
static uint8 Fault_Charge_OffLine(void);
//通信掉线
static uint8 Fault_CSSU_OffLine(void);                   //从板掉线
static uint8 Fault_Relay_BreakDown(void);                //继电器粘连故障
 /*=======================================================================
                              故障判断函数0x00
 ========================================================================
 *函数名1:     Init_TaskFltLevJudg(void)
 *功能:        
 *参数:        无    
 *返回：       无
 
 *说明：       
========================================================================*/ 
void Init_TaskFltLevJudg(void)
{
  //故障等级判断变量清零
  memset(&g_Flt_DisChg,           0, sizeof(Fault_DisCharge_T));  //放电故障等级清零
  memset(&g_Flt_Charge,           0, sizeof(Fault_Charge_T));     //充电故障等级清零
  memset(&State_Offline,          0, sizeof(State_Offline_T));    //掉线故障状态清零
  memset(&HeartBeat,              0, sizeof(HeartBeat_T));        //心跳信号清零
}

/*=======================================================================
                              故障判断函数0x00
 ========================================================================
 *函数名1:     Task_FaultLevelJudge(uint8 workstate)
 *功能:        根据系统电压、温度、电流判断故障等级
 *参数:        无    
 *返回：       无
 
 *说明：       
========================================================================*/ 
void Task_FltLevJudg(void)
{
  FltLevJudg(g_WorkStateJudge.WorkState);  
}

/*=======================================================================
                              故障判断函数0x00
 ========================================================================
 *函数名1:     FltLevJudg(uint8 workstate)
 *功能:        根据系统电压、温度、电流判断故障等级
 *参数:        无    
 *返回：       无
 
 *说明：       
========================================================================*/ 
static
void FltLevJudg(uint8 workstate)
{
   State_Offline.CSSU1 = Fault_CSSU_OffLine();//子板掉线
   State_Offline.RelayFlt_Positive = Fault_Relay_BreakDown();//继电器粘连故障
   switch(workstate)
   {
    case MODE_DISCHARGE: //放电状态
      //清除充电状态故障
      g_Flt_Charge.Level_Charge_SwitchOff_flag  = 0; 
      g_Flt_Charge.Level_Charge_BalanceON_Flag  = 0;
      g_Flt_Charge.Level_Volt_Sys_High          = 0;
      g_Flt_Charge.Level_Volt_Cell_High         = 0;
      g_Flt_Charge.Level_Insul                  = 0;
      State_Offline.Charge                      = 0;
      //判断放电状态故障
      g_Flt_DisChg.Level_Volt_Sys_Low           = Fault_DisChg_VoltSL(g_VoltInfo.SysVolt_Total, g_TempInfo.CellTemp_Ave);
      g_Flt_DisChg.Level_Volt_Cell_Low          = Fault_DisChg_VoltCL(g_VoltInfo.CellVolt_Min, g_TempInfo.CellTemp_Ave);
      g_Flt_DisChg.Level_Volt_Cell_Diff_High    = Fault_DisChg_VoltCD(g_VoltInfo.CellVolt_Diff);
      g_Flt_DisChg.Level_Temp_High              = Fault_DisChg_TempH(g_TempInfo.CellTemp_Max);
      g_Flt_DisChg.Level_Temp_Low               = Fault_DisChg_TempL(g_TempInfo.CellTemp_Min);
      g_Flt_DisChg.Level_Temp_Diff_High         = Fault_DisChg_TempD(g_TempInfo.CellTemp_Diff);
      g_Flt_DisChg.Level_Current_DisCharge_High = Fault_DisChg_CurrH(g_DataColletInfo.DataCollet_Current_Filter);
      g_Flt_DisChg.Level_Insul                  = Fault_DisChg_Insul(g_IsoDetect.insulation_resist);
      //断开继电器的二级故障标记
      if((g_Flt_DisChg.Level_Volt_Sys_Low==2) ||\
         (g_Flt_DisChg.Level_Volt_Cell_Low == 2)||\
         (g_Flt_DisChg.Level_Temp_High == 2)||\
         (g_Flt_DisChg.Level_Temp_Low == 2) ||\
         (g_Flt_DisChg.Level_Current_DisCharge_High == 2) ||\
         (g_Flt_DisChg.Level_Insul == 2)||(State_Offline.CSSU1 == 1))
      {
        g_Flt_DisChg.Level_DisCharge_SwitchOff_flag = 1;  
      }
      
    break;
    
    case MODE_CHARGE:   //充电状态
      //清除放电状态故障
      g_Flt_DisChg.Level_Volt_Sys_Low             = 0;
      g_Flt_DisChg.Level_Volt_Cell_Low            = 0;
      g_Flt_DisChg.Level_DisCharge_SwitchOff_flag = 0;
      g_Flt_DisChg.Level_Insul                    = 0;
      //判断充电状态故障
      g_Flt_Charge.Level_Volt_Sys_High        = Fault_Charge_VoltSH(g_VoltInfo.SysVolt_Total);
      g_Flt_Charge.Level_Volt_Cell_High       = Fault_Charge_VoltCH(g_VoltInfo.CellVolt_Max);
      g_Flt_Charge.Level_Volt_Cell_Diff_High  = Fault_Charge_VoltCD(g_VoltInfo.CellVolt_Diff);
      g_Flt_Charge.Level_Temp_High            = Fault_Charge_TempH(g_TempInfo.CellTemp_Max);
      g_Flt_Charge.Level_Temp_Low             = Fault_Charge_TempL(g_TempInfo.CellTemp_Min);
      g_Flt_Charge.Level_Temp_Diff_High       = Fault_Charge_TempD(g_TempInfo.CellTemp_Diff);
      g_Flt_Charge.Level_Current_Charge_High  = Fault_Charge_CurrH(g_DataColletInfo.DataCollet_Current_Filter);
      g_Flt_Charge.Level_Insul                = Fault_Charge_Insul(g_IsoDetect.insulation_resist);
      State_Offline.Charge                    = Fault_Charge_OffLine();
      //断开继电器的二级故障标记
      if((g_Flt_Charge.Level_Volt_Sys_High==2) ||\
         (g_Flt_Charge.Level_Volt_Cell_High==2) ||\
         (g_Flt_Charge.Level_Temp_High == 2)||\
         (g_Flt_Charge.Level_Temp_Low == 2)||\
         (g_Flt_Charge.Level_Insul == 2)||(State_Offline.CSSU1 == 1)||\
         (State_Offline.Charge == 1))
      {
        g_Flt_Charge.Level_Charge_SwitchOff_flag = 1;//2级故障闭合继电器  
      }
      //均衡开启标记
      if((g_Flt_Charge.Level_Volt_Sys_High!=0) ||\
         (g_Flt_Charge.Level_Volt_Cell_High != 0)|\
         (g_Flt_Charge.Level_Temp_High != 0)||\
         (g_Flt_Charge.Level_Temp_Low != 0) ||\
         (g_Flt_Charge.Level_Temp_Diff_High != 0) ||\
         (g_Flt_Charge.Level_Current_Charge_High != 0) ||\
         (g_Flt_Charge.Level_Insul != 0)||(State_Offline.CSSU1 != 0)||\
         (State_Offline.RelayFlt_Positive != 0)||(State_Offline.Charge != 0))
      {
        g_Flt_Charge.Level_Charge_BalanceON_Flag = 0;//只要出现故障则不启动均衡(除压差故障)  
      }
      else
      {
        g_Flt_Charge.Level_Charge_BalanceON_Flag = 1;//开启均衡
      }
            
    break;
   }   
   g_Roll_Tick.Roll_FltJudg++; 
}
/*============================故障判断函数===============================*/

/*=======================================================================
                              放电总压低0x01
 ======================================================================*/
//1级故障函数
static
uint32 Fault1_VoltSys_DisCharge(uint8 Temp)
{
  if(Temp>=NTEMP_BOUNDARY)//常温(>=0℃)
  { 
    return (g_BMSMonitor_Volt.Volt_Sys_Low1);//(28500)*(25)
  }
  else //低温
  {
    return (F1_DISCHG_VOLTSL_LT*0.001);    //低温1级27000.0*25
  }
}
//2级故障函数
static
uint32 Fault2_VoltSys_DisCharge(uint8 Temp)
{
  if(Temp>=NTEMP_BOUNDARY)//常温(>=0℃)
  { 
    return (g_BMSMonitor_Volt.Volt_Sys_Low2);
  }
  else //低温
  {
    return (F2_DISCHG_VOLTSL_LT*0.001);  //低温2级25000.0*25
  }
}
//1恢复至0
static
uint32 Recover1_VoltSys_DisCharge(uint8 Temp)
{
  if(Temp>=NTEMP_BOUNDARY)//常温(>=0℃)
  { 
    return (g_BMSMonitor_Volt.Volt_Sys_Low1 + 2*SYS_SERIES_YiDongLi);
  }
  else //低温
  {
    return (F1_DISCHG_VOLTSL_LT*0.001 + SYS_SERIES_YiDongLi);//可换成标定变量(分辨率：0.001V/位)
  }
}
//故障判断
static uint8 Fault_DisChg_VoltSL(uint32 Volt,uint8 Temp)  //输入系统电压和环境温度
{
  static uint8 cnt[4];      //时间计数
  static uint8 FltL;
  
  if(FltL==0)           //0级故障
  {
    if(Volt/1000.0<=Fault1_VoltSys_DisCharge(Temp))  //0变1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0;
    }
    
    if(Volt/1000.0<=Fault2_VoltSys_DisCharge(Temp))  //0变2
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 2;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[2] = 0; 
    cnt[3] = 0;
  }
  else if(FltL == 1) //1级故障
  {
    if(Volt/1000.0 <= Fault2_VoltSys_DisCharge(Temp))      //1变2
    {
      if(++cnt[2]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
         cnt[2] = 0;
         FltL = 2;
      }
    }
    else
    {
      cnt[2] = 0;
    }
    
    if(Volt/1000.0 >= Recover1_VoltSys_DisCharge(Temp))    //1变0
    {
      if(++cnt[3]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[3] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[3] = 0;
    }
    cnt[0] = 0; 
    cnt[1] = 0;
  }
  else   //2级故障
  {
    cnt[0] = 0; 
    cnt[1] = 0;
    cnt[2] = 0;
    cnt[3] = 0; 
  }
  return(FltL);
}

/*=======================================================================
                              放电单体电压低0x02
 ======================================================================*/

//1级故障函数
static
uint16 Fault1_VoltCell_DisCharge(uint8 Temp)
{
  if(Temp>=NTEMP_BOUNDARY)//常温(>=0℃)
  { 
    return (g_BMSMonitor_Volt.Volt_Cell_Low1);
  }
  else //低温
  {
    return (F1_DISCHG_VOLTCL_LT);
  }
}
//2级故障函数
static
uint16 Fault2_VoltCell_DisCharge(uint8 Temp)
{
  if(Temp>=NTEMP_BOUNDARY)//常温(>=0℃)
  { 
    return (g_BMSMonitor_Volt.Volt_Cell_Low2);
  }
  else //低温
  {
    return (F2_DISCHG_VOLTCL_LT);
  }
}
//恢复至0
static
uint16 Recover1_VoltCell_DisCharge(uint8 Temp)
{
  if(Temp>=NTEMP_BOUNDARY)//常温(>=0℃)
  { 
    return (g_BMSMonitor_Volt.Volt_Cell_Low1 + 2000);
  }
  else //低温
  {
    return (F1_DISCHG_VOLTCL_LT + 1000);
  }
}

//判断故障等级
static uint8 Fault_DisChg_VoltCL(uint16 Volt,uint8 Temp)  //输入单体电压和环境温度
{
  static uint8 cnt[4];      //时间计数
  static uint8 FltL;
  
  if(FltL==0)           //0级故障
  {
    if(Volt<=Fault1_VoltCell_DisCharge(Temp))  //0变1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0;
    }
    
    if(Volt<=Fault2_VoltCell_DisCharge(Temp))  //0变2
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 2;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[2] = 0; 
    cnt[3] = 0;
  }
  else if(FltL == 1) //1级故障
  {
    if(Volt <= Fault2_VoltCell_DisCharge(Temp))      //1变2
    {
      if(++cnt[2]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
         cnt[2] = 0;
         FltL = 2;
      }
    }
    else
    {
      cnt[2] = 0;
    }
    
    if(Volt >= Recover1_VoltCell_DisCharge(Temp))    //1变0
    {
      if(++cnt[3]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[3] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[3] = 0;
    }
    cnt[0] = 0; 
    cnt[1] = 0;
  }
  else   //2级故障
  {
    cnt[0] = 0; 
    cnt[1] = 0;
    cnt[2] = 0;
    cnt[3] = 0; 
  }
  return(FltL);
}

/*=======================================================================
                              放电单体压差0x03
 ======================================================================*/
static uint8 Fault_DisChg_VoltCD(uint16 V_Diff)  //输入单体最高/低电压
{
  static uint8 cnt[2];      //时间计数
  static uint8 FltL;
  
  //判断故障等级
  if(FltL==0)           //0级故障
  {
    if(V_Diff>=g_BMSMonitor_Volt.Volt_Cell_Diff1)  //0变1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0; 
    }
    cnt[1] = 0; 
  }
  else     //1级故障
  {
    if(V_Diff<=(g_BMSMonitor_Volt.Volt_Cell_Diff1 - 500))    //1变0
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[0] = 0; 
  }
  return(FltL);
}

/*=======================================================================
                                放电高温0x04
 ======================================================================*/
static uint8 Fault_DisChg_TempH(uint8 Temp)  //输入温度
{
  static uint8 cnt[4];      //时间计数
  static uint8 FltL;
  
  if(FltL==0)           //0级故障
  {
    if(Temp>=g_BMSMonitor_Temp.Temp_DisCharge_High1)  //0变1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0;
    }
    
    if(Temp>=g_BMSMonitor_Temp.Temp_DisCharge_High2)  //0变2
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 2;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[2] = 0; 
    cnt[3] = 0;
  }
  else if(FltL == 1) //1级故障
  {
    if(Temp>=g_BMSMonitor_Temp.Temp_DisCharge_High2)      //1变2
    {
      if(++cnt[2]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
         cnt[2] = 0;
         FltL = 2;
      }
    }
    else
    {
      cnt[2] = 0;
    }
    
    if(Temp<=(g_BMSMonitor_Temp.Temp_DisCharge_High1 - 2))    //1变0
    {
      if(++cnt[3]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[3] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[3] = 0;
    }
    cnt[0] = 0; 
    cnt[1] = 0;
  }
  else   //2级故障
  {
    cnt[0] = 0; 
    cnt[1] = 0;
    cnt[2] = 0;
    cnt[3] = 0; 
  }
  return(FltL);
}

/*=======================================================================
                                放电低温0x05
 ======================================================================*/
  
//判断故障等级
static uint8 Fault_DisChg_TempL(uint8 Temp)  //输入温度
{
  static uint8 cnt[4];      //时间计数
  static uint8 FltL;
  
  if(FltL==0)           //0级故障
  {
    if(Temp<=g_BMSMonitor_Temp.Temp_DisCharge_Low1)  //0变1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0;
    }
    
    if(Temp<=g_BMSMonitor_Temp.Temp_DisCharge_Low2)  //0变2
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 2;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[2] = 0; 
    cnt[3] = 0;
  }
  else if(FltL == 1) //1级故障
  {
    if(Temp<=g_BMSMonitor_Temp.Temp_DisCharge_Low2)      //1变2
    {
      if(++cnt[2]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
         cnt[2] = 0;
         FltL = 2;
      }
    }
    else
    {
      cnt[2] = 0;
    }
    
    if(Temp>=(g_BMSMonitor_Temp.Temp_DisCharge_Low1 + 3))    //1变0
    {
      if(++cnt[3]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[3] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[3] = 0;
    }
    cnt[0] = 0; 
    cnt[1] = 0;
  }
  else   //2级故障
  {
    cnt[0] = 0; 
    cnt[1] = 0;
    cnt[2] = 0;
    cnt[3] = 0; 
  }
  return(FltL);
}

/*=======================================================================
                              放电温差0x06
 ======================================================================*/
static uint8 Fault_DisChg_TempD(uint8 T_Diff)  //输入单体最高/低电压
{
  static uint8 cnt[2];      //时间计数
  static uint8 FltL;
  
  //判断故障等级
  if(FltL==0)           //0级故障
  {
    if(T_Diff>=(g_BMSMonitor_Temp.Temp_DisCharge_Diff1))  //0变1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0; 
    }
    cnt[1] = 0; 
  }
  else     //1级故障
  {
    if(T_Diff<=(g_BMSMonitor_Temp.Temp_DisCharge_Diff1-2))    //1变0
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[0] = 0; 
  }
  return(FltL);
}

/*=======================================================================
                                放电过流0x07
 ======================================================================*/
static uint8 Fault_DisChg_CurrH(float Current)//输入电流
{
  static uint8 cnt[4];      //时间计数
  static uint8 FltL;
  
  if(FltL==0)           //0级故障
  {
    if(Current>=(g_BMSMonitor_Curr.Current_DisCharge_High1*0.1-750))  //0变1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0;
    }
    
    if(Current>=(g_BMSMonitor_Curr.Current_DisCharge_High2*0.1-750))  //0变2
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 2;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[2] = 0; 
    cnt[3] = 0;
  }
  else if(FltL == 1) //1级故障
  {
    if(Current>=(g_BMSMonitor_Curr.Current_DisCharge_High2*0.1-750))      //1变2
    {
      if(++cnt[2]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
         cnt[2] = 0;
         FltL = 2;
      }
    }
    else
    {
      cnt[2] = 0;
    }
    
    if(Current<=(g_BMSMonitor_Curr.Current_DisCharge_High1*0.1-750 - 20))    //1变0
    {
      if(++cnt[3]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[3] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[3] = 0;
    }
    cnt[0] = 0; 
    cnt[1] = 0;
  }
  else   //2级故障
  {
    cnt[0] = 0; 
    cnt[1] = 0;
    cnt[2] = 0;
    cnt[3] = 0; 
  }
  return(FltL);
}

/*=======================================================================
                                放电绝缘故障0x08
 ======================================================================*/
static uint8 Fault_DisChg_Insul(uint16 Insul)
{
  static uint8 cnt[4];      //时间计数
  static uint8 FltL;
  
  if(FltL==0)           //0级故障
  {
    
    if(Insul<=(g_BMSMonitor_Insul.Insulation_Resis2*0.1))  //0变2
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 2;
      }
    }
    else
    {
      cnt[0] = 0;
    }
  }
  return(FltL);
}


/*=======================================================================
                              充电总压高0x11
 ======================================================================*/
static uint8 Fault_Charge_VoltSH(uint32 Volt)  //输入系统电压
{
  static uint8 cnt[4];      //时间计数
  static uint8 FltL;
  
  if(FltL==0)           //0级故障
  {
    if(Volt/1000.0>=g_BMSMonitor_Volt.Volt_Sys_High1)  //0变1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0;
    }
    
    if(Volt/1000.0>=g_BMSMonitor_Volt.Volt_Sys_High2)  //0变2
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 2;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[2] = 0; 
    cnt[3] = 0;
  }
  else if(FltL == 1) //1级故障
  {
    if(Volt/1000.0>=g_BMSMonitor_Volt.Volt_Sys_High2)      //1变2
    {
      if(++cnt[2]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
         cnt[2] = 0;
         FltL = 2;
      }
    }
    else
    {
      cnt[2] = 0;
    }
    
    if(Volt/1000.0<=(g_BMSMonitor_Volt.Volt_Sys_High1 - 0.5*SYS_SERIES_YiDongLi))    //1变0
    {
      if(++cnt[3]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[3] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[3] = 0;
    }
    cnt[0] = 0; 
    cnt[1] = 0;
  }
  else   //2级故障
  {
    cnt[0] = 0; 
    cnt[1] = 0;
    cnt[2] = 0;
    cnt[3] = 0; 
  }
  return(FltL);
}

/*=======================================================================
                              充电单体电压高0x12
 ======================================================================*/
static uint8 Fault_Charge_VoltCH(uint16 Volt)  //输入单体电压和环境温度
{
  static uint8 cnt[4];      //时间计数
  static uint8 FltL;
  
  if(FltL==0)           //0级故障
  {
    if(Volt>=g_BMSMonitor_Volt.Volt_Cell_High1)  //0变1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0;
    }
    
    if(Volt>=g_BMSMonitor_Volt.Volt_Cell_High2)  //0变2
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 2;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[2] = 0; 
    cnt[3] = 0;
  }
  else if(FltL == 1) //1级故障
  {
    if(Volt>=g_BMSMonitor_Volt.Volt_Cell_High2)      //1变2
    {
      if(++cnt[2]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
         cnt[2] = 0;
         FltL = 2;
      }
    }
    else
    {
      cnt[2] = 0;
    }
    
    if(Volt<=(g_BMSMonitor_Volt.Volt_Cell_High1 - 500))    //1变0
    {
      if(++cnt[3]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[3] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[3] = 0;
    }
    cnt[0] = 0; 
    cnt[1] = 0;
  }
  else   //2级故障
  {
    cnt[0] = 0; 
    cnt[1] = 0;
    cnt[2] = 0;
    cnt[3] = 0; 
  }
  return(FltL);
}

/*=======================================================================
                              充电单体压差0x13
 ======================================================================*/
static uint8 Fault_Charge_VoltCD(uint16 V_Diff)  //输入单体最高/低电压
{
  static uint8 cnt[2];      //时间计数
  static uint8 FltL;
  
  //判断故障等级
  if(FltL==0)           //0级故障
  {
    if(V_Diff>=g_BMSMonitor_Volt.Volt_Cell_Diff1)  //0变1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0; 
    }
    cnt[1] = 0; 
  }
  else     //1级故障
  {
    if(V_Diff<=(g_BMSMonitor_Volt.Volt_Cell_Diff1 - 500))    //1变0
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[0] = 0; 
  }
  return(FltL);
}

/*=======================================================================
                                充电高温0x14
 ======================================================================*/

//判断故障等级
static uint8 Fault_Charge_TempH(uint8 Temp)  //输入温度
{
  static uint8 cnt[4];      //时间计数
  static uint8 FltL;
  
  if(FltL==0)           //0级故障
  {
    if(Temp>=g_BMSMonitor_Temp.Temp_Charge_High1)  //0变1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0;
    }
    
    if(Temp>=g_BMSMonitor_Temp.Temp_Charge_High2)  //0变2
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 2;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[2] = 0; 
    cnt[3] = 0;
  }
  else if(FltL == 1) //1级故障
  {
    if(Temp>=g_BMSMonitor_Temp.Temp_Charge_High2)      //1变2
    {
      if(++cnt[2]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
         cnt[2] = 0;
         FltL = 2;
      }
    }
    else
    {
      cnt[2] = 0;
    }
    
    if(Temp<=(g_BMSMonitor_Temp.Temp_Charge_High1 - 2))    //1变0
    {
      if(++cnt[3]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[3] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[3] = 0;
    }
    cnt[0] = 0; 
    cnt[1] = 0;
  }
  else   //2级故障
  {
    cnt[0] = 0; 
    cnt[1] = 0;
    cnt[2] = 0;
    cnt[3] = 0; 
  }
  return(FltL);
}

/*=======================================================================
                                充电低温0x15
 ======================================================================*/
 
//判断故障等级
static 
uint8 Fault_Charge_TempL(uint8 Temp)  //输入温度
{
  static uint8 cnt[4];      //时间计数
  static uint8 FltL;
  
  if(FltL==0)           //0级故障
  {
    if(Temp<=g_BMSMonitor_Temp.Temp_Charge_Low1)  //0变1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0;
    }
    
    if(Temp<=g_BMSMonitor_Temp.Temp_Charge_Low2)  //0变2
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 2;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[2] = 0; 
    cnt[3] = 0;
  }
  else if(FltL == 1) //1级故障
  {
    if(Temp<=g_BMSMonitor_Temp.Temp_Charge_Low2)      //1变2
    {
      if(++cnt[2]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
         cnt[2] = 0;
         FltL = 2;
      }
    }
    else
    {
      cnt[2] = 0;
    }
    
    if(Temp>=(g_BMSMonitor_Temp.Temp_Charge_Low1 + 5))    //1变0
    {
      if(++cnt[3]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[3] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[3] = 0;
    }
    cnt[0] = 0; 
    cnt[1] = 0;
  }
  else   //2级故障
  {
    cnt[0] = 0; 
    cnt[1] = 0;
    cnt[2] = 0;
    cnt[3] = 0; 
  }
  return(FltL);
}

/*=======================================================================
                              充电温差0x16
 ======================================================================*/
static 
uint8 Fault_Charge_TempD(uint8 T_Diff)  //输入单体最高/低电压
{
  static uint8 cnt[2];      //时间计数
  static uint8 FltL;
  
  //判断故障等级
  if(FltL==0)           //0级故障
  {
    if(T_Diff>=g_BMSMonitor_Temp.Temp_Charge_Diff1)  //0变1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0; 
    }
    cnt[1] = 0; 
  }
  else     //1级故障
  {
    if(T_Diff<=(g_BMSMonitor_Temp.Temp_Charge_Diff1-2))    //1变0
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[0] = 0; 
  }
  return(FltL);
}

/*=======================================================================
                                充电过流0x17
 ======================================================================*/

static 
uint8 Fault_Charge_CurrH(float Current)//输入温度
{
  static uint8 cnt[4];      //时间计数
  static uint8 FltL;
  
  if(FltL==0)           //0级故障
  {
    if(abs(Current)>=abs(g_BMSMonitor_Curr.Current_Charge_High1*0.1-750))  //0变1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0;
    }
    
    if(abs(Current)>=abs(g_BMSMonitor_Curr.Current_Charge_High2*0.1-750))  //0变2
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 2;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[2] = 0; 
    cnt[3] = 0;
  }
  else if(FltL == 1) //1级故障
  {
    if(abs(Current)>=abs(g_BMSMonitor_Curr.Current_Charge_High2*0.1-750))      //1变2
    {
      if(++cnt[2]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
         cnt[2] = 0;
         FltL = 2;
      }
    }
    else
    {
      cnt[2] = 0;
    }
    
    if(abs(Current)<=abs(g_BMSMonitor_Curr.Current_Charge_High1*0.1-750 - 5))    //1变0
    {
      if(++cnt[3]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[3] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[3] = 0;
    }
    cnt[0] = 0; 
    cnt[1] = 0;
  }
  else   //2级故障
  {
    cnt[0] = 0; 
    cnt[1] = 0;
    cnt[2] = 0;
    cnt[3] = 0; 
  }
  return(FltL);
}

/*=======================================================================
                           子板掉线故障故障0x18
 ======================================================================*/
static 
uint8 Fault_CSSU_OffLine(void)
{
  static uint16 cnt;      
  static uint8 state=0;
  if(HeartBeat.HeartBeat_CSSU1 == 1 )
  { 
     HeartBeat.HeartBeat_CSSU1 = 0;
     state = 0;
     cnt = 0;      
  }
  else
  {
     if(++cnt*PERIOD_DISCHARGE/1000 >= 10)//10S
     {
       cnt = 0;
       state = 1; 
       //memset(&g_FromCSSU_Temp, 0x00, sizeof(FromCSSU_Temp_T));//子板掉线后清掉子板信息
       //memset(&g_FromCSSU_Volt, 0x00, sizeof(FromCSSU_Volt_T)); 
       //memset(&g_FromCSSU_FltData, 0x00, sizeof(FromCSSU_FltData_T));
     }
  }
  return state;
}

/*=======================================================================
                              充电绝缘故障0x19
 ======================================================================*/
static 
uint8 Fault_Charge_Insul(uint16 Insul)
{
  static uint8 cnt[4];      //时间计数
  static uint8 FltL;
  
  if(FltL==0)           //0级故障
  {
    
    if(Insul <= g_BMSMonitor_Insul.Insulation_Resis2*0.1)  //0变2
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 2;
      }
    }
    else
    {
      cnt[0] = 0;
      //FltL = 0;
    }
  }
  return(FltL);
} 


/*=======================================================================
                            继电器粘连故障0x19
 ======================================================================*/ 
static 
uint8 Fault_Relay_BreakDown(void)
{
   static uint8 cnt[2];
   static uint8 flt; 
   if(Port_StateGet(Relay_Positive_PORT, Relay_Positive_pin) == Relay_ON)//继电器开启的状态
   {
      if(abs(g_VoltInfo.SysVolt_Total/10000.0-g_FromCSSU_Volt.InsulVolt_Total/10000.0)>0.5*CELL_VOLT_NOMINAL*SYS_SERIES_YiDongLi)
      {
          if(++cnt[0]*PERIOD_DISCHARGE/1000>=2)
          {
             cnt[0] = 0;    
             flt = 1;
          }
      }
      else
      {
         flt = 0;
         cnt[0] = 0;   
      }
      cnt[1] = 0;
   }
   else//继电器关闭的状态
   {
      if(abs(g_VoltInfo.SysVolt_Total/10000.0-g_FromCSSU_Volt.InsulVolt_Total/10000.0)<0.5*CELL_VOLT_NOMINAL*SYS_SERIES_YiDongLi)
      {
          if(++cnt[1]*PERIOD_DISCHARGE/1000>=2)
          {
             cnt[1] = 0;   
             flt = 1;
          }
      }
      else
      {
         flt = 0;
         cnt[1] = 0;     
      }
      cnt[0] = 0;
   }
   return flt;
}

/*=======================================================================
                           充电桩掉线故障0x20
 ======================================================================*/
static 
uint8 Fault_Charge_OffLine(void)
{
  static uint16 cnt;      
  static uint8 state=0;
  if(HeartBeat.HeartBeat_Charge == 1 )
  { 
     HeartBeat.HeartBeat_Charge = 0;
     state = 0;
     cnt = 0;      
  }
  else
  {
     if(++cnt*PERIOD_DISCHARGE/1000 >= 50)//50S
     {
       cnt = 0;
       state = 1; 
     }
  }
  return state;
}
