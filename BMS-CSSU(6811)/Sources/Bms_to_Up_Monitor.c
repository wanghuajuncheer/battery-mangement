/*=======================================================================
 *Subsystem:   裸机
 *File:        Bms_to_Up_Monitor.C
 *Author:      ZWB
 *Description: 通信：
               接口：
               波特率：
/*========================================================================
 * History:    修改历史记录列表
 * 1. Date:    
      Author: 
      Modification: 
/*=======================================================================*/

#include "Includes.h"


/*=======================================================================
 *函数名:      Bms_to_Up_Monitor(void) 
 *功能:        BMS information to UpMonitor
 *参数:        无       
 *返回：       无
 *说明：       BMS发送信息给上位机；
========================================================================*/
void Bms_to_Up_Monitor(void) 
{
    uint8  i,j,Return_Value;
    uint16  BalanceVolt;
    CANFRAME  BMS_to_Upmonitor;
    uint8 batt,batt1; 
    
    /*
    for(i=0; i<25; i++)
    {
      VoltInfo.CellVolt[i] = 34500+i;
      VoltInfo.CellVolt_Total = VoltInfo.CellVolt_Total+VoltInfo.CellVolt[i];
    }
     VoltInfo.CellVolt_Max = 34524; 
     VoltInfo.CellVolt_Min = 34500;
     VoltInfo.CellVolt_MaxNode = 24;
     VoltInfo.CellVolt_MinNode =0;            
    
    for(i=0; i<5; i++)
    {
      TempInfo.CellTemp[i] = 22+i;
      TempInfo.CellTemp_tatoltemp = TempInfo.CellTemp_tatoltemp+TempInfo.CellTemp[i];
    }
     TempInfo.CellTemp_Max = 22;
     TempInfo.CellTemp_Min = 22;
     TempInfo.CellTemp_MaxNode = 4;
     TempInfo.CellTemp_MinNode = 0;
    */ 
    
    
    batt=(NUM1_Batper_true+NUM2_Batper_true+NUM3_Batper_true+NUM4_Batper_true+NUM5_Batper_true)/3;
    batt1=(NUM1_Batper_true+NUM2_Batper_true+NUM3_Batper_true+NUM4_Batper_true+NUM5_Batper_true)%3;
       
    BMS_to_Upmonitor.m_ID = BMS_Send_Information1;//0x18FF9701       
  	BMS_to_Upmonitor.m_IDE = 1;
  	BMS_to_Upmonitor.m_RTR = 0;
  	BMS_to_Upmonitor.m_dataLen = 8;
  	BMS_to_Upmonitor.m_priority = 6;
    for(i = 0; i <batt ; i++) 
    {
      BMS_to_Upmonitor.m_data[0] = 0xFF;//子板备用      
      BMS_to_Upmonitor.m_data[1] = (uint8)(i);//每个6804采集电压的编号,从0开始往下推  
      BMS_to_Upmonitor.m_data[2] = (uint8)VoltInfo.CellVolt[i*3];
      BMS_to_Upmonitor.m_data[3] = (VoltInfo.CellVolt[i*3]>>8)&0X00FF;
      BMS_to_Upmonitor.m_data[4] = (uint8)VoltInfo.CellVolt[i*3+1];
      BMS_to_Upmonitor.m_data[5] = (VoltInfo.CellVolt[i*3+1]>>8)&0X00FF;
      BMS_to_Upmonitor.m_data[6] = (uint8)VoltInfo.CellVolt[i*3+2];
      BMS_to_Upmonitor.m_data[7] = (VoltInfo.CellVolt[i*3+2]>>8)&0X00FF;
      Return_Value= MSCAN2SendMsg(&BMS_to_Upmonitor);
      delay_time(100);
    }  
    switch(batt1) 
    {
      case 1:
      BMS_to_Upmonitor.m_data[0] = 0xFF;//子板备用  
      BMS_to_Upmonitor.m_data[1] = (uint8)(i); //每个6804采集电压的编号 
      BMS_to_Upmonitor.m_data[2] = (uint8)VoltInfo.CellVolt[i*3];
      BMS_to_Upmonitor.m_data[3] = (VoltInfo.CellVolt[i*3]>>8)&0X00FF;
      BMS_to_Upmonitor.m_data[4] = 0xFF;
      BMS_to_Upmonitor.m_data[5] = 0xFF;
      BMS_to_Upmonitor.m_data[6] = 0xFF;
      BMS_to_Upmonitor.m_data[7] = 0xFF;
       Return_Value= MSCAN2SendMsg(&BMS_to_Upmonitor);
      break;
      
      case 2:
      BMS_to_Upmonitor.m_data[0] = (uint8)(i/NUM_IC);
      BMS_to_Upmonitor.m_data[1] = (uint8)(i);                             //每个6804采集电压的编号 
      BMS_to_Upmonitor.m_data[2] = (uint8)VoltInfo.CellVolt[i*3];
      BMS_to_Upmonitor.m_data[3] = (VoltInfo.CellVolt[i*3]>>8)&0X00FF;
      BMS_to_Upmonitor.m_data[4] = (uint8)VoltInfo.CellVolt[i*3+1];
      BMS_to_Upmonitor.m_data[5] = (VoltInfo.CellVolt[i*3+1]>>8)&0X00FF;
      BMS_to_Upmonitor.m_data[6] = 0xFF;
      BMS_to_Upmonitor.m_data[7] = 0xFF;
       Return_Value= MSCAN2SendMsg(&BMS_to_Upmonitor);
      break;
      
      default:
      break; 
    }      
    
    BMS_to_Upmonitor.m_ID = BMS_ture_battery; //0x18FF9601      
  	BMS_to_Upmonitor.m_data[0] = NUM1_Batper_true;
    BMS_to_Upmonitor.m_data[1] = NUM2_Batper_true;
    BMS_to_Upmonitor.m_data[2] = NUM3_Batper_true;
    BMS_to_Upmonitor.m_data[3] = 0xFF;
    BMS_to_Upmonitor.m_data[4] = 0xFF;
    BMS_to_Upmonitor.m_data[5] = 0xFF;
    BMS_to_Upmonitor.m_data[6] = 0xFF;  
    BMS_to_Upmonitor.m_data[7] = 0xFF;       
    Return_Value= MSCAN2SendMsg(&BMS_to_Upmonitor);
    
    BMS_to_Upmonitor.m_ID = BMS_Send_Information2; //0x18FF9711      
  	BMS_to_Upmonitor.m_data[0] = (uint8)VoltInfo.CellVolt_Max;
    BMS_to_Upmonitor.m_data[1] = (VoltInfo.CellVolt_Max>>8)&0x00FF;
    BMS_to_Upmonitor.m_data[2] = VoltInfo.CellVolt_MaxNode;
    BMS_to_Upmonitor.m_data[3] = (uint8)VoltInfo.CellVolt_Min;
    BMS_to_Upmonitor.m_data[4] = (VoltInfo.CellVolt_Min>>8)&0x00FF;
    BMS_to_Upmonitor.m_data[5] = VoltInfo.CellVolt_MinNode;
    BMS_to_Upmonitor.m_data[6] = ToBMU_BalanceState.CSSUBalanceOn;  
    BMS_to_Upmonitor.m_data[7] = ToBMU_BalanceState.CSSUBalanceNode;       
    Return_Value= MSCAN2SendMsg(&BMS_to_Upmonitor); 
   
    BMS_to_Upmonitor.m_ID = BMS_Send_Information3; //0x18FF9801     
  	for(i=0; i< ((NUM_Tem+6)/7) ;i++)         
    {
      BMS_to_Upmonitor.m_data[0] = i;
      for(j=1; j < ((NUM_Tem+1)%7); j++) 
      {
        BMS_to_Upmonitor.m_data[j] = TempInfo.CellTemp[j-1+i*7];
      } 
      for(j=((NUM_Tem+1)%7); j<8; j++)
      {
        BMS_to_Upmonitor.m_data[j] = 0xFF;
      }
      Return_Value = MSCAN2SendMsg(&BMS_to_Upmonitor);
    }
    j=NUM_Tem%7;
    if((j!=0)&&(NUM_Tem>7))
    {
      BMS_to_Upmonitor.m_data[0] = i;
      for(i=1; i<j+1; i++)
      {
         BMS_to_Upmonitor.m_data[i] = TempInfo.CellTemp[i-1+BMS_to_Upmonitor.m_data[0]*7];
      }
      for(i=j+1; j<8; j++)
      {
         BMS_to_Upmonitor.m_data[i] = 0xFF;
      }
      Return_Value = MSCAN2SendMsg(&BMS_to_Upmonitor);
    }
  
    BMS_to_Upmonitor.m_ID = BMS_Send_Information4;//0x18FF9811
    BMS_to_Upmonitor.m_data[0] = TempInfo.CellTemp_Max ;
  	BMS_to_Upmonitor.m_data[1] = TempInfo.CellTemp_MaxNode;   
  	BMS_to_Upmonitor.m_data[2] = TempInfo.CellTemp_Min ;
  	BMS_to_Upmonitor.m_data[3] = TempInfo.CellTemp_MinNode;   
  	BMS_to_Upmonitor.m_data[4] = TempInfo.CellTemp_tatoltemp;
  	BMS_to_Upmonitor.m_data[5] = TempInfo.CellTemp_tatoltemp>>8;   
  	BMS_to_Upmonitor.m_data[6] = 0xFF;
  	BMS_to_Upmonitor.m_data[7] = 0xFF;       	 
    Return_Value= MSCAN2SendMsg(&BMS_to_Upmonitor); 

    BMS_to_Upmonitor.m_ID = BMS_Send_Information5;//导线开路0x18ff9901
    BMS_to_Upmonitor.m_data[0] = TempInfo.CellTemp_over_ic;
  	BMS_to_Upmonitor.m_data[1] = VoltInfo.Openwire_error;	
  	BMS_to_Upmonitor.m_data[2] = VoltInfo.CellVolt_Total;   
  	BMS_to_Upmonitor.m_data[3] = VoltInfo.CellVolt_Total>>8;
  	BMS_to_Upmonitor.m_data[4] = VoltInfo.CellVolt_Total>>16;
  	BMS_to_Upmonitor.m_data[5] = IsoDetect.insulation_TotalVolt;
  	BMS_to_Upmonitor.m_data[6] = IsoDetect.insulation_TotalVolt>>8;
  	BMS_to_Upmonitor.m_data[7] = IsoDetect.insulation_TotalVolt>>16;       
    Return_Value= MSCAN2SendMsg(&BMS_to_Upmonitor); 
     
    BMS_to_Upmonitor.m_ID = BMS_Send_Information6;//导线开路0x19FF9901
  	BMS_to_Upmonitor.m_dataLen = 6;
  	for(i = 0; i < NUM_IC ; i++)
  	{
      BMS_to_Upmonitor.m_data[i*2] = Openwire_flag[i];                   
    	BMS_to_Upmonitor.m_data[i*2 + 1] = (uint8)((Openwire_flag[i]>>8)&0x00FF);	       
  	}
  	Return_Value= MSCAN2SendMsg(&BMS_to_Upmonitor);
 
    Task_Flag_Counter.BMS_to_UpMonitor++;
      
}

