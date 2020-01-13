/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_BmsAndUpMonitor.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _TASK_BMS_AND_UP_MONITOR_H_
#define _TASK_BMS_AND_UP_MONITOR_H_  

  #include  "TypeDefinition.h"
  #include  "PIT.h"

  //PIT初始化
  enum 
  {
    //PIT初始化正常
    Init_Normal_PIT = 0,
    Init_Fault_PIT_Channel,
    Init_Fault_PIT_Period
  };

  uint8 Init_PIT(void);
  //uint8 Init_PIT(uint8 channel, uint8 perio);
   
#endif