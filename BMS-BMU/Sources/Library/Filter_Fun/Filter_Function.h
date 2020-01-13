/*=======================================================================
 *Subsystem:   裸机
 *File:        Filter_Function.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _FILTER_FUNCTION_H_
#define _FILTER_FUNCTION_H_

  float FilterFunction_Ave(float*input, uint8 arrary);    //电流滤波函数,对20个以内的电流值求平均值
  float FilterFunction_Median(float(*adc)(void), float Median);                //电流滤波函数，霍尔滤波

#endif