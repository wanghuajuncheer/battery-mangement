 /*=======================================================================
 *Subsystem:   裸机
 *File:        Insulation.c
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
 =============================================================================
 * History:    修改历史记录列表
 * 1. Date:    
      Author:  
      Modification:
===============================================================================*/
 
 
#include"Includes.h"


  IsoResist IsoDetect;        //定义绝缘检测结构体
  
/*=======================================================================
 *函数名:      VoltageDetect
 *功能:        电池包电压检测初始化（总压检测，绝缘检测）
 *参数:        无         
                      
 *返回：       无
 *说明：       ADC 时钟频率：2MHz
========================================================================*/
  void VoltageDetectInit(void) 
  {
     HighVoltS1_Dir = 1;
     HighVoltS1 = INS_SwitchOFF;      //绝缘检测V+开关关闭
     
     HighVoltS2_Dir = 1; 
     HighVoltS2 = INS_SwitchOFF;      //绝缘检测V-开关关闭
     
     //memset(&IsoDetect,0,sizeof(IsoResist));     
  }

/*=======================================================================
 *函数名:      InsulationDetect
 *功能:        电池包绝缘检测
 *参数:        无         
                      
 *返回：       无
 *说明：       ADC 时钟频率：2MHz
========================================================================*/

      
  void InsulationDetect(void) 
  {
      uint8 count,i,j;
      static uint8 Time_Flag,Time_Cnt;
      uint32 SumVpositive,SumVnegtive,total_VOL,total_VOL1;
      static uint16 VoltFlag1,VoltFlag2;
      float Vpositive_1,Vnegtive_1;
      uint16 VposBuff[12],VnegBuff[12],VposBuff1[12],VnegBuff1[12],VposBuff2[12],VnegBuff2[12];
      uint16 Max_Volt,Max_Volt1,Min_Volt,Min_Volt1;
      
      VoltageDetectInit();
      
      if(Time_Flag == 0) 
      { 
        Time_Cnt = 0;           
        for(count = 0; count < 12; count++)//测试12次再去掉最高值和最低值 
        {
          VposBuff[count] = ADCValue(HVPositiveChannel);   //正对底盘：PAD1
          
          VnegBuff[count] = ADCValue(HVNegtiveChannel);    //负对底盘：PAD8
  
        }    
        
        Max_Volt = 0;                 
        Max_Volt1 = 0; 
        Min_Volt = 0xFFFF;
        Min_Volt1 = 0xFFFF;
        SumVpositive =0;
        SumVnegtive =0;
        
        for(i =0; i<12; i++)            // 查找每一个数组中的最大最小值(12组)
        {
           if(VposBuff[i] >=Max_Volt)
           {
              Max_Volt = VposBuff[i];
           } 
           if(VposBuff[i] <= Min_Volt)
           {
              Min_Volt = VposBuff[i];
           }
           
           if(VnegBuff[i] >Max_Volt1)
           {
              Max_Volt1 = VnegBuff[i];
           } 
           if(VnegBuff[i] < Min_Volt1)
           {
              Min_Volt1 = VnegBuff[i];
           }
        }
        
        for(count = 0; count < 12; count++)      //把所有值全部加起来 //
        {
           SumVpositive += VposBuff[count];
           SumVnegtive += VnegBuff[count];
        }
                
        SumVpositive = SumVpositive - Max_Volt - Min_Volt;
        SumVnegtive = SumVnegtive - Max_Volt1 - Min_Volt1; 
                
        IsoDetect.insulation_Vposit = SumVpositive/10.0;
        IsoDetect.insulation_Vnegt = SumVnegtive/10.0;               // 去除最大最小值之后求平均值
                
        SumVpositive =0;
        SumVnegtive =0;
      
        IsoDetect.insulation_TotalVolt = ((IsoDetect.insulation_Vposit + IsoDetect.insulation_Vnegt) * Stand_Volt)/4096.0/100.0*5100.0*10000.0;
      }
       
      
    
    Time_Cnt++;      
    //Task_Flag_Counter.Counter_Insulation++; */
  
  }