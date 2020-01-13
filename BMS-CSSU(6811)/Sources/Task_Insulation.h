/*=======================================================================
 *Subsystem:   裸机
 *File:        Insulation.h
 *Author:      WenMing
 *Description: 接口
               电压检测开关：               PB0                 
               绝缘V+开关：                   PB1
               绝缘V-开关：                   PB2
               绝缘电压隔离检测开关正：       PB3
               绝缘电压隔离检测开关正：       PB4
               总压检测                       PAD00
               V+电压检测:                    PAD01
               V-电压检测:                    PAD08
               采样频率:                      2MHz
 ========================================================================
 * History:        // 修改历史记录列表，每条修改记录应包括修改日期、修改者及修改内容简述
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _Insulation_H
#define _Insulation_H 

   //#define HighVoltSV       PORTB_PB0     //电压检测开关
   //#define HighVoltSV_Dir   DDRB_DDRB0
   
   #define HighVoltS1       PORTB_PB3     //V+开关
   #define HighVoltS1_Dir   DDRB_DDRB3
   
   #define HighVoltS2       PORTB_PB2     //V-开关
   #define HighVoltS2_Dir   DDRB_DDRB2
   
   //#define HighVoltLS1      PORTB_PB4     //电压隔离检测开关正
   //#define HighVoltLS1_Dir  DDRB_DDRB4
   
   //#define HighVoltLS2      PORTB_PB1     //电压隔离检测开关负
   //#define HighVoltLS2_Dir  DDRB_DDRB1
   
   #define INS_SwitchON         1
   #define INS_SwitchOFF        0             //绝缘MOS管开关
   
   #define DetectCount      12            //采样电压12组
   
   #define LowVoltageDec    5            //低压时的绝缘检测值（未定）
   #define Bias_Resitance   100            //偏置电阻阻值
   
   #define Resistance_Alarm1  100         //绝缘电阻故障等级阈值（未定）
   #define Resistance_Alarm2  50          //绝缘电阻故障等级阈值 （未定） 
     
   #define HVPositiveChannel  6          //绝缘检测正极AD口
   #define HVNegtiveChannel    13          //绝缘检测负极AD口
   #define Stand_Volt          4.5               //宏定义基准电压
   typedef struct
   {
     uint8  insulation_grade;    //绝缘故障等级
     uint8  insulation_curr;     //是否漏电0，1
     uint16 insulation_resist;   //绝缘阻值
     float insulation_Vposit;    //对正电压
     float insulation_Vnegt;     //对负电压
     uint32 insulation_TotalVolt; //绝缘检测总压
     uint8 insulation_Disable;   //使能标记(充电前期不能使能)
     
     uint16 insulation_resist_P;  //正极电阻
     uint16 insulation_resist_N;  //负极电阻
   }IsoResist;
   extern IsoResist IsoDetect;

   void VoltageDetectInit(void);
   void InsulationDetect(void);


#endif