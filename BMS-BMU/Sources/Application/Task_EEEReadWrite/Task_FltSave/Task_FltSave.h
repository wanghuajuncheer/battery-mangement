/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_FltSave.h
 *Author:      WenMing
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _TASK_FLT_SAVE_H_
#define _TASK_FLT_SAVE_H_

  #include  "TypeDefinition.h"
  
  #define Eeprom_Communication_ID  0x1610C0F4  
 
  typedef struct
  {
    uint8 ReadFlag;   //读取存储故障信息
    uint8 ClearFlag;  //清楚存储故障信息
  }FltCodeSave_T;
  extern  FltCodeSave_T  g_FltCodeSave;
   
  
  void Task_FltCodeProcess(void);
  void Task_FltCodeStore(void);
  
  
#endif