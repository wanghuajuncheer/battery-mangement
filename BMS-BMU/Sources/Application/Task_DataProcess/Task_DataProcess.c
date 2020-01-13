/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_DataProcess.c
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:  
========================================================================*/
#include  "includes.h"
VoltInfo_T        g_VoltInfo;
TempInfo_T        g_TempInfo;
DataColletInfo_T  g_DataColletInfo; 
OpenWireInfo_T    g_OpenWireInfo;
InsulInfo_T       g_InsulInfo;

static void DataProcess_Volt(void);
static void DataProcess_Temp(void);
static void DataProcess_OpenWire(void);

/*=======================================================================
 *函数名:      Init_TaskDataProcess(void) 
 *功能:        采集电流、电压、温度等信息
 *参数:        无       
 *返回：       无
 *说明：       计算SOC时，所用的电流值,还需要考虑霍尔传感器是否是一样的
========================================================================*/
void Init_TaskDataProcess(void)                       
{
  //电池信息变量清零
  memset(&g_VoltInfo,       0, sizeof(VoltInfo_T));        //电池电压数据清零
  memset(&g_TempInfo,       0, sizeof(TempInfo_T));        //电池温度数据清零
  memset(&g_InsulInfo,      0, sizeof(InsulInfo_T));       //绝缘信息清零
  memset(&g_OpenWireInfo,   0, sizeof(OpenWireInfo_T));    //导线开路信息清零
  memset(&g_DataColletInfo, 0, sizeof(DataColletInfo_T));  //采集数据清零
}


/*=======================================================================
 *函数名:      Task_DataProcess(void) 
 *功能:        采集电流、电压、温度等信息
 *参数:        无       
 *返回：       无
 *说明：       计算SOC时，所用的电流值,还需要考虑霍尔传感器是否是一样的
========================================================================*/
void Task_DataProcess(void)                       
{
  DataProcess_Volt();
  DataProcess_Temp();
  DataProcess_OpenWire();
  
  g_Roll_Tick.Roll_DataPro++;
}

/*=======================================================================
 *函数名:      DataProcess_Volt()  
 *功能:        采集电压信息
 *参数:        无       
 *返回：       无
 
 *说明：        
========================================================================*/
static
void DataProcess_Volt(void)
{
  uint8   i;
  
  //读取0~49节电池的电压
  for(i=0; i<NUM_Battery; i++) 
  {
    g_VoltInfo.CellVolt[i] = g_LTC6811_VoltInfo.CellVolt[i]; //主板的电压信息放前 
  }
  for(i=0; i<SYS_SERIES_YiDongLi; i++) 
  {
    g_VoltInfo.CellVolt[i+NUM_Battery] = g_FromCSSU_Volt.CellVolt[i];  
  }
  
  //读取最大电压以及所在位置
  if (g_LTC6811_VoltInfo.CellVolt_Max >= g_FromCSSU_Volt.CellVolt_Max) 
  {
     g_VoltInfo.CellVolt_Max     = g_LTC6811_VoltInfo.CellVolt_Max;      
     g_VoltInfo.CellVolt_MaxNode = g_LTC6811_VoltInfo.CellVolt_MaxNode;
  }
  else
  {
     g_VoltInfo.CellVolt_Max     = g_FromCSSU_Volt.CellVolt_Max;
     g_VoltInfo.CellVolt_MaxNode = NUM_Battery + g_FromCSSU_Volt.CellVolt_MaxNode;
  }
  //读取最小电压以及所在位置
  if (g_LTC6811_VoltInfo.CellVolt_Min <= g_FromCSSU_Volt.CellVolt_Min) 
  {
     g_VoltInfo.CellVolt_Min     = g_LTC6811_VoltInfo.CellVolt_Min;      
     g_VoltInfo.CellVolt_MinNode = g_LTC6811_VoltInfo.CellVolt_MinNode;
  }  
  else
  {
     g_VoltInfo.CellVolt_Min = g_FromCSSU_Volt.CellVolt_Min;
     g_VoltInfo.CellVolt_MinNode = NUM_Battery + g_FromCSSU_Volt.CellVolt_MinNode;
  }  
      
  g_VoltInfo.SysVolt_Total = (g_LTC6811_VoltInfo.CellVolt_Total + g_FromCSSU_Volt.CSSUVolt_Total)/2;                                 //系统总压

  g_VoltInfo.CellVolt_Diff  = g_VoltInfo.CellVolt_Max - g_VoltInfo.CellVolt_Min;
  g_VoltInfo.CellVolt_Ave   = g_VoltInfo.SysVolt_Total/SYS_SERIES_YiDongLi;
} 

/*=======================================================================
 *函数名:      DataProcess_Temp(void)  
 *功能:        采集电压信息
 *参数:        无       
 *返回：       无
 
 *说明：        
========================================================================*/
static
void DataProcess_Temp(void) 
{
  uint8   i;
  
  //读取0~24节电池温度信息
  for(i=0; i<SYS_NUMBER_MODULE_TEMP; i++)
  {
    g_TempInfo.CellTemp[i] = g_LTC6811_TempInfo.CellTemp[i];
  }
  for(i=0; i<SYS_NUMBER_MODULE_TEMP; i++)
  {
    g_TempInfo.CellTemp[i+SYS_NUMBER_MODULE_TEMP] = g_FromCSSU_Temp.CellTemp[i];
  }
  
  //读取最大温度及位置
  if (g_LTC6811_TempInfo.CellTemp_Max >= g_FromCSSU_Temp.CellTemp_Max) 
  {
    g_TempInfo.CellTemp_Max      = g_LTC6811_TempInfo.CellTemp_Max;
    g_TempInfo.CellTemp_MaxNode  = g_LTC6811_TempInfo.CellTemp_MaxNode;
  } 
  else
  {
    g_TempInfo.CellTemp_Max      = g_FromCSSU_Temp.CellTemp_Max;
    g_TempInfo.CellTemp_MaxNode  = SYS_NUMBER_MODULE_TEMP+g_FromCSSU_Temp.CellTemp_MaxNode;
  }
  
  //读取最小温度及位置
  if (g_LTC6811_TempInfo.CellTemp_Min <= g_FromCSSU_Temp.CellTemp_Min) 
  {
    g_TempInfo.CellTemp_Min      = g_LTC6811_TempInfo.CellTemp_Min;
    g_TempInfo.CellTemp_MinNode  = g_LTC6811_TempInfo.CellTemp_MinNode;
  } 
  else
  {
    g_TempInfo.CellTemp_Min      = g_FromCSSU_Temp.CellTemp_Min;
    g_TempInfo.CellTemp_MinNode  = SYS_NUMBER_MODULE_TEMP+g_FromCSSU_Temp.CellTemp_MinNode;
  }    
       
  g_TempInfo.CellTemp_Ave  = (g_LTC6811_TempInfo.CellTemp_Ave + g_FromCSSU_Temp.CellTemp_Ave)/2;                                 //系统平均温度
  g_TempInfo.CellTemp_Diff = g_TempInfo.CellTemp_Max - g_TempInfo.CellTemp_Min;
}

/*=======================================================================
 *函数名:      DataProcess_OpenWire(void)  
 *功能:        采集电压信息
 *参数:        无       
 *返回：       无
 
 *说明：        
========================================================================*/
static
void DataProcess_OpenWire(void) 
{
   g_OpenWireInfo.OpenWire_Status = g_LTC6811_OpwireInfo.OpenwireErr|\
                                    g_FromCSSU_FltData.OpenWire_Status;
}





