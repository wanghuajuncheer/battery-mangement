/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_UpMonitor.C
 *Author:      Wen Yuhao
 *Description: 通信：
               接口：
               波特率：
 ========================================================================
 * History:    修改历史记录列表
 * 1. Date:
      Author:
      Modification:
========================================================================*/
#include  "includes.h"

BMSMonitor_Volt_T g_BMSMonitor_Volt;
BMSMonitor_Temp_T g_BMSMonitor_Temp;
BMSMonitor_Curr_T g_BMSMonitor_Curr;
BMSMonitor_Insu_T g_BMSMonitor_Insul; 
BMSMonitor_SOH_T  g_BMSMonitor_SOH;
BMSMonitor_New_LT_T g_BMSMonitor_New_LT;

/*=======================================================================
 *函数名:      CAN_UpMonitorInit(void) 
 *功能:        上位机使用的CAN通道初始化
 *参数:        无       
 *返回：       无
 
 *说明：       
========================================================================*/ 
uint8 CAN_UpMonitorInit(void)
{
  uint8 state;
  state = CAN2_Init(500);
  return(state);
}

/*=======================================================================
 *函数名:      CAN_ToUpMonitor(pCANFRAME canptr) 
 *功能:        BMS information to UpMonitor
 *参数:        无       
 *返回：       无
 *说明：       BMS发送信息给上位机的CAN通道选择；
========================================================================*/ 
uint8 CAN_ToUpMonitor(pCANFRAME canptr)
{
  if(!CAN2_SendMsg(canptr))
  {
     return 0;//正常 
  }  
  return 1;  //CAN故障 */
}


/*=======================================================================
 *函数名:      Init_UpMonitor(void) 
 *功能:        初始化上位机
 *参数:        无       
 *返回：       无
 *说明：       
 ========================================================================*/ 
void Init_UpMonitor(void)
{
  memset(&g_BMSMonitor_Volt, 0, sizeof(BMSMonitor_Volt_T));   //常温电压阈值
  memset(&g_BMSMonitor_Temp, 0, sizeof(BMSMonitor_Temp_T));   //温度阈值
  memset(&g_BMSMonitor_Curr, 0, sizeof(BMSMonitor_Curr_T));   //电流阈值
  memset(&g_BMSMonitor_Insul, 0, sizeof(BMSMonitor_Insu_T)); //绝缘阈值
  memset(&g_BMSMonitor_New_LT, 0, sizeof(BMSMonitor_New_LT_T)); //低温电压阈值

  memset(&g_MonitorBMS_Start, 0, sizeof(MonitorBMS_Start_T)); //下标启动标记

}

/*=======================================================================
 *函数名:      UpMonitor_DelayTimeus(uint16 us)
 *功能:        
 *参数:              
 *返回：       
 *说明：       
========================================================================*/
static
void UpMonitor_DelayTimeus(uint16 us) 
{
	  uint16 delayval;
	  delayval = us * 9;
	  while(delayval--); 
}


/*=======================================================================
 *函数名:      Task_BmsToUpMonitor(void) 
 *功能:        BMS information to UpMonitor
 *参数:        无       
 *返回：       无
 *说明：       BMS发送信息给上位机；
========================================================================*/  
static
void CAN_ToUpMonitorMsg(void)
{  
  uint8 i;
  
  uint8 Positive;
  CANFRAME BMS_to_Upmonitor;
  
  BMS_to_Upmonitor.m_IDE = 1;
	BMS_to_Upmonitor.m_RTR = 0;
	BMS_to_Upmonitor.m_dataLen = 8;
	BMS_to_Upmonitor.m_priority = 6; 
  
  if(g_MonitorBMS_Start.Threshold_StarUpload == 1)
  {
    //电池类信息
    //0x1800C0F4
    for(i=0;i<3;i++)
    { 
      BMS_to_Upmonitor.m_ID = 0x1800C0F4;     	
      switch(i)
      {
        case 0:
        	BMS_to_Upmonitor.m_data[0] = i;     //编号 0x00
        	BMS_to_Upmonitor.m_data[1] = CELL_TYPE;                                //电池类型 
        	BMS_to_Upmonitor.m_data[2] = (uint8)(CELL_RESIS_INTERNAL*10);          //电池内阻，单位是0.1mΩ/Bit  
        	BMS_to_Upmonitor.m_data[3] = ((uint16)(CELL_RESIS_INTERNAL*10)) >> 8; 
        	BMS_to_Upmonitor.m_data[4] = (uint8)(SYS_CAPACITY*100);               //电池容量，单位是0.01AH/Bit
        	BMS_to_Upmonitor.m_data[5] = ((uint16)(SYS_CAPACITY*100)) >> 8;
        	BMS_to_Upmonitor.m_data[6] = (uint8)CELL_LIFE_CYCLE;                   //循环寿命，1次/Bit
        	BMS_to_Upmonitor.m_data[7] = ((uint16)(CELL_LIFE_CYCLE)) >> 8;       
          while(CAN_ToUpMonitor(&BMS_to_Upmonitor));  
          UpMonitor_DelayTimeus(20);
        break;
        
        case 1:
          BMS_to_Upmonitor.m_data[0] = i;     //编号 0x01
          BMS_to_Upmonitor.m_data[1] = (uint8)(CELL_VOLT_NOMINAL*10000);     //单体标称电压，单位是0.0001V/Bit  
          BMS_to_Upmonitor.m_data[2] = ((uint16)(CELL_VOLT_NOMINAL*10000)) >> 8;          
          BMS_to_Upmonitor.m_data[3] = (uint8)(CELL_VOLT_MAX*10000);    //最高单体电压，单位是0.0001V/Bi
          BMS_to_Upmonitor.m_data[4] = ((uint16)(CELL_VOLT_MAX*10000)) >> 8;             
          BMS_to_Upmonitor.m_data[5] = (uint8)(CELL_VOLT_MIN*10000);        //最低单体电压，单位是0.0001V/Bit
          BMS_to_Upmonitor.m_data[6] = ((uint16)(CELL_VOLT_MIN*10000)) >> 8;                  
          BMS_to_Upmonitor.m_data[7] = CELL_TEMP_MAX_DISCHARGE;   //电池最高保护温度
          while(CAN_ToUpMonitor(&BMS_to_Upmonitor));  
          UpMonitor_DelayTimeus(20);
        break;
        
        case 2:
          BMS_to_Upmonitor.m_data[0] = i;     //编号 0x02
          BMS_to_Upmonitor.m_data[1] = (uint8)(F1_DISCHG_VOLTCD);          //最大单体压差 
          BMS_to_Upmonitor.m_data[2] = (uint16)(F1_DISCHG_VOLTCD) >> 8;            
          BMS_to_Upmonitor.m_data[3] = (uint8)F2_DISCHG_TEMPL;             //电池最低使用温度
          BMS_to_Upmonitor.m_data[4] = (uint8)SYS_SERIES_YiDongLi;         //电池系统总串数   
          BMS_to_Upmonitor.m_data[5] = (uint8)SYS_NUMBER_MODULE;           //电池系统模组数
          BMS_to_Upmonitor.m_data[6] = (uint8)SYS_NUMBER_BOX;              //电池系统箱数
          BMS_to_Upmonitor.m_data[7] = (uint8)SYS_PARALLEL;                //电池系统的总并数
          while(CAN_ToUpMonitor(&BMS_to_Upmonitor));  
          UpMonitor_DelayTimeus(20);
        break;
      }
    }

    //电池故障等级
    //0x1810C0F4      电池电压保护阈值信息
    for(i=0;i<6;i++)
    {
      BMS_to_Upmonitor.m_ID = 0x1810C0F4;
      switch(i)
      {
        //单体过压
        case 0 :
        	BMS_to_Upmonitor.m_data[0] = i;     //编号 0x00
        	BMS_to_Upmonitor.m_data[1] = (uint8)(g_BMSMonitor_Volt.Volt_Cell_High1);         //单体电压过高阈值1级 
        	BMS_to_Upmonitor.m_data[2] = ((uint16)(g_BMSMonitor_Volt.Volt_Cell_High1)) >> 8;             
        	BMS_to_Upmonitor.m_data[3] = (uint8)(g_BMSMonitor_Volt.Volt_Cell_High2);         //单体电压过高阈值2级
        	BMS_to_Upmonitor.m_data[4] = ((uint16)(g_BMSMonitor_Volt.Volt_Cell_High2)) >> 8;       
        	BMS_to_Upmonitor.m_data[5] = 0xFF;//(uint8)(g_BMSMonitor_Volt.Single_Over3_Volt);         //单体电压过高阈值3级
        	BMS_to_Upmonitor.m_data[6] = 0xFF;//((uint16)(g_BMSMonitor_Volt.Single_Over3_Volt)) >> 8;       
        	BMS_to_Upmonitor.m_data[7] = 0xFF;       
          while(CAN_ToUpMonitor(&BMS_to_Upmonitor));  
          UpMonitor_DelayTimeus(20); 
        break;
        //单体欠压   
        case 1:
          BMS_to_Upmonitor.m_data[0] = i;     //编号 0x01
         	BMS_to_Upmonitor.m_data[1] = (uint8)(g_BMSMonitor_Volt.Volt_Cell_Low1);         //单体电压过低阈值1级 
        	BMS_to_Upmonitor.m_data[2] = ((uint16)(g_BMSMonitor_Volt.Volt_Cell_Low1)) >> 8;             
        	BMS_to_Upmonitor.m_data[3] = (uint8)(g_BMSMonitor_Volt.Volt_Cell_Low2);         //单体电压过低阈值2级
        	BMS_to_Upmonitor.m_data[4] = ((uint16)(g_BMSMonitor_Volt.Volt_Cell_Low2)) >> 8;       
        	BMS_to_Upmonitor.m_data[5] = 0xFF;//(uint8)(g_BMSMonitor_Volt.Single_Under3_Volt);         //单体电压过低阈值3级
        	BMS_to_Upmonitor.m_data[6] = 0xFF;//((uint16)(g_BMSMonitor_Volt.Single_Under3_Volt)) >> 8;                  
          BMS_to_Upmonitor.m_data[7] = 0xFF;                               
          while(CAN_ToUpMonitor(&BMS_to_Upmonitor));   
          UpMonitor_DelayTimeus(20);
        break;
        //系统过压  
        case 2:
          BMS_to_Upmonitor.m_data[0] = i;     //编号 0x02
        	BMS_to_Upmonitor.m_data[1] = (uint8)(g_BMSMonitor_Volt.Volt_Sys_High1);         //系统电压过高阈值1级 
        	BMS_to_Upmonitor.m_data[2] = ((uint16)(g_BMSMonitor_Volt.Volt_Sys_High1)) >> 8;             
        	BMS_to_Upmonitor.m_data[3] = (uint8)(g_BMSMonitor_Volt.Volt_Sys_High2);         //系统电压过高阈值2级
        	BMS_to_Upmonitor.m_data[4] = ((uint16)(g_BMSMonitor_Volt.Volt_Sys_High2)) >> 8;       
        	BMS_to_Upmonitor.m_data[5] = 0xFF;//(uint8)(Monitor_BMS_Volt.Pack_Over3_Volt);         //系统电压过高阈值3级
        	BMS_to_Upmonitor.m_data[6] = 0xFF;//((uint16)(Monitor_BMS_Volt.Pack_Over3_Volt)) >> 8;       
        	BMS_to_Upmonitor.m_data[7] = 0xFF;
          while(CAN_ToUpMonitor(&BMS_to_Upmonitor));  
          UpMonitor_DelayTimeus(20);
        break;
        //系统欠压
        case 3:
          BMS_to_Upmonitor.m_data[0] = i;     //编号 0x03
        	BMS_to_Upmonitor.m_data[1] = (uint8)(g_BMSMonitor_Volt.Volt_Sys_Low1);         //系统电压过低阈值1级 
        	BMS_to_Upmonitor.m_data[2] = ((uint16)(g_BMSMonitor_Volt.Volt_Sys_Low1)) >> 8;             
        	BMS_to_Upmonitor.m_data[3] = (uint8)(g_BMSMonitor_Volt.Volt_Sys_Low2);         //系统电压过低阈值2级
        	BMS_to_Upmonitor.m_data[4] = ((uint16)(g_BMSMonitor_Volt.Volt_Sys_Low2)) >> 8;       
        	BMS_to_Upmonitor.m_data[5] = 0xFF;//(uint8)(Monitor_BMS_Volt.Pack_Under3_Volt);         //系统电压过低阈值3级
        	BMS_to_Upmonitor.m_data[6] = 0xFF;//((uint16)(Monitor_BMS_Volt.Pack_Under3_Volt)) >> 8;       
        	BMS_to_Upmonitor.m_data[7] = 0xFF;
          while(CAN_ToUpMonitor(&BMS_to_Upmonitor)); 
          UpMonitor_DelayTimeus(20);
        break;
        //单体压差
        case 4:
          BMS_to_Upmonitor.m_data[0] = i;     //编号 0x04
        	BMS_to_Upmonitor.m_data[1] = (uint8)(g_BMSMonitor_Volt.Volt_Cell_Diff1);         //单体电压压差阈值1级 
        	BMS_to_Upmonitor.m_data[2] = ((uint16)(g_BMSMonitor_Volt.Volt_Cell_Diff1)) >> 8;             
        	BMS_to_Upmonitor.m_data[3] = 0xFF;//(uint8)(Monitor_BMS_Volt.Single_Diff2_Volt);         //单体电压压差阈值2级
        	BMS_to_Upmonitor.m_data[4] = 0xFF;//((uint16)(Monitor_BMS_Volt.Single_Diff2_Volt)) >> 8;       
        	BMS_to_Upmonitor.m_data[5] = 0xFF;//(uint8)(Monitor_BMS_Volt.Single_Diff3_Volt);         //单体电压压差阈值3级
        	BMS_to_Upmonitor.m_data[6] = 0xFF;//((uint16)(Monitor_BMS_Volt.Single_Diff3_Volt)) >> 8;       
        	BMS_to_Upmonitor.m_data[7] = 0xFF;
          while(CAN_ToUpMonitor(&BMS_to_Upmonitor));  
          UpMonitor_DelayTimeus(20);
        break; 
        //系统压差
        case 5:
          BMS_to_Upmonitor.m_data[0] = i;     //编号 0x05
        	BMS_to_Upmonitor.m_data[1] = 0xFF;//(uint8)(Monitor_BMS_Volt.System_Diff1_Volt);         //系统电压压差阈值1级 
        	BMS_to_Upmonitor.m_data[2] = 0xFF;//((uint16)(Monitor_BMS_Volt.System_Diff1_Volt)) >> 8;             
        	BMS_to_Upmonitor.m_data[3] = 0xFF;//(uint8)(Monitor_BMS_Volt.System_Diff2_Volt);         //系统电压压差阈值2级
        	BMS_to_Upmonitor.m_data[4] = 0xFF;//((uint16)(Monitor_BMS_Volt.System_Diff2_Volt)) >> 8;       
        	BMS_to_Upmonitor.m_data[5] = 0xFF;//(uint8)(Monitor_BMS_Volt.System_Diff3_Volt);         //系统电压压差阈值3级
        	BMS_to_Upmonitor.m_data[6] = 0xFF;//((uint16)(Monitor_BMS_Volt.System_Diff3_Volt)) >> 8;       
        	BMS_to_Upmonitor.m_data[7] = 0xFF;
          while(CAN_ToUpMonitor(&BMS_to_Upmonitor));   
          UpMonitor_DelayTimeus(20);
        break; 
      }
    }

    //0x1811C0F4      电池温度保护阈值信息
    for(i=0;i<4;i++)
    {
      BMS_to_Upmonitor.m_ID = 0x1811C0F4;
      switch(i)
      {
        //充电电芯温度 
        case 0:
        	BMS_to_Upmonitor.m_data[0] = i;     //编号 0x00
        	BMS_to_Upmonitor.m_data[1] = (uint8)(g_BMSMonitor_Temp.Temp_Charge_High1);         //充电电芯温度过高 
        	BMS_to_Upmonitor.m_data[2] = (uint8)(g_BMSMonitor_Temp.Temp_Charge_High2);              
        	BMS_to_Upmonitor.m_data[3] = 0xFF;//(uint8)(Monitor_BMS_Temp.Charge_Over3_Temp);         
        	BMS_to_Upmonitor.m_data[4] = (uint8)(g_BMSMonitor_Temp.Temp_Charge_Low1);        //充电电芯温度过低 
        	BMS_to_Upmonitor.m_data[5] = (uint8)(g_BMSMonitor_Temp.Temp_Charge_Low2);         
        	BMS_to_Upmonitor.m_data[6] = 0xFF;//(uint8)(Monitor_BMS_Temp.Charge_Under3_Temp);        
        	BMS_to_Upmonitor.m_data[7] = 0xFF;       
          while(CAN_ToUpMonitor(&BMS_to_Upmonitor));   
          UpMonitor_DelayTimeus(20); 
        break;
        //放电电芯温度   
        case 1:
          BMS_to_Upmonitor.m_data[0] = i;     //编号 0x01
         	BMS_to_Upmonitor.m_data[1] = (uint8)(g_BMSMonitor_Temp.Temp_DisCharge_High1);         //放电电芯温度过高 
        	BMS_to_Upmonitor.m_data[2] = (uint8)(g_BMSMonitor_Temp.Temp_DisCharge_High2);             
        	BMS_to_Upmonitor.m_data[3] = 0xFF;//(uint8)(Monitor_BMS_Temp.DisCharge_Over3_Temp);         
        	BMS_to_Upmonitor.m_data[4] = (uint8)(g_BMSMonitor_Temp.Temp_DisCharge_Low1);        //放电电芯温度过低 
        	BMS_to_Upmonitor.m_data[5] = (uint8)(g_BMSMonitor_Temp.Temp_DisCharge_Low2);         
        	BMS_to_Upmonitor.m_data[6] = 0xFF;//(uint8)(Monitor_BMS_Temp.DisCharge_Under3_Temp);                  
          BMS_to_Upmonitor.m_data[7] = 0xFF;                               
          while(CAN_ToUpMonitor(&BMS_to_Upmonitor));    
          UpMonitor_DelayTimeus(20);
        break;
        //电池温差   
        case 2:
          BMS_to_Upmonitor.m_data[0] = i;     //编号 0x02
        	BMS_to_Upmonitor.m_data[1] = (uint8)(g_BMSMonitor_Temp.Temp_Charge_Diff1);         //充电电池温差过大
        	BMS_to_Upmonitor.m_data[2] = 0xFF;//(uint8)(Monitor_BMS_Temp.Charge_Diff2_Temp);             
        	BMS_to_Upmonitor.m_data[3] = 0xFF;//(uint8)(Monitor_BMS_Temp.Charge_Diff3_Temp);         
        	BMS_to_Upmonitor.m_data[4] = (uint8)(g_BMSMonitor_Temp.Temp_DisCharge_Diff1);      //放电电池温差过大 
        	BMS_to_Upmonitor.m_data[5] = 0xFF;//(uint8)(Monitor_BMS_Temp.DisCharge_Diff2_Temp);       
        	BMS_to_Upmonitor.m_data[6] = 0xFF;//(uint8)(Monitor_BMS_Temp.DisCharge_Diff3_Temp);       
        	BMS_to_Upmonitor.m_data[7] = 0xFF;
          while(CAN_ToUpMonitor(&BMS_to_Upmonitor));     
          UpMonitor_DelayTimeus(20);
        break;
        //充电枪&BMS过温阈值
        case 3:
          BMS_to_Upmonitor.m_data[0] = i;     //编号 0x03
        	BMS_to_Upmonitor.m_data[1] = 0xFF;//(uint8)(Monitor_BMS_Temp.ChargeGun_Over1_Temp);         //充电枪过温
        	BMS_to_Upmonitor.m_data[2] = 0xFF;//(uint8)(Monitor_BMS_Temp.ChargeGun_Over2_Temp);             
        	BMS_to_Upmonitor.m_data[3] = 0xFF;//(uint8)(Monitor_BMS_Temp.ChargeGun_Over3_Temp);         
        	BMS_to_Upmonitor.m_data[4] = 0xFF;//(uint8)(Monitor_BMS_Temp.Chip_Over1_Temp);              //BMS过温
        	BMS_to_Upmonitor.m_data[5] = 0xFF;//(uint8)(Monitor_BMS_Temp.Chip_Over2_Temp);         
        	BMS_to_Upmonitor.m_data[6] = 0xFF;//(uint8)(Monitor_BMS_Temp.Chip_Over3_Temp);       
        	BMS_to_Upmonitor.m_data[7] = 0xFF;
          while(CAN_ToUpMonitor(&BMS_to_Upmonitor));      
          UpMonitor_DelayTimeus(20);
        break;
      }
    }

    //0x1812C0F4      电池电流阈值信息
    for(i=0;i<3;i++)
    {
      BMS_to_Upmonitor.m_ID = 0x1812C0F4;
      switch(i)
      {
        //系统放电过流 
        case 0:
        	BMS_to_Upmonitor.m_data[0] = i;     //编号 0x00
        	BMS_to_Upmonitor.m_data[1] = (uint8)(g_BMSMonitor_Curr.Current_DisCharge_High1);          
        	BMS_to_Upmonitor.m_data[2] = ((uint16)(g_BMSMonitor_Curr.Current_DisCharge_High1)) >> 8;              
        	BMS_to_Upmonitor.m_data[3] = (uint8)(g_BMSMonitor_Curr.Current_DisCharge_High2);         
        	BMS_to_Upmonitor.m_data[4] = ((uint16)(g_BMSMonitor_Curr.Current_DisCharge_High2)) >> 8;       
        	BMS_to_Upmonitor.m_data[5] = 0xFF;//(uint8)(Monitor_BMS_Current.DisCharge_Over3_Current);         
        	BMS_to_Upmonitor.m_data[6] = 0xFF;//((uint16)(Monitor_BMS_Current.DisCharge_Over3_Current)) >> 8;       
        	BMS_to_Upmonitor.m_data[7] = 0xFF;       
          while(CAN_ToUpMonitor(&BMS_to_Upmonitor));     
          UpMonitor_DelayTimeus(20); 
        break;
        //系统充电过流 
        case 1:
          BMS_to_Upmonitor.m_data[0] = i;     //编号 0x01
         	BMS_to_Upmonitor.m_data[1] = (uint8)(g_BMSMonitor_Curr.Current_Charge_High1);         
        	BMS_to_Upmonitor.m_data[2] = ((uint16)(g_BMSMonitor_Curr.Current_Charge_High1)) >> 8;             
        	BMS_to_Upmonitor.m_data[3] = (uint8)(g_BMSMonitor_Curr.Current_Charge_High2);         
        	BMS_to_Upmonitor.m_data[4] = ((uint16)(g_BMSMonitor_Curr.Current_Charge_High2)) >> 8;         
        	BMS_to_Upmonitor.m_data[5] = 0xFF;//(uint8)(Monitor_BMS_Current.Charge_Over3_Current);         
        	BMS_to_Upmonitor.m_data[6] = 0xFF;//((uint16)(Monitor_BMS_Current.Charge_Over3_Current)) >> 8;                  
          BMS_to_Upmonitor.m_data[7] = 0xFF;                               
          while(CAN_ToUpMonitor(&BMS_to_Upmonitor));      
          UpMonitor_DelayTimeus(20);
        break;
        //SOC阈值
        case 2:
          BMS_to_Upmonitor.m_data[0] = i;     //编号 0x02
        	BMS_to_Upmonitor.m_data[1] = 0xFF;//(uint8)(Monitor_BMS_Current.SOC_Low1);         //低SOC
        	BMS_to_Upmonitor.m_data[2] = 0xFF;//(uint8)(Monitor_BMS_Current.SOC_Low2);             
        	BMS_to_Upmonitor.m_data[3] = 0xFF;//(uint8)(Monitor_BMS_Current.SOC_Low3);         
        	BMS_to_Upmonitor.m_data[4] = 0xFF;//(uint8)(Monitor_BMS_Current.SOC_High1);        //高SOC
        	BMS_to_Upmonitor.m_data[5] = 0xFF;//(uint8)(Monitor_BMS_Current.SOC_High2);         
        	BMS_to_Upmonitor.m_data[6] = 0xFF;//(uint8)(Monitor_BMS_Current.SOC_High3);       
        	BMS_to_Upmonitor.m_data[7] = 0xFF;
          while(CAN_ToUpMonitor(&BMS_to_Upmonitor));        
          UpMonitor_DelayTimeus(20);
        break;
      }
    }

    //0x1813C0F4     绝缘故障阈值信息
    BMS_to_Upmonitor.m_ID = 0x1813C0F4;     	
    BMS_to_Upmonitor.m_data[0] = 0x00;     //编号  0x00
    BMS_to_Upmonitor.m_data[1] = 0xFF;//(uint8)(Monitor_BMS_Insulation.Insulation_Resis1);         
    BMS_to_Upmonitor.m_data[2] = 0xFF;//((uint16)(Monitor_BMS_Insulation.Insulation_Resis1)) >> 8;             
    BMS_to_Upmonitor.m_data[3] = (uint8)(g_BMSMonitor_Insul.Insulation_Resis2);         
    BMS_to_Upmonitor.m_data[4] = ((uint16)(g_BMSMonitor_Insul.Insulation_Resis2)) >> 8;         
    BMS_to_Upmonitor.m_data[5] = 0xFF;//(uint8)(Monitor_BMS_Insulation.Insulation_Resis3);         
    BMS_to_Upmonitor.m_data[6] = 0xFF;//((uint16)(Monitor_BMS_Insulation.Insulation_Resis3)) >> 8;                  
    BMS_to_Upmonitor.m_data[7] = 0xFF;                               
    while(CAN_ToUpMonitor(&BMS_to_Upmonitor));      
    UpMonitor_DelayTimeus(20);

    g_MonitorBMS_Start.Threshold_StarUpload = 0x00;
  }
  
  
  //实时更新数据信息 
  //0x1820C0F4    电池系统自检及时间信息
  for(i=0;i<3;i++)
  {
    BMS_to_Upmonitor.m_ID = 0x1820C0F4;
    switch(i)
    {
      case 0 :     //系统实时时间
      	BMS_to_Upmonitor.m_data[0] = 0;     //编号 0x00
      	BMS_to_Upmonitor.m_data[1] = Read_IIC_Time.IIC_Read_Year;     //系统实时时间:年 
      	BMS_to_Upmonitor.m_data[2] = Read_IIC_Time.IIC_Read_Month;    //系统实时时间:月         
      	BMS_to_Upmonitor.m_data[3] = Read_IIC_Time.IIC_Read_Day;      //系统实时时间:日
      	BMS_to_Upmonitor.m_data[4] = Read_IIC_Time.IIC_Read_Hour;     //系统实时时间:时  
      	BMS_to_Upmonitor.m_data[5] = Read_IIC_Time.IIC_Read_Minute;   //系统实时时间:分
      	BMS_to_Upmonitor.m_data[6] = 0xFF;
      	BMS_to_Upmonitor.m_data[7] = 0xFF;       
        while(CAN_ToUpMonitor(&BMS_to_Upmonitor));
        UpMonitor_DelayTimeus(20);
      break;
      
      case 1:    //系统运行状态及时长
        BMS_to_Upmonitor.m_data[0] = 1;     //编号 0x01
        BMS_to_Upmonitor.m_data[1] = g_WorkStateJudge.WorkState;    //系统运行状态：00 放电，01 快充，02慢充  
        BMS_to_Upmonitor.m_data[2] = (uint8)(g_SysTime.BMS_TotalRun_MiniteTime/60);    //BMS系统运行时间（小时）       
        BMS_to_Upmonitor.m_data[3] = (uint16)(g_SysTime.BMS_TotalRun_MiniteTime/60) >> 8;    
        BMS_to_Upmonitor.m_data[4] = 0xFF;    //BMS系统单次运行时长
        BMS_to_Upmonitor.m_data[5] = 0xFF;        
        BMS_to_Upmonitor.m_data[6] = 0xFF;    //BMS系统上次运行时长              
        BMS_to_Upmonitor.m_data[7] = 0xFF;   
        while(CAN_ToUpMonitor(&BMS_to_Upmonitor));
        UpMonitor_DelayTimeus(20);
      break;
      
      case 2:      //电池自检状态及自检失败原因
      
        BMS_to_Upmonitor.m_data[0] = 2;     //编号 0x02
        BMS_to_Upmonitor.m_data[1] = 0x01;   //电池自检状态  00 自检中，01 成功，02 失败
        BMS_to_Upmonitor.m_data[2] = 0x00;       
        BMS_to_Upmonitor.m_data[3] = 0x00;    
        BMS_to_Upmonitor.m_data[4] = 0x00;    
        BMS_to_Upmonitor.m_data[5] = 0x00;        
        BMS_to_Upmonitor.m_data[6] = 0x00;                 
        BMS_to_Upmonitor.m_data[7] = 0x00; 
        while(CAN_ToUpMonitor(&BMS_to_Upmonitor));
        UpMonitor_DelayTimeus(20);
      break;
    }
  }
  
  //0x1830C0F4    电池系统总压信息
  for(i=0;i<2;i++)
  {
    BMS_to_Upmonitor.m_ID = 0x1830C0F4; 
    switch(i)
    {
      case 0:    //电池总压及压差
      	BMS_to_Upmonitor.m_data[0] = i;     //编号 0x00
      	BMS_to_Upmonitor.m_data[1] = (uint8)(g_VoltInfo.SysVolt_Total*0.001);     //电池总压（单体累加）   0.1V分辨率
      	BMS_to_Upmonitor.m_data[2] = ((uint16)(g_VoltInfo.SysVolt_Total*0.001))>>8;             
      	BMS_to_Upmonitor.m_data[3] = 0xFF; //(uint8)(SOC_DATA.BMSU_Volt*10);     //电池总压（总压模块总压）  0.1V分辨率
      	BMS_to_Upmonitor.m_data[4] = 0xFF; //((uint16)(SOC_DATA.BMSU_Volt*10)) >> 8;        
      	BMS_to_Upmonitor.m_data[5] = 0xFF; //(uint8)(abs(g_TempInfo.TotalVolt_V-SOC_DATA.BMSU_Volt)*10);  //压差（abs(累加电压-测试电压)）    0.1V分辨率
      	BMS_to_Upmonitor.m_data[6] = 0xFF; //((uint16)(abs(g_TempInfo.TotalVolt_V-SOC_DATA.BMSU_Volt)*10)) >> 8;
      	BMS_to_Upmonitor.m_data[7] = 0xFF;       
        while(CAN_ToUpMonitor(&BMS_to_Upmonitor));
        UpMonitor_DelayTimeus(20);
      break;
      
      case 1:    //单体最高/最低电压及压差
        BMS_to_Upmonitor.m_data[0] = i;     //编号 0x01
        BMS_to_Upmonitor.m_data[1] = (uint8)(g_VoltInfo.CellVolt_Max);          //单体最高电压（分辨率：0.0001V）
        BMS_to_Upmonitor.m_data[2] = ((uint16)(g_VoltInfo.CellVolt_Max)) >> 8;   
        BMS_to_Upmonitor.m_data[3] = (uint8)(g_VoltInfo.CellVolt_Min);          //单体最低电压（分辨率：0.0001V)   
        BMS_to_Upmonitor.m_data[4] = ((uint16)(g_VoltInfo.CellVolt_Min)) >> 8;
        BMS_to_Upmonitor.m_data[5] = (uint8)(g_VoltInfo.CellVolt_Diff);                //单体压差（分辨率：0.0001V) 
        BMS_to_Upmonitor.m_data[6] = ((uint16)(g_VoltInfo.CellVolt_Diff)) >> 8;             
        BMS_to_Upmonitor.m_data[7] = 0xFF;   
        while(CAN_ToUpMonitor(&BMS_to_Upmonitor));
        UpMonitor_DelayTimeus(20);
      break;
    }
  }
   
  //0x1840C0F4    电池系统温度信息
  for(i=0;i<2;i++)
  {
    BMS_to_Upmonitor.m_ID = 0x1840C0F4;
    switch(i)
    {
      case 0:     //单体最高/最低温度及温差
      	BMS_to_Upmonitor.m_data[0] = i;     //编号 0x00
      	BMS_to_Upmonitor.m_data[1] = (uint8)(g_TempInfo.CellTemp_Max);     //单体电池最高温度   1℃分辨率
      	BMS_to_Upmonitor.m_data[2] = (uint8)(g_TempInfo.CellTemp_Min);     //单体电池最低温度   1℃分辨率             
      	BMS_to_Upmonitor.m_data[3] = (uint8)(g_TempInfo.CellTemp_Diff + 40);    //单体电池温差  1℃分辨率
      	BMS_to_Upmonitor.m_data[4] = 0xFF;        
      	BMS_to_Upmonitor.m_data[5] = 0xFF;  
      	BMS_to_Upmonitor.m_data[6] = 0xFF;
      	BMS_to_Upmonitor.m_data[7] = 0xFF;       
        while(CAN_ToUpMonitor(&BMS_to_Upmonitor));
        UpMonitor_DelayTimeus(20);
      break;
    }
  }
  
  //0x1850C0F4    电池系统电流信息
  BMS_to_Upmonitor.m_ID = 0x1850C0F4;     	
	BMS_to_Upmonitor.m_data[0] = (uint8)((g_DataColletInfo.DataCollet_Current_Filter + 750)*10);        //电流测量值   0.1A分辨率  偏移量：-750
	BMS_to_Upmonitor.m_data[1] = ((uint16)((g_DataColletInfo.DataCollet_Current_Filter + 750)*10))>>8;
	BMS_to_Upmonitor.m_data[2] = (uint8)((CurrLimit.Curr_Charge_Cons + 750)*10);     //充电电流限制值   0.1A分辨率  偏移量：-750  （未写）          
	BMS_to_Upmonitor.m_data[3] = ((uint16)((CurrLimit.Curr_Charge_Cons + 750)*10))>>8;
	BMS_to_Upmonitor.m_data[4] = 0xFF;  //(uint8)((Current_Limit.ConstantDischargeCurrent + 750)*10);     //放电电流限制值   0.1A分辨率  偏移量：-750   （未写）
	BMS_to_Upmonitor.m_data[5] = 0xFF;  //((uint16)((Current_Limit.ConstantDischargeCurrent + 750)*10))>>8;  
	BMS_to_Upmonitor.m_data[6] = 0xFF;  //(uint8)((Current_Limit.FeedbackCurrent + 750)*10);          //最大反馈电流限制值   0.1A分辨率  偏移量：-750   （未写）
	BMS_to_Upmonitor.m_data[7] = 0xFF;  //((uint16)((Current_Limit.FeedbackCurrent + 750)*10))>>8;       
  while(CAN_ToUpMonitor(&BMS_to_Upmonitor));
  UpMonitor_DelayTimeus(20);

  //0x1860C0F4    电池系统绝缘信息
  BMS_to_Upmonitor.m_ID = 0x1860C0F4;     	
  BMS_to_Upmonitor.m_data[0] = (uint8)(g_IsoDetect.insulation_Vposit*10);    //正对地电压   0.1V分辨率
  BMS_to_Upmonitor.m_data[1] = ((uint16)(g_IsoDetect.insulation_Vposit*10))>>8;           
  BMS_to_Upmonitor.m_data[2] = (uint8)(g_IsoDetect.insulation_Vnegt*10);           //负对地电压   0.1V分辨率
  BMS_to_Upmonitor.m_data[3] = ((uint16)(g_IsoDetect.insulation_Vnegt*10))>>8;              
  BMS_to_Upmonitor.m_data[4] = (uint8)(g_IsoDetect.insulation_resist_P*10);           //正对地电阻值   0.1V分辨率
  BMS_to_Upmonitor.m_data[5] = ((uint16)(g_IsoDetect.insulation_resist_P*10))>>8;               
  BMS_to_Upmonitor.m_data[6] = (uint8)(g_IsoDetect.insulation_resist_N*10);            //负对地电阻值  0.1V分辨率（未写）
  BMS_to_Upmonitor.m_data[7] = ((uint16)(g_IsoDetect.insulation_resist_N*10))>>8;
  while(CAN_ToUpMonitor(&BMS_to_Upmonitor));
  UpMonitor_DelayTimeus(20);
     
  //0x1870C0F4    电池SOC、SOH信息
  for(i=0;i<2;i++)
  {
    BMS_to_Upmonitor.m_ID = 0x1870C0F4;
    switch(i)
    {
      case 0:     //SOC,SOC,单次充/放电量
      	BMS_to_Upmonitor.m_data[0] = i;     //编号 0x00
      	BMS_to_Upmonitor.m_data[1] = (uint8)((g_SOCInfo.SOC_ValueRead+0.005)*100);      //SOC_Read      1%分辨率
      	BMS_to_Upmonitor.m_data[2] = (uint8)(g_SOCInfo.SOC_ValueVoltGet*100);      //SOC_Volt      1℃分辨率             
      	BMS_to_Upmonitor.m_data[3] = (uint8)((g_BMSMonitor_SOH.SOH+0.005)*100);     //SOH           1%分辨率
      	BMS_to_Upmonitor.m_data[4] = 0xFF;                                     //单次充电量
      	BMS_to_Upmonitor.m_data[5] = 0xFF;  
      	BMS_to_Upmonitor.m_data[6] = 0xFF;//(uint8)(g_EnergyInfo.Energy_Once_DisCharge*10);    //单次放电量
      	BMS_to_Upmonitor.m_data[7] = 0xFF;//((uint16)(g_EnergyInfo.Energy_Once_DisCharge*10))>>8;        
        while(CAN_ToUpMonitor(&BMS_to_Upmonitor));
        UpMonitor_DelayTimeus(20);
      break;
      
      case 1:    //主板温度及慢充/快充充电枪温度
        BMS_to_Upmonitor.m_data[0] = i;      //编号 0x01
        BMS_to_Upmonitor.m_data[1] = (uint8)(g_EnergyInfo.Energy_Total_Charge*10);            //累计充电电量   0.1kWh分辨率
        BMS_to_Upmonitor.m_data[2] = ((uint16)(g_EnergyInfo.Energy_Total_Charge*10)) >> 8; 
        BMS_to_Upmonitor.m_data[3] = ((uint32)(g_EnergyInfo.Energy_Total_Charge*10)) >> 16;             
        BMS_to_Upmonitor.m_data[4] = (uint8)(g_EnergyInfo.Energy_Total_DisCharge*10);           //累计放电电量   0.1kWh分辨率
        BMS_to_Upmonitor.m_data[5] = ((uint16)(g_EnergyInfo.Energy_Total_DisCharge*10)) >> 8;              
        BMS_to_Upmonitor.m_data[6] = ((uint32)(g_EnergyInfo.Energy_Total_DisCharge*10)) >> 16;         
        BMS_to_Upmonitor.m_data[7] = 0xFF;
        while(CAN_ToUpMonitor(&BMS_to_Upmonitor));
        UpMonitor_DelayTimeus(20);
      break;
    }
  }
  
  //0x1880C0F4    电池故障信息
  for(i=0;i<2;i++)
  {
    Positive = Port_StateGet(Relay_Positive_PORT,Relay_Positive_pin);
    BMS_to_Upmonitor.m_ID = 0x1880C0F4;
    switch(i)
    {
      case 0:     
      	BMS_to_Upmonitor.m_data[0] = i;     //编号  0x00
      	#if(ENABLE_RELAYADHESION_JUDGE == 1)    //继电器判断功能使能
      	  BMS_to_Upmonitor.m_data[1] = (Positive & 0x03) + ((1 << 2) & 0x0C) + ((1 << 4) & 0x30) + ((1 << 6) & 0xC0);      //继电器状态 0 打开，1关闭
      	  BMS_to_Upmonitor.m_data[2] = 0xFF; //预留字节             
        	BMS_to_Upmonitor.m_data[3] = (g_Flt_Charge.Level_Volt_Cell_High & 0x03) + ((g_Flt_DisChg.Level_Volt_Cell_Low  << 2) & 0x0C) + (((g_Flt_DisChg.Level_Volt_Cell_Diff_High|g_Flt_Charge.Level_Volt_Cell_Diff_High) << 4) & 0x30) + ((g_Flt_Charge.Level_Volt_Sys_High  << 6) & 0xC0);     //单体电池过压/欠压/压差过大，总压过高
        	BMS_to_Upmonitor.m_data[4] = (g_Flt_DisChg.Level_Volt_Sys_Low  & 0x03) + (((g_Flt_DisChg.Level_Insul|g_Flt_Charge.Level_Insul) << 2) & 0x0C) + ((g_Flt_DisChg.Level_Temp_High << 4) & 0x30) + ((g_Flt_DisChg.Level_Temp_Low << 6) & 0xC0);     //总压过低，绝缘故障，放电温度过高/过低
        	BMS_to_Upmonitor.m_data[5] = (g_Flt_DisChg.Level_Temp_Diff_High & 0x03) + ((g_Flt_Charge.Level_Temp_High<< 2) & 0x0C) + ((g_Flt_Charge.Level_Temp_Low << 4) & 0x30) + ((g_Flt_Charge.Level_Temp_Diff_High << 6) & 0xC0);   //放电温差过大，充电温度过高/过低，充电温差过大
        	BMS_to_Upmonitor.m_data[6] = (0x00 & 0x03) + ((0x00 << 2) & 0x0C) + ((g_Flt_Charge.Level_Current_Charge_High << 4) & 0x30) + ((g_Flt_DisChg.Level_Current_DisCharge_High << 6) & 0xC0);   //SOC高，SOC低，充电电流过大，放电电流过大                                     
        	BMS_to_Upmonitor.m_data[7] = (0x00 & 0x03) + ((0x00 << 2) & 0x0C) + ((0x00 << 4) & 0x30) + (0b11 << 6) ;  //充电枪温度过高（未写），慢充接触器温度过高（未写），总压测量故障       
          while(CAN_ToUpMonitor(&BMS_to_Upmonitor));
          UpMonitor_DelayTimeus(20);
        #else                         //继电器判断功能未使能
          BMS_to_Upmonitor.m_data[1] = 0x00;
      	  BMS_to_Upmonitor.m_data[2] = 0xFF;              
        	BMS_to_Upmonitor.m_data[3] = 0x00;  //单体电池过压/欠压/压差过大，总压过高
        	BMS_to_Upmonitor.m_data[4] = 0x00;  //总压过低，绝缘故障，放电温度过高/过低
        	BMS_to_Upmonitor.m_data[5] = 0x00;  //放电温差过大，充电温度过高/过低，充电温差过大
        	BMS_to_Upmonitor.m_data[6] = 0x00;  //SOC高，SOC低，充电电流过大，放电电流过大                                     
        	BMS_to_Upmonitor.m_data[7] = 0x00;  //充电枪温度过高，慢充接触器温度过高，总压测量故障       
          while(CAN_ToUpMonitor(&BMS_to_Upmonitor));
          UpMonitor_DelayTimeus(20);
        #endif
      	
      break;
      
      case 1:    
        BMS_to_Upmonitor.m_data[0] = i;      //编号 0x01
        BMS_to_Upmonitor.m_data[1] = 0x00;   //BMS芯片温度过高
        BMS_to_Upmonitor.m_data[2] = State_Offline.RelayFlt_Positive&0x01;//(DiscFlt.HIVL_ECT0_Fault & 0x01) + ((DiscFlt.HIVL_ECT1_Fault << 1) & 0x02) + ((DiscFlt.HIVL_ECT2_Fault << 2) & 0x04) + ((DiscFlt.HIVL_ECT3_Fault << 3) & 0x08) + (0b11111 << 4) ;      //互锁故障
        BMS_to_Upmonitor.m_data[3] = ((State_Offline.CSSU1) & 0x01) + ((State_Offline.VCU << 1) & 0x02) + ((State_Offline.HVU << 2) & 0x04) + ((State_Offline.Charge<<3)&0x08);     //通信故障  0000 正常，0001 CSSU掉线，0010 VUC掉线，0100 HVU掉线，1000 TBOX掉线(暂时没有设为0x01)          
        BMS_to_Upmonitor.m_data[4] = g_PassiveBalance.BalanceOn;           
        BMS_to_Upmonitor.m_data[5] = g_PassiveBalance.BalanceNode;              
        BMS_to_Upmonitor.m_data[6] = 0x00;         
        BMS_to_Upmonitor.m_data[7] = 0x00;
        while(CAN_ToUpMonitor(&BMS_to_Upmonitor));
        UpMonitor_DelayTimeus(20);
      break;
    }
  }
  g_Roll_Tick.Roll_BMSUp++;
}


/*=======================================================================
 *函数名:      Task_BMUToUpMonitor(void) 
 *功能:        BMS collect information to UpMonitor
 *参数:        无       
 *返回：       无
 *说明：       BMS发送采集的电压信息信息给上位机；
========================================================================*/ 
void Task_BMUToUpMonitor(void)
{
  uint8  i,j;
  CANFRAME BMS_to_Upmonitor;
  uint8 batt,batt1; 
  
  batt=(NUM1_Batper_true+NUM2_Batper_true+NUM3_Batper_true+NUM4_Batper_true+NUM5_Batper_true)/3;
  batt1=(NUM1_Batper_true+NUM2_Batper_true+NUM3_Batper_true+NUM4_Batper_true+NUM5_Batper_true)%3;
  
  BMS_to_Upmonitor.m_IDE = 1;
	BMS_to_Upmonitor.m_RTR = 0;
	BMS_to_Upmonitor.m_dataLen = 8;
	BMS_to_Upmonitor.m_priority = 6; 
    
  BMS_to_Upmonitor.m_ID = BMS_Send_Information1;//0x18FF9700       
  for(i = 0; i <batt ; i++) 
  {
    BMS_to_Upmonitor.m_data[0] = g_Flt_Charge.Level_Charge_BalanceON_Flag;  //BMS主板是否允许子板进行均衡    
    BMS_to_Upmonitor.m_data[1] = (uint8)(i);             
    BMS_to_Upmonitor.m_data[2] = (uint8)g_LTC6811_VoltInfo.CellVolt[i*3];
    BMS_to_Upmonitor.m_data[3] = (g_LTC6811_VoltInfo.CellVolt[i*3]>>8)&0xFF;
    BMS_to_Upmonitor.m_data[4] = (uint8)g_LTC6811_VoltInfo.CellVolt[i*3+1];
    BMS_to_Upmonitor.m_data[5] = (g_LTC6811_VoltInfo.CellVolt[i*3+1]>>8)&0xFF;
    BMS_to_Upmonitor.m_data[6] = (uint8)g_LTC6811_VoltInfo.CellVolt[i*3+2];
    BMS_to_Upmonitor.m_data[7] = (g_LTC6811_VoltInfo.CellVolt[i*3+2]>>8)&0xFF;
    while(CAN_ToUpMonitor(&BMS_to_Upmonitor));
    UpMonitor_DelayTimeus(20);
  }  
  switch(batt1) 
  {
    case 1:
    BMS_to_Upmonitor.m_data[0] = g_Flt_Charge.Level_Charge_BalanceON_Flag;  //BMS主板是否允许子板进行均衡  
    BMS_to_Upmonitor.m_data[1] = (uint8)(i);                                //每个6804采集电压的编号 
    BMS_to_Upmonitor.m_data[2] = (uint8)g_LTC6811_VoltInfo.CellVolt[i*3];
    BMS_to_Upmonitor.m_data[3] = (g_LTC6811_VoltInfo.CellVolt[i*3]>>8)&0xFF;
    BMS_to_Upmonitor.m_data[4] = 0xFF;
    BMS_to_Upmonitor.m_data[5] = 0xFF;
    BMS_to_Upmonitor.m_data[6] = 0xFF;
    BMS_to_Upmonitor.m_data[7] = 0xFF;
    while(CAN_ToUpMonitor(&BMS_to_Upmonitor)); 
    UpMonitor_DelayTimeus(20);
    break;
    
    case 2:
    BMS_to_Upmonitor.m_data[0] = g_Flt_Charge.Level_Charge_BalanceON_Flag;  //BMS主板是否允许子板进行均衡  
    BMS_to_Upmonitor.m_data[1] = (uint8)(i);                                //每个6804采集电压的编号 
    BMS_to_Upmonitor.m_data[2] = (uint8)g_LTC6811_VoltInfo.CellVolt[i*3];
    BMS_to_Upmonitor.m_data[3] = (g_LTC6811_VoltInfo.CellVolt[i*3]>>8)&0xFF;
    BMS_to_Upmonitor.m_data[4] = (uint8)g_LTC6811_VoltInfo.CellVolt[i*3+1];
    BMS_to_Upmonitor.m_data[5] = (g_LTC6811_VoltInfo.CellVolt[i*3+1]>>8)&0xFF;
    BMS_to_Upmonitor.m_data[6] = 0xFF;
    BMS_to_Upmonitor.m_data[7] = 0xFF;
    while(CAN_ToUpMonitor(&BMS_to_Upmonitor)); 
    UpMonitor_DelayTimeus(20);
    break;
    
  default:
    break; 
  }   
                    
  BMS_to_Upmonitor.m_ID = BMS_Send_Information2;//0x18FF9710       
	BMS_to_Upmonitor.m_data[0] = (uint8)g_LTC6811_VoltInfo.CellVolt_Max;
  BMS_to_Upmonitor.m_data[1] = (g_LTC6811_VoltInfo.CellVolt_Max>>8)&0x00FF;
  BMS_to_Upmonitor.m_data[2] = g_LTC6811_VoltInfo.CellVolt_MaxNode;
  BMS_to_Upmonitor.m_data[3] = (uint8)g_LTC6811_VoltInfo.CellVolt_Min;
  BMS_to_Upmonitor.m_data[4] = (g_LTC6811_VoltInfo.CellVolt_Min>>8)&0x00FF;
  BMS_to_Upmonitor.m_data[5] = g_LTC6811_VoltInfo.CellVolt_MinNode;
  BMS_to_Upmonitor.m_data[6] = g_PassiveBalance.BalanceOn;//均衡的电池编号  
  BMS_to_Upmonitor.m_data[7] = g_PassiveBalance.BalanceNode;//均衡的电池编号        
  while(CAN_ToUpMonitor(&BMS_to_Upmonitor));
  UpMonitor_DelayTimeus(20);
 
  BMS_to_Upmonitor.m_ID = BMS_Send_Information3;//18FF9800      
	for(i=0; i< ((NUM_Tem+6)/7) ;i++)         
  {
    BMS_to_Upmonitor.m_data[0] = i;
    for(j=1; j < ((NUM_Tem+1)%7); j++) 
    {
      BMS_to_Upmonitor.m_data[j] = g_LTC6811_TempInfo.CellTemp[j-1+i*7];
    } 
    for(j=((NUM_Tem+1)%7); j<8; j++)
    {
      BMS_to_Upmonitor.m_data[j] = 0xFF;
    } 
    while(CAN_ToUpMonitor(&BMS_to_Upmonitor));
    UpMonitor_DelayTimeus(20); 
  }
  j=NUM_Tem%7;
  if((j!=0)&&(NUM_Tem>7))
  {
    BMS_to_Upmonitor.m_data[0] = i;
    for(i=1; i<j+1; i++)
    {
       BMS_to_Upmonitor.m_data[i] = g_LTC6811_TempInfo.CellTemp[i-1+BMS_to_Upmonitor.m_data[0]*7];
    }
    for(i=j+1; j<8; j++)
    {
       BMS_to_Upmonitor.m_data[i] = 0xFF;
    }
    while(CAN_ToUpMonitor(&BMS_to_Upmonitor));
    UpMonitor_DelayTimeus(20); 
  }
  
  BMS_to_Upmonitor.m_ID = BMS_Send_Information4;//0x18FF9810
  BMS_to_Upmonitor.m_data[0] = g_LTC6811_TempInfo.CellTemp_Max;
	BMS_to_Upmonitor.m_data[1] = g_LTC6811_TempInfo.CellTemp_MaxNode;   
	BMS_to_Upmonitor.m_data[2] = g_LTC6811_TempInfo.CellTemp_Min;
	BMS_to_Upmonitor.m_data[3] = g_LTC6811_TempInfo.CellTemp_MinNode;   
	BMS_to_Upmonitor.m_data[4] = g_LTC6811_TempInfo.CellTemp_Tatoltemp;
	BMS_to_Upmonitor.m_data[5] = g_LTC6811_TempInfo.CellTemp_Tatoltemp>>8;   
	BMS_to_Upmonitor.m_data[6] = 0xFF;
	BMS_to_Upmonitor.m_data[7] = 0xFF;       	 
  while(CAN_ToUpMonitor(&BMS_to_Upmonitor)); 
  UpMonitor_DelayTimeus(20);
  

  BMS_to_Upmonitor.m_ID = BMS_Send_Information5;//0x18FF9900
  BMS_to_Upmonitor.m_data[0] = g_LTC6811_TempInfo.ICTemp_OverState;
	BMS_to_Upmonitor.m_data[1] = g_LTC6811_OpwireInfo.OpenwireErr;	
	BMS_to_Upmonitor.m_data[2] = g_LTC6811_VoltInfo.CellVolt_Total;   
	BMS_to_Upmonitor.m_data[3] = g_LTC6811_VoltInfo.CellVolt_Total>>8;
	BMS_to_Upmonitor.m_data[4] = g_LTC6811_VoltInfo.CellVolt_Total>>16;
	BMS_to_Upmonitor.m_data[5] = (uint8)g_VoltInfo.SysVolt_Total;
	BMS_to_Upmonitor.m_data[6] = (uint8)(g_VoltInfo.SysVolt_Total>>8);
	BMS_to_Upmonitor.m_data[7] = (uint8)(g_VoltInfo.SysVolt_Total>>16);       
  while(CAN_ToUpMonitor(&BMS_to_Upmonitor));
  UpMonitor_DelayTimeus(20); 
   
  BMS_to_Upmonitor.m_ID = BMS_Send_Information6;//0x19FF9900   
  BMS_to_Upmonitor.m_dataLen = 6;          
	for(i = 0; i < NUM_IC ; i++)
	{
    BMS_to_Upmonitor.m_data[i*2] = g_LTC6811_OpwireInfo.OpenwireLocation[i];                   // 导线开路
  	BMS_to_Upmonitor.m_data[i*2 + 1] = (uint8)((g_LTC6811_OpwireInfo.OpenwireLocation[i]>>8)&0x00FF);	       
	}
  while(CAN_ToUpMonitor(&BMS_to_Upmonitor)); 
  UpMonitor_DelayTimeus(20);
  
  BMS_to_Upmonitor.m_ID = BMS_Send_Information7;//0x18FF9600
  BMS_to_Upmonitor.m_dataLen = 8;
	BMS_to_Upmonitor.m_data[0] = NUM1_Batper_true;
  BMS_to_Upmonitor.m_data[1] = NUM2_Batper_true;
  BMS_to_Upmonitor.m_data[2] = NUM3_Batper_true;
  BMS_to_Upmonitor.m_data[3] = 0xFF;
  BMS_to_Upmonitor.m_data[4] = 0xFF;
  BMS_to_Upmonitor.m_data[5] = 0xFF;
  BMS_to_Upmonitor.m_data[6] = 0xFF;  
  BMS_to_Upmonitor.m_data[7] = 0xFF;   
  while(CAN_ToUpMonitor(&BMS_to_Upmonitor)); 
  
  g_Roll_Tick.Roll_BMUUp++;
}
/*=======================================================================
 *函数名:      Task_BMSToUpMonitor(void) 
 *功能:        BMS发送给上位机信息
 *参数:        无       
 *返回：       无
 *说明：       当上位机首次发送ID号0x19FFF4C0中data[0]==0xAA时同时发送阈值信息和电池信息
               当上位机发送ID号0x19FFF4C0中data[1]==0xAA时发送阈值信息
               其中:data[0]=0xAA在上位机点击获取信息后按500ms的周期下发
               data[1]=0xAA在上位机点击获取信息后只发送一次
========================================================================*/
void Task_BMSToUpMonitor(void)
{
  if(g_MonitorBMS_Start.Msg_StarUpload == 1)
  {
    CAN_ToUpMonitorMsg();
    g_MonitorBMS_Start.Msg_StarUpload = 0;
  }
}