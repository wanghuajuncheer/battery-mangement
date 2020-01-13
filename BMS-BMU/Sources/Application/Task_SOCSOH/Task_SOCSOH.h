 /*=======================================================================
 *Subsystem:   裸机
 *File:        Task_SOCSOH.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef  _TASK_SOCSOH_H_
#define  _TASK_SOCSOH_H_
  
  #include  "TypeDefinition.h"
 

  #define SOC_PERIOD      100          /* SOC的计算周期是100ms */
 
  #define HALLCHANNEL     7           //宏定义霍尔传感器通道
  #define HALL_RANGE      750         //霍尔传感器的量程
  
  //SOC信息结构体
  typedef  struct
  {
    uint16  SOC_Init;                   //SOC初始化
    float   SOC_LowestVoltGet;          //最低电压对应的SOC值（0-1）
    float   SOC_HighestVoltGet;         //最高电压对应的SOC值（0-1）  
    uint16  SOC_CalTime;                //SOC计时 1s自增一次
    float   SOC_ValueRead;              //SOC读取值：单位为1；0-1,读取的值
    float   SOC_ValueVoltGet;           //SOC计算值：单位为1；0-1
    float   SOC_ValueInitDiff;         //SOC初值误差，读取值-计算值
    float   SOC_ValueRealtimeDiff;     //SOC实时误差，逐渐递减
    uint8   SOC_CheckTable_Flag;        //常电状态下，对是否进行查表进行标记
    
  }SOCInfo_T;
  extern    SOCInfo_T    g_SOCInfo;
  
  
  typedef  struct
  {
    float   Energy_Once_DisCharge;           //单次放电量
    float   Energy_Total_Charge;             //总充电量 
    float   Energy_Total_DisCharge;          //总放电量
     
  }EnergyInfo_T;
  extern    EnergyInfo_T    g_EnergyInfo;

  void Task_SOCSOH(void);
  
#endif