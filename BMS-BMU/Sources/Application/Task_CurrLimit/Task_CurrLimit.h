/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_CurrLimit.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _TASK_CURR_LIMIT_H_
#define _TASK_CURR_LIMIT_H_

  #include"TypeDefinition.h"

  typedef struct
  {
    float Curr_Charge_Cons;        //充电持续电流大小
  }CurrLimit_T;
  extern CurrLimit_T CurrLimit;   
    
  void Init_TaskCurrLimit(void);
  void Task_CurrLimit(void);  
    
   
#endif