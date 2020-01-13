/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_FltLevJudg.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _TASK_FAULTLEVELJUDGE_H_
#define _TASK_FAULTLEVELJUDGE_H_

  #include  "TypeDefinition.h"
  

/*======================================================================
                             故障判断结构体
========================================================================*/
  //==========================充电故障判断============================
  typedef struct
  {
    //-------------------故障等级：按照设定要求-----------------------
    //单体电压
    uint8 Level_Volt_Cell_High;               
    uint8 Level_Volt_Cell_Low; 
    uint8 Level_Volt_Cell_Diff_High;
    //系统电压    
    uint8 Level_Volt_Sys_High; 
    
    //温度     
    uint8 Level_Temp_High;    
    uint8 Level_Temp_Low;     
    uint8 Level_Temp_Diff_High;
    
    //充电电流   
    uint8 Level_Current_Charge_High;
    
    //绝缘检测
    uint8 Level_Insul; 
 
    uint8 Level_Charge_SwitchOff_flag; //存在2级，断开继电器
    
    //均衡
    uint8 Level_Charge_BalanceON_Flag;//存在故障,不进行均衡
           
  }Fault_Charge_T;
  extern Fault_Charge_T g_Flt_Charge;             //充电故障判断
  
  //==========================放电故障判断===========================
  typedef struct
  {
    //-------------------故障等级：按照设定要求-----------------------
    //单体电压
    uint8 Level_Volt_Cell_Low; 
    uint8 Level_Volt_Cell_Diff_High;
    //系统电压    
    uint8 Level_Volt_Sys_Low;
    
    //温度     
    uint8 Level_Temp_High;    
    uint8 Level_Temp_Low;     
    uint8 Level_Temp_Diff_High;
    
    //放电电流   
    uint8 Level_Current_DisCharge_High;
    
    //绝缘检测
    uint8 Level_Insul; 
    
    uint8 Level_DisCharge_SwitchOff_flag; //存在2级，断开继电器
    
  }Fault_DisCharge_T;
  extern Fault_DisCharge_T g_Flt_DisChg;       //放电故障判断
  
  //==============================掉线信号==============================
  typedef struct
  {
    //---------------------故障状态：0正常；1故障----------------------
    uint8 VCU;     
    uint8 HVU;
    
    uint8 CSSU1;
    uint8 Charge;
    uint8 RelayFlt_Positive;
    /*#if(SYS_NUMBER_MODULE>=2)
      uint8 CSSU2;
    #endif
    #if(SYS_NUMBER_MODULE>=3)
      uint8 CSSU3;
    #endif
    #if(SYS_NUMBER_MODULE>=4)
      uint8 CSSU4;
    #endif
    #if(SYS_NUMBER_MODULE>=5)
      uint8 CSSU5;
    #endif
    #if(SYS_NUMBER_MODULE>=6)
      uint8 CSSU6;
    #endif */
          
  }State_Offline_T;
  extern State_Offline_T State_Offline;             //充电故障判断 
  
  //==============================心跳信号==============================
  typedef struct
  {
    //CSSU
    uint8 HeartBeat_CSSU1;
    uint8 HeartBeat_Charge;
    /*#if(SYS_NUMBER_MODULE>=2)
      uint8 HeartBeat_CSSU2;
    #endif
    #if(SYS_NUMBER_MODULE>=3)
      uint8 HeartBeat_CSSU3;
    #endif
    #if(SYS_NUMBER_MODULE>=4)
      uint8 HeartBeat_CSSU4;
    #endif
    #if(SYS_NUMBER_MODULE>=5)
      uint8 HeartBeat_CSSU5;
    #endif
    #if(SYS_NUMBER_MODULE>=6)
      uint8 HeartBeat_CSSU6;
    #endif*/
    //VCU
    //uint8 HeartBeat_VCU;
    //HVU
    //uint8 HeartBeat_HVU;
  }HeartBeat_T;
  extern HeartBeat_T HeartBeat;              //心跳信号
  
/*-======================================================================
                              函数声明
=========================================================================*/
 void Init_TaskFltLevJudg(void);
 void Task_FltLevJudg(void);

#endif