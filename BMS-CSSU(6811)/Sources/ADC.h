/*=======================================================================
 *Subsystem:   裸机
 *File:        ADC.h
 *Author:      WenMing
 *Description: 接口
               总压检测PAD0
               霍尔传感器：PAD03
               分流器：PAD10
               采样频率：2MHz                
========================================================================
 * History:        // 修改历史记录列表，每条修改记录应包括修改日期、修改者及修改内容简述
 * 1. Date:
      Author:
      Modification:
========================================================================*/


#ifndef _ADC_H
#define _ADC_H

#include"Includes.h"
  
 
  void ADC_init(void);                    //AD初始化
  uint16 ADCValue(uint8 channel);         //AD采集通道设置
    



#endif