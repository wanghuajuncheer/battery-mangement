/*=======================================================================
 *Subsystem:   裸机
 *File:        DataFromCSSU.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _DATA_FROM_CSSU_H_
#define _DATA_FROM_CSSU_H_

  #include "TypeDefinition.h"
  #include  "BattInfoConfig.h"
  #include "CAN.h"
  
    //电压信息结构体
  typedef  struct
  {
    uint16  CellVolt[SYS_SERIES_YiDongLi];//单体电压
    uint16  CellVolt_Max;           //单体最大电压
    uint16  CellVolt_Min;           //单体最小电压
    uint8   CellVolt_MaxNode;       //单体最大电压节点
    uint8   CellVolt_MinNode;       //单体最小电压节点
    //uint16  CellVolt_Diff;          //子板压差
    //uint16  CellVolt_Ave;           //只会用于均衡控制
    uint32  CSSUVolt_Total;         //子板总压
    uint32  InsulVolt_Total;        //绝缘总压
  }FromCSSU_Volt_T;
  extern FromCSSU_Volt_T g_FromCSSU_Volt;
  
  //温度信息结构体
  typedef  struct
  {
    uint8   CellTemp[SYS_NUMBER_MODULE_TEMP];   //单体温度      分辨率：1℃  偏移量：-40
    uint8   CellTemp_Max;           //单体最高温度       分辨率：1℃  偏移量：-40
    uint8   CellTemp_MaxNode;       //单体最高温度节点
    uint8   CellTemp_Min;           //单体最低温度       分辨率：1℃  偏移量：-40
    uint8   CellTemp_MinNode;       //单体最低温度节点
    uint8   CellTemp_Ave;           //单体平均温度       分辨率：1℃  偏移量：-40
    //uint8   CellTemp_Diff;          //子板温差
  }FromCSSU_Temp_T;
  extern FromCSSU_Temp_T g_FromCSSU_Temp;

  
  typedef struct
  {
    uint8   OpenWire_Status;         //导线开路状态
    uint8   CSSUFlt_ChipTemp;        //子板芯片温度故障
    uint8   CSSU_BalanceState;       //子板的均衡状态,00:未均衡
  }FromCSSU_FltData_T;
  extern FromCSSU_FltData_T g_FromCSSU_FltData;

  
  void DataFromCSSU(pCANFRAME data);

#endif