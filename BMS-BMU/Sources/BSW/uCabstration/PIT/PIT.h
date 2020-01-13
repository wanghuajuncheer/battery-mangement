/*=======================================================================
 *Subsystem:   裸机
 *File:        PIT.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _PIT_H_
#define _PIT_H_  

  #include  "TypeDefinition.h"
  #include  "MC9S12XEP100.h"

//开放或禁止PIT溢出中断,channel为通道号
  #define EnablePIT(channel)           (PITINTE |= (1<<channel))   //开放定时器溢出中断
  #define DisablePIT(channel)          (PITINTE &= ~(1<<channel))  //禁止定时器溢出中断

  
  typedef struct 
  {
    //uint16 T10ms;       // 10ms任务时间标志;
    uint16 T500ms;      // 500ms任务时间标志;
    //uint16 T1s;         // 1s任务时间标志; 
  }PIT_TimePeriod_T;
  extern PIT_TimePeriod_T PIT_TimePeriod;  
  
  uint8 PIT0_Init(void);
  //void PITInit(uint8 channel,uint8 MUXSEL,uint8 MTLD,uint16 LD);

#endif                                                                           
