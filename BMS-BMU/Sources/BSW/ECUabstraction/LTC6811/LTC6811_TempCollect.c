/*=======================================================================
 *Subsystem:   裸机
 *File:        LTC6811_TempCollect.C
 *Author:      WenM
 *Description: 此源文件主要采集电池温度(利用GPIO口)和采集6811的芯片温度
 ========================================================================
 * History:    修改历史记录列表
 * 1. Date:    2017 - 11 - 10
      Author:  ZWB
      Modification:增加了SS的赋值，为了先唤醒LTC6804的SPI。
========================================================================*/
#include  "includes.h"
LTC6811_TempInfo_T g_LTC6811_TempInfo; //LTC6811温度采集全局变量(GPIO温度、芯片温度)
/*=======================================================================
 *函数名:      Task_Pack_Temp_Collect(void)
 *功能:        获取辅助寄存器GPIO
 *参数:        无       
 *返回：       无
 *说明：       收集Pack温度
========================================================================*/
void LTC6811_TempCMDSend(void)
{
   Ltc6804_clraux();                             /* 清除辅助寄存器 */ 
                                    
   LTC6804_adax();                               /* 启动GPIO ADC转换 */
   
   LTC6804_wrcfg(NUM_IC, CFG1);
}

/*=======================================================================
 *函数名:      Task_Pack_Temp_Process(void)
 *功能:        Pack温度处理函数
 *参数:        无       
 *返回：       无
 *说明：       对Pack温度进行处理
========================================================================*/
void LTC6811_TempCollect(void)        
{
   uint8  j,k;
   uint16 cell_temp[NUM_IC][6];    // 读取辅助寄存器中的Pack温度值 
   uint16 Cell_temp[NUM_IC][6];      // 温度寄存器
   uint8  PEC_error_t[NUM_IC];
   uint8  maxtemp,mintemp;           // maxtemp=0x8000,mintemp=0X7FFF 32767 = (15个1);
   int16  Temperature[NUM_IC][5];    // PACK温度 
   
   for(k = 0; k < NUM_IC; k++)
   {
      PEC_error_t[k] = 0;            // PEC位清零
   }                      
   for(k = 0;k < NUM_IC;k++)
   {
      for(j = 0;j < 6;j++)
      {
         Cell_temp[k][j] = 0;
      }
   }

   LTC6811_Wakeup();//唤醒
       
   LTC6804_rdaux(0, NUM_IC, cell_temp, PEC_error_t);
          
   for(k = 0; k < NUM_IC; k++)
   {
       if(!PEC_error_t[k])
       {
           for(j = 0; j < 6; j++)
           {
               if(cell_temp[k][j]<29152 && cell_temp[k][j]>1098) 
               Cell_temp[k][j] = cell_temp[k][j];
           }
       } 
   } 
  //第1个6811采集的温度
  Temperature[0][0] = HXYA_Gpio_Search(Cell_temp[0][1]);
  Temperature[0][1] = HXYA_Gpio_Search(Cell_temp[0][2]);
  //第2个6811采集的温度
  Temperature[1][0] = HXYA_Gpio_Search(Cell_temp[1][1]);
  Temperature[1][1] = HXYA_Gpio_Search(Cell_temp[1][2]);
  Temperature[1][2] = HXYA_Gpio_Search(Cell_temp[1][3]);
  //全局变量赋值
  g_LTC6811_TempInfo.CellTemp[0] = Temperature[0][0]+40; //偏移量为40
  g_LTC6811_TempInfo.CellTemp[1] = Temperature[0][1]+40; //偏移量为40
  g_LTC6811_TempInfo.CellTemp[2] = Temperature[1][0]+40; //偏移量为40
  g_LTC6811_TempInfo.CellTemp[3] = Temperature[1][1]+40; //偏移量为40
  g_LTC6811_TempInfo.CellTemp[4] = Temperature[1][2]+40; //偏移量为40
  
  g_LTC6811_TempInfo.CellTemp_Tatoltemp = 0;
  
  for(k = 0; k < NUM_Tem ; k++)
  {  
    g_LTC6811_TempInfo.CellTemp_Tatoltemp = g_LTC6811_TempInfo.CellTemp_Tatoltemp + g_LTC6811_TempInfo.CellTemp[k];
  }
  g_LTC6811_TempInfo.CellTemp_Ave = g_LTC6811_TempInfo.CellTemp_Tatoltemp/NUM_Tem;
  
  maxtemp=mintemp=g_LTC6811_TempInfo.CellTemp[0];
  g_LTC6811_TempInfo.CellTemp_MaxNode = g_LTC6811_TempInfo.CellTemp_MinNode=0;
  
  for(k = 0; k <NUM_Tem; k++) 
  {
    if(maxtemp < g_LTC6811_TempInfo.CellTemp[k]) 
    {
      maxtemp=g_LTC6811_TempInfo.CellTemp[k];
      g_LTC6811_TempInfo.CellTemp_MaxNode=k;
    }
    if(mintemp > g_LTC6811_TempInfo.CellTemp[k]) 
    {
      mintemp=g_LTC6811_TempInfo.CellTemp[k];
      g_LTC6811_TempInfo.CellTemp_MinNode=k; 
    }
  }  
  g_LTC6811_TempInfo.CellTemp_Max=maxtemp;
  g_LTC6811_TempInfo.CellTemp_Min=mintemp;
}
/*=======================================================================
 *函数名:      Task_Chip_Temp_Collect(void)
 *功能:        对芯片温度采集
 *参数:        无       
 *返回：       无
 *说明：       对状态寄存器采集
========================================================================*/
void LTC6811_ChipTempCMDSend(void)
{                                                             
    Ltc6804_Clrstat();                                               /* 对辅助寄存器清零; */
    
    LTC6804_adstat();                                                /* 启动对状态寄存器的数据收集;*/ 
}    

/*=======================================================================
 *函数名:      Task_Chip_Temp_Process(void)
 *功能:        对芯片温度处理
 *参数:        无       
 *返回：       无
 *说明：       
========================================================================*/
void LTC6811_ChipTempCollect(void)                         
{ 
   uint8  i=0;
   uint16 cell_state[NUM_IC][6];
   static uint16 voltage_temp[NUM_IC];
   float  Temp_IC[NUM_IC];                                       
   uint8  PEC_error_s[NUM_IC];
   uint8  maxtemp, mintemp;
    
    for(i = 0;i < NUM_IC;i++)
    {                                                                                                                                                                                                                                                                                          
        PEC_error_s[i]=0;                
    } 
    
    //SS2 = 0;
    //SS2 = 1;
    LTC6811_Wakeup();//唤醒
        
    LTC6804_rdstat(0, NUM_IC, cell_state, PEC_error_s); // 只需要状态寄存器组A
    
    for(i = 0; i < NUM_IC; i++)
    {
        if( PEC_error_s[i] ==0 ) 
        {
            //if( cell_state[i][1] < 31000 ) 
            {  
                voltage_temp[i] = cell_state[i][1]; 
            }
        }
    }
    
    for(i = 0; i < NUM_IC; i++)
    {
        Temp_IC[i] = (voltage_temp[i] + 37) / 75.0 - 273.0;                   
    }  
    
    for(i = 0; i < NUM_IC; i++)
    {
       if ( Temp_IC[i] >= -40 && Temp_IC[i] <=120 ) 
       {
          g_LTC6811_TempInfo.ICTemp[i] = (uint8)(Temp_IC[i]+40);  
       }
    }  
    
    maxtemp = mintemp = g_LTC6811_TempInfo.ICTemp[0];
    for(i = 0; i< NUM_IC; i++)
    {
        if(maxtemp < g_LTC6811_TempInfo.ICTemp[i])
        {
           maxtemp = g_LTC6811_TempInfo.ICTemp[i]; 
        }
    }
    
    if( maxtemp > Temp_IC_over+40)
    {
        g_LTC6811_TempInfo.ICTemp_OverState = 0x01;
    }
    else
    {
        g_LTC6811_TempInfo.ICTemp_OverState = 0x00;
    }
}
