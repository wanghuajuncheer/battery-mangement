#include  "includes.h"
static float Charge_CurrInit(float currlimt, float chargeInit);
static float ChargeEnd_CurrentOut(uint16 cellvoltmax, float curr); 
static uint8 ChargeEndJudge(float soc,float curr_end,uint16 cellvoltmax,uint8 temph,uint8 templ, uint8 BMSFlt, uint8 chargeFlt);
/*=======================================================================
 *函数名:      BmsToChargePileInfo(void) 
 *功能:        搜集发送至充电机的信息
 *参数:        无       
 *返回：       无
 
 *说明：       配置需要发送给充电机的信息,提高程序可移植性
========================================================================*/ 
void Charge_VoltCurrRequest(void)
{
  static uint8 cnt;
  float curr;
  //最大电压请求
  g_BMSCharge.Volt_Max_ChargePile = (CELL_VOLT_MAX * SYS_SERIES_YiDongLi + 5);         //最高允许充电端电压 分辨率:0.1V
  if(g_BMSCharge.Control_ChargePile == 1)//出现故障
  {
    g_BMSCharge.Volt_Max_ChargePile = 0;
  }
  //最大电流请求
  curr = Charge_CurrInit(CurrLimit.Curr_Charge_Cons, g_BMSCharge.Curr_Max_ChargePile);
  g_BMSCharge.Curr_Max_ChargePile = ChargeEnd_CurrentOut(g_VoltInfo.CellVolt_Max, curr);//先进行处理 分辨率0.1A
  
  g_BMSCharge.Control_ChargePile  = ChargeEndJudge(g_SOCInfo.SOC_ValueRead, g_BMSCharge.Curr_Max_ChargePile,\
                                                   g_VoltInfo.CellVolt_Max,\
                                                   g_TempInfo.CellTemp_Max, g_TempInfo.CellTemp_Min,\
                                                   g_Flt_Charge.Level_Charge_SwitchOff_flag, g_Charge_State.FltState);
  
  g_BMSCharge.VoltC_Max           = g_VoltInfo.CellVolt_Max;  //单体最高电压 分辨率:0.001V
  g_BMSCharge.VoltC_Min           = g_VoltInfo.CellVolt_Min;  //单体最低电压 分辨率:0.001V
  g_BMSCharge.SOC                 = g_SOCInfo.SOC_ValueRead;  //先进行处理 分辨率:0.4%
  g_BMSCharge.Temp_Max            = g_TempInfo.CellTemp_Max;
  g_BMSCharge.VoltS               = g_VoltInfo.SysVolt_Total; //将g_VoltInfo.SysVolt_Total单位:0.1mV->0.1V
  
  //充电过温
  if(g_Flt_Charge.Level_Temp_High == 2)
  {
    BMSCharge_State.TempH_Cell = 1;
  }
  else
  {
    BMSCharge_State.TempH_Cell = 0;
  }
  
  //充电低温
   if(g_Flt_Charge.Level_Temp_Low == 2)
  {
    BMSCharge_State.TempL_Cell = 1;
  }
  else
  {
    BMSCharge_State.TempL_Cell = 0;
  }
  
  //充电过流
   if(g_Flt_Charge.Level_Current_Charge_High == 2)
  {
    BMSCharge_State.CurrH_Cell = 1;
  }
  else
  {
    BMSCharge_State.CurrH_Cell = 0;
  }
  
  //绝缘过低
   if(g_Flt_Charge.Level_Insul == 2)
  {
    BMSCharge_State.Insul = 1;
  }
  else
  {
    BMSCharge_State.Insul = 0;
  }
  
  //BMS接收报文超时
  if((State_Offline.RelayFlt_Positive == 1)||(State_Offline.CSSU1 == 1))
  {
    BMSCharge_State.BMSGetMsg = 1;
  } 
  else
  {
    BMSCharge_State.BMSGetMsg = 0;
  }
  
  //故障标志 1:任何故障
  if(g_Charge_State.FltState||g_Flt_Charge.Level_Charge_SwitchOff_flag)
  {
    BMSCharge_State.FaultFlag = 1;
  }
  else
  {
    BMSCharge_State.FaultFlag = 0;
  }   
}

/*=======================================================================
 *函数名:      ChargeEndJudge(void) 
 *功能:        充电中止判断
 *参数:        无       
 *返回：       无
 
 *说明：       
========================================================================*/ 
static
uint8 ChargeEndJudge(float soc,float curr_end,uint16 cellvoltmax,uint8 temph,uint8 templ, uint8 BMSFlt, uint8 chargeFlt)
{
  static uint8 cnt[5];
  //SOC到达100%
  if(soc+0.002>=1)        
  {
    if(++cnt[0]>3)
    {
      cnt[0] = 3;
      return 1;
    }
  }
  else
  {
    cnt[0] = 0;
  }
  //充电末期电流小于0.03C
  if(abs(curr_end) < 0.03*SYS_CAPACITY) //0.03*542=16.26A
  {
    if(++cnt[1]>3)
    {
      cnt[1] = 3;
      return (1);  
    }
  }
  else
  {
     cnt[1] = 0;
  }
  //到达单体截止电压
  if(cellvoltmax/10000.0 > CELL_VOLT_MAX)
  {
    if(++cnt[2]>3)
    {
      cnt[2] = 3;
      return (1); 
    }
  }
  else
  {
     cnt[2] = 0;
  }
  //到达最高温度
  if(temph > CELL_TEMP_MAX_CHARGE + 40)
  {
    if(++cnt[3]>3)
    {
      cnt[3] = 3;
      return (1);  
    }
  }
  else
  {
     cnt[3] = 0;
  }
  //低温停止
  if(templ < CELL_TEMP_MIN_CHARGE + 40)
  {
    if(++cnt[4]>3)
    {
      cnt[4] = 3;
      return (1);
    }
  }
  else
  {
     cnt[4] = 0;
  }
  //BMS故障和充电机输出故障
  if((BMSFlt == 1)||(chargeFlt == 1))
  {
    return 1;
  }

  return (0);
}

/*=======================================================================
 *函数名:      Charge_Curr(uint16, float curr) 
 *功能:        充电充电末端的电流请求
 *参数:        chargeInit：初始化时的电流值
               curr:电流
 *返回：       float:充电枪电流值
 *说明：       判断电池是否到达3.63V,若到达则按原来请求电流的0.7倍再请求
               电流减小低于3.63V时，就按原来的电流大小请求
========================================================================*/
static 
float Charge_CurrInit(float currlimt, float chargeInit)
{
   float currIn;
   if (chargeInit <= 0)//初始的chargeInit值的确定
   {
      chargeInit = currlimt;
   }
   if(chargeInit > currlimt)//当到达充电末期后变电流的值大于电流限制值时按限制值
   {  
      currIn = currlimt;
   }
   else//否则按变电流的值
   {
      currIn = chargeInit; 
   }
   return currIn;
}

/*=======================================================================
 *函数名:      ChargeEnd_CurrentOut(uint16, float curr) 
 *功能:        充电末端的变电流请求
 *参数:        cellvoltmax：最大单体电压
               currIn:电流
 *返回：       float:充电枪电流值
 *说明：       判断电池是否到达3.63V,若到达则按原来请求电流的0.7倍再请求
               电流减小低于3.63V时，就按原来的电流大小请求
========================================================================*/
static 
float ChargeEnd_CurrentOut(uint16 cellvoltmax, float currIn)
{
  float current;
  static uint8 currflag;
  static uint8 cnt;
  if((cellvoltmax >= 36300)&&(currflag == 0)) //currflag的作用为避免重复的降电流
  {
    currflag = 1;
    cnt = 0;
    if(currIn >= 0.01*SYS_CAPACITY)
    {
      current = 0.7*currIn;  
    }
    else
    {
      current = currIn;
    }
  }
  else
  {
    if(++cnt>3)//3s之后再判断电压情况再决定是否进行降电流
    {
      cnt = 0;
      currflag = 0;
    }
    current = currIn; 
  }
  return current;
}




