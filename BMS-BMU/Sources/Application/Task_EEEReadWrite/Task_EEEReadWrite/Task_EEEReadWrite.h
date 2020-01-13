/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_EEEReadWrite.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _TASK_EEE_RAEDWRITE_H_
#define _TASK_EEE_RAEDWRITE_H_

  #include  "TypeDefinition.h"
  

  #define EEprom_Ptr      0x0E00  //数据地址标记位起始地址
  #define EEprom_Baseadrr 0x0E10  //变量存储的基地址
  #define EEprom_Length   16      //存储轮询长度
  #define Elem_Num 26             //定义存储变量地址的长度
  
  typedef struct 
  {
    uint8   pEErom_base;
    uint8   EE_Value;       //EEPROM储存正确性判断
    uint16  Charge_Times;    
    
  }EEprom_Data_T;
  extern EEprom_Data_T EEprom_Data; 

  void Get_EEprom_Value(void);
  void EEprom_DateMemset(void);
  void Task_EEpromWrite(void); 

#endif
