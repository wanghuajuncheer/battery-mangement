/*=======================================================================
 *Subsystem:   Âã»ú
 *File:        DS3231_TimeGet.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _TASK_TIME_GET_H_
#define _TASK_TIME_GET_H_  

  #include  "TypeDefinition.h"
  //#include  "derivative.h" 

  typedef struct
  {
    uint8 IIC_Read_Hour;
    uint8 IIC_Read_Day;
    uint8 IIC_Read_Month;
    uint8 IIC_Read_Year;
    uint8 IIC_Read_Minute;
    //uint8 IIC_Read_Second;
  }Read_IIC_Time_T;
  extern Read_IIC_Time_T Read_IIC_Time;

  uint8 HEX2BCD(uint8 val);
  void  DS3231SN_INIT(uint8 year, uint8 month, uint8 week, uint8 day, uint8 hour, uint8 min);
  void  DS3231_Read_Time(void);

#endif 