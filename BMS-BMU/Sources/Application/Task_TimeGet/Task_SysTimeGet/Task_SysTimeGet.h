/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_SysTimeGet.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _STATICYIMEGET_H_
#define _STATICYIMEGET_H_  
  
  #include  "TypeDefinition.h"

  typedef struct
  {
    uint32  SOC_Static_Time;           //SOC静置时间,本次开启时间-上次关闭时间  
    uint32  BMS_PowerOff_Time;         //断电前的存储时间
    uint16  BMS_SleepStatic_Time;      //统计BMS进入休眠状态前的总时间
    uint32  BMS_TotalRun_MiniteTime;   //BMS总运行时间,单位:分钟
    
    uint8  BMS_StartRun_Time;         //上电时的分钟时间
    
  }SysTime_T;
  extern SysTime_T g_SysTime;

  void  Time_Init(void);
  uint8 Sleep_StaticTime(uint8, uint8, float, float, uint16);
  void Task_SysTimeGet(void);

#endif