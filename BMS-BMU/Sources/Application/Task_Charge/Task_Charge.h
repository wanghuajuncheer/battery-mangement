/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_Charge.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _TASK_CHARGE_H_
#define _TASK_CHARGE_H_  

  #include  "TypeDefinition.h"  
  #include  "CAN.h"  
  

  typedef struct
  {
    uint16  Volt_Max_ChargePile;//最大请求电压
    uint16  Curr_Max_ChargePile;//最大请求电流
    uint8   Control_ChargePile; //充电桩充电控制位
   
    uint16  VoltC_Max;          //最大单体电压
    uint16  VoltC_Min;          //最小单体电压
    float   SOC;                //发送SOC值
    uint8   Temp_Max;           //最大温度
    uint16  VoltS;              //系统总压
  }BMSCharge_T;
  extern BMSCharge_T g_BMSCharge;
  
  typedef struct
  {
    uint8   TempH_Cell;
    uint8   TempL_Cell;
    uint8   CurrH_Cell;
    uint8   Insul;
    uint8   BMSGetMsg;
    uint8   FaultFlag;
  
  }BMSCharge_State_T;
  extern BMSCharge_State_T BMSCharge_State;
  
  typedef struct
  {
    uint16  Volt_ChargePileOut;
    uint16  Curr_ChargePileOut;
   
  }ChargePileBMS_T;
  extern ChargePileBMS_T ChargePileBMS;
  
  typedef struct
  {
    uint8   Hard;
    uint8   TempH_ChargePile;
    uint8   VoltL_ChargePile;
    uint8   On_Line;
    uint8   GetMsg;
  
    uint8   FltState;
    
  }Charge_State_T;
  extern Charge_State_T g_Charge_State;
  
  //BMS发送充电机初始化
  uint8 CAN_ToChargeInit(void);
  
  //BMS发送至充电桩
  void Task_Charge(void);
  void Charge_VoltCurrRequest(void); 
  
  //充电桩发送至BMS
  void CAN_ChargetoBMS(pCANFRAME data);
  
#endif