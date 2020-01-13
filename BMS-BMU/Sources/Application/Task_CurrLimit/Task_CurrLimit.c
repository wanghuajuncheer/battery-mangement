/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_CurrLimit.C
 *Author:      Wenming
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
 CurrLimit_T CurrLimit;

 static float ChagCurrLimit_Cons(uint8 Temp);
 
 /*=======================================================================
 *函数名:      Init_TaskCurrLimit
 *功能:       
 *参数:        无                                     
 *返回：       无
 *说明：       
========================================================================*/ 
void Init_TaskCurrLimit(void) 
{
  //电流限制变量清零
  memset(&CurrLimit,     0, sizeof(CurrLimit_T));//电流限制数据清零

}
 
/*=======================================================================
 *函数名:      Task_CurrLimit
 *功能:        充放电电流判断
 *参数:        无                                     
 *返回：       无
 *说明：       BMS根据温度条件判断最大输出或充电电流的大小
========================================================================*/ 
void Task_CurrLimit(void)
{
  //充电电流限制
  if(g_WorkStateJudge.WorkState == MODE_CHARGE)
  {
    CurrLimit.Curr_Charge_Cons = ChagCurrLimit_Cons(g_TempInfo.CellTemp_Ave); 
  }
  else
  {
    CurrLimit.Curr_Charge_Cons = 0;
  }
    
  g_Roll_Tick.Roll_Currlimit++;

} 

/*=======================================================================
 *函数名:      ChagCurrLimit_Cons(uint8 Temp)
 *功能:        充电持续电流限制
 *参数:        Temp:温度                                     
 *返回：       Current:限制电流
 *说明：       BMS根据温度条件判断最大输出或充电电流的大小
========================================================================*/ 
static
float ChagCurrLimit_Cons(uint8 Temp)
{
  static float Current;
  
  if(Temp>=(0 +40) && Temp<=(55 +40))
  {
    if(Temp>(50 +40))
    {
      Current = 100;
    }
    else if(Temp>(10 +40))
    {
      Current = 150;
    }
    else if(Temp>(5 +40))
    {
      Current = 100;
    }
    else 
    {
      Current = 50;
    }
  }
  else
  {
    Current = 0;
  }
  return Current;
}