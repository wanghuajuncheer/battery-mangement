 /*=======================================================================
 *Subsystem:   裸机
 *File:        Task_DataProcess.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef  _TASK_DATA_PROCESS_H_
#define  _TASK_DATA_PROCESS_H_

  #include "TypeDefinition.h"  
  #include "BattInfoConfig.h"   

  #define NUM_OPENWIRE   6

  //电压信息结构体
  typedef  struct
  {
    uint16  CellVolt[SYS_SERIES];    //单体电压
    uint16  CellVolt_Max;           //单体最大电压
    uint16  CellVolt_Min;           //单体最小电压
    uint8   CellVolt_MaxNode;       //单体最大电压节点
    uint8   CellVolt_MinNode;       //单体最小电压节点
    uint16  CellVolt_Diff;          //单体压差
    uint16  CellVolt_Ave;           //只会用于均衡控制
    uint32  SysVolt_Total;          //系统总压
  }VoltInfo_T;
  extern VoltInfo_T g_VoltInfo;

  //温度信息结构体
  typedef  struct
  {
    uint8   CellTemp[SYS_NUMBER_TEMP];   //单体温度      分辨率：1℃  偏移量：-40
    uint8   CellTemp_Max;           //单体最高温度       分辨率：1℃  偏移量：-40
    uint8   CellTemp_MaxNode;       //单体最高温度节点
    uint8   CellTemp_Min;           //单体最低温度       分辨率：1℃  偏移量：-40
    uint8   CellTemp_MinNode;       //单体最低温度节点
    uint8   CellTemp_Ave;           //单体平均温度       分辨率：1℃  偏移量：-40
    uint8   CellTemp_Diff;
  }TempInfo_T;
  extern TempInfo_T g_TempInfo;


  //绝缘信息结构体
  typedef  struct
  {
    uint16  Insul_Resis_Pos;          //绝缘正对地电阻
    uint16  Insul_Resis_Neg;          //绝缘负对地电阻
    uint16  Insul_Resis;              //绝缘电阻,取正/负对地电阻中的最小值
    uint8   Insul_FaultGrade;         //绝缘故障等级
    uint8   Insul_Life;               //绝缘信号
    uint16  Insul_Volt;               //绝缘电压
  }InsulInfo_T;
  extern InsulInfo_T g_InsulInfo;


  //导线开路
  typedef struct
  {
    uint8   OpenWire_Status;             //导线开路状态
    uint16  OpenWire_Node[NUM_OPENWIRE]; //导线开路节点
  }OpenWireInfo_T;
  extern OpenWireInfo_T g_OpenWireInfo;

  //采集的数据
  typedef struct
  {
    //uint32  SysVolt_Total;             //系统电压总和    分辨率:0.0001V
    //uint16  InsulVolt_Total;
    uint8   CSSU_Flt_TempH;
    float   DataCollet_Current_Filter;   //滤波处理后的电流
  }DataColletInfo_T;
  extern DataColletInfo_T g_DataColletInfo;
    
  void Init_TaskDataProcess(void);
  void Task_DataProcess(void);

#endif
