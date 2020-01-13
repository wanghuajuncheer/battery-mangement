/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_Screen.h
 *Author:      WenMing
 *Description: 通信：SCI2.
               接口：PJ0（RXD）；PJ1（TXD）
               波特率：
 ========================================================================
 * History:        // 修改历史记录列表，每条修改记录应包括修改日期、修改者及修改内容简述
 * 1. Date:
      Author:
      Modification:
========================================================================*/


#ifndef _TASK_SCREEN_H_
#define _TASK_SCREEN_H_

#include  "TypeDefinition.h"
#include  "MC9S12XEP100.h"


/*构建头文件及宏定义*/
  
  #define Array_couple              20  //宏定义发送双字节数组个数为20
  #define Array_single              17  //宏定义发送单字节数组个数为17

  #define RS485_Enable       PORTA_PA6
  #define RS485_EnableDir    DDRA_DDRA6


  /*======显示屏显示内容双字节======*/
  typedef union
  { 
    uint8 SCI_Content2[Array_couple];
    struct
    {       
        uint16 RunningTime;           //运行时间
        uint16 BMS_Current;           //电池组电流
      	uint16 BMS_SOC;               //SOC
      	uint16 BMS_SOH;               //SOH
      	uint16 Pack_Hightemp;         //电池组最高温度
      	uint16 Pack_Lowtemp;          //电池组最低温度
      	uint16 Pack_Volt;             //电池组总压
      	uint16 Single_Maxvolt;        //单体最高电压
      	uint16 Single_Lowvolt;        //单体最低电压
      	uint16 iso_resistance;        //绝缘电阻阻值
    }TX2;
  }RS485_couple;                  //双字节SCI发送   
  
  
  /*======显示屏显示内容单字节======*/
  typedef union
  {
    uint8 SCI_Content1[Array_single];
    struct
    {
      uint8	Alam_SOC;               //SOC告警
      uint8 Alam_VoltSL;            //放电总压低
      uint8 Alam_VoltCL;            //放电单体低
      uint8 Alam_TempH_DisChg;      //放电高温
      uint8 Alam_TempL_DisChg;      //放电低温
      uint8 Alam_CurrH_DisChg;      //放电过流
      uint8 Alam_VoltCD_DisChg;     //放电单体压差
      uint8 Alam_TempD_DisChg;      //放电温差
        
      uint8 Alam_VoltSH;            //充电总压低
      uint8 Alam_VoltCH;            //充电单体低
      uint8 Alam_TempH_Charge;      //充电高温
      uint8 Alam_TempL_Charge;      //充电低温
      uint8 Alam_CurrH_Charge;      //充电过流
      uint8 Alam_VoltCD_Charge;     //充电单体压差
      uint8 Alam_TempD_Charge;      //充电温差
      
      uint8 Alam_Insul;             //绝缘故障
      uint8 Alam_Checkself;         //自检状态  
      
    }TX1;     
  }RS485_single;                   //单字节SCI发送
  
  typedef struct
  {
    RS485_couple TxData_couple;
    RS485_single TxData_single;   
  }RS485;
      
  uint8 Init_Screen(void); 
  void Task_ScreenTransfer(void);
   
  
 #endif 

