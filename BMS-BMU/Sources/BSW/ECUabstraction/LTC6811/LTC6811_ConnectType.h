/*=======================================================================
 *Subsystem:   裸机
 *File:        LTC6811_ConnectType.h
 *Author:      WenM
 *Description: 此配置文件主要是针对外部电池接线的配置
 ========================================================================
 * History:    修改历史记录列表
 * 1. Date:
      Author:
      Modification:
========================================================================*/
#ifndef _LTC6811_CONNECTTYPE_H_
#define _LTC6811_CONNECTTYPE_H_  
  
  #define NUM_pack               0   //子板的编号(决定子板的ID号)
  #define NUM_IC                 3   //子板6811的个数
  #define NUM_Tem                5   //子板测试的温度个数
  
  //各个6804的具体配置
  //导线开路的配置数
  #define NUM1_Batper            8
  #define NUM2_Batper            8  
  #define NUM3_Batper            9
  #define NUM4_Batper            0
  #define NUM5_Batper            0
  //第1个6811的电压采集连接方式
  #define NUM1_Batper_true       8 //每个6804测量电池的个数
  #define NUM1_Batper_front      4 //6804中第一个ADC1接的电池个数
  #define NUM1_Batper_rear       4 //6804中第二个ADC2接的电池个数
  #define NUM1_Batper_empty      (6-NUM1_Batper_front) //第一个ADC空出的电池节数(6-NUM1_Batper_front)
  //第2个6811的电压采集连接方式
  #define NUM2_Batper_true       8
  #define NUM2_Batper_front      4
  #define NUM2_Batper_rear       4
  #define NUM2_Batper_empty      (6-NUM2_Batper_front)
  //第3个6811的电压采集连接方式
  #define NUM3_Batper_true       9
  #define NUM3_Batper_front      5
  #define NUM3_Batper_rear       4
  #define NUM3_Batper_empty      (6-NUM3_Batper_front)
  //第4个6811的电压采集连接方式
  #define NUM4_Batper_true       0
  #define NUM4_Batper_front      0
  #define NUM4_Batper_rear       0
  #define NUM4_Batper_empty      0
  //第5个6811的电压采集连接方式
  #define NUM5_Batper_true       0
  #define NUM5_Batper_front      0
  #define NUM5_Batper_rear       0
  #define NUM5_Batper_empty      0
  //一个子板测试的电压总个数
  #define NUM_Battery      (NUM1_Batper_true+NUM2_Batper_true+NUM3_Batper_true+NUM4_Batper_true+NUM5_Batper_true)

#endif