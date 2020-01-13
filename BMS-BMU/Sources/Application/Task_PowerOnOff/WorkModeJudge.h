 /*=======================================================================
 *Subsystem:   裸机
 *File:        WorkModeJudge.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/
#ifndef _WORK_MODE_JUDGE_H_
#define _WORK_MODE_JUDGE_H_ 
  
  #include  "TypeDefinition.h"
/*=============================================================
                            工作状态配置
==============================================================*/
  #define           MODE_DISCHARGE          0     //放电
  #define           MODE_CHARGE             1     //快充
  #define           MODE_SLOW_CHARGE        2     //慢充 
  
  #define           QCharge_CC2             9      
  
  typedef struct
  {
    uint8  WorkState;                       // 工作状态
    //uint8  WorkState_EnterTime_DisCharge;   // 放电状态进入计时   
    //uint8  WorkState_EnterTime_Charge;      // 快充状态进入计时     
    //uint8  WorkState_EnterFlag;             // 进入放电程序中记录
  }WorkStateJudge_T;
  extern WorkStateJudge_T  g_WorkStateJudge;
  
  uint8 WokeModeJudgment(void);
#endif