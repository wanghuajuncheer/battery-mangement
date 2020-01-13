/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_UpMonitor.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _TASK_UP_MONITOR_H_
#define _TASK_UP_MONITOR_H_  

  #include  "TypeDefinition.h"  
  #include  "CAN.h"
  /********BMS to Up Monitor information CSSU*******/

  #define    BMS_Send_Information1   (0x18FF9700+NUM_pack)
  #define    BMS_Send_Information2   (0x18FF9710+NUM_pack)
  #define    BMS_Send_Information3   (0x18FF9800+NUM_pack)
  #define    BMS_Send_Information4   (0x18FF9810+NUM_pack)
  #define    BMS_Send_Information5   (0x18FF9900+NUM_pack)
  #define    BMS_Send_Information6   (0x19FF9900+NUM_pack)
  #define    BMS_Send_Information7   (0x18FF9600+NUM_pack)

 //上位机数据标定
 //电池阀值标定
  //#define    Monitor_BMS_Volt   0x1810C0F4     //电池电压阈值信息        
  typedef struct 
  { //00
    uint16  Volt_Cell_High1;  //单体电压过压一级阀值， 单位 0.0001V/Bit
    uint16  Volt_Cell_High2;  //单体电压过压二级阀值， 单位 0.0001V/Bit
    //01
    uint16  Volt_Cell_Low1; //单体电压欠压一级阀值， 单位 0.0001V/Bit
    uint16  Volt_Cell_Low2; //单体电压欠压二级阀值， 单位 0.0001V/Bit
    //02
    uint16  Volt_Sys_High1;  //电池组过压阀值1级
    uint16  Volt_Sys_High2;  //电池组过压阀值2级
    //03
    uint16  Volt_Sys_Low1; //电池组欠压阀值1级
    uint16  Volt_Sys_Low2; //电池组欠压阀值2级
    //04
    uint16  Volt_Cell_Diff1;  //单体压差一级阀值， 单位 0.0001V/Bit
  
  }BMSMonitor_Volt_T;
  extern BMSMonitor_Volt_T g_BMSMonitor_Volt;
  
  //#define    Monitor_BMS_Temp   0x1811C0F4     //温度阈值信息 
  typedef struct 
  { //00
    uint8  Temp_Charge_High1;   //单体充电温度过温一级阀值， 单位 1℃/Bit  偏移量：-40
    uint8  Temp_Charge_High2;   //单体充电温度过温二级阀值， 单位 1℃/Bit  偏移量：-40
    
    uint8  Temp_Charge_Low1;  //单体充电温度低温一级阀值， 单位 1℃/Bit  偏移量：-40
    uint8  Temp_Charge_Low2;  //单体充电温度低温二级阀值， 单位 1℃/Bit  偏移量：-40
    
    //01
    uint8  Temp_DisCharge_High1;    //单体放电温度过温一级阀值， 单位 1℃/Bit   偏移量：-40
    uint8  Temp_DisCharge_High2;    //单体放电温度过温二级阀值， 单位 1℃/Bit   偏移量：-40
    
    uint8  Temp_DisCharge_Low1;   //单体放电温度低温一级阀值， 单位 1℃/Bit   偏移量：-40
    uint8  Temp_DisCharge_Low2;   //单体放电温度低温二级阀值， 单位 1℃/Bit   偏移量：-40

    //02
    uint8  Temp_Charge_Diff1;     //单体充电温差一级阀值， 单位 1℃/Bit    偏移量：-40
  
    uint8  Temp_DisCharge_Diff1;  //单体放电温差一级阀值， 单位 1℃/Bit    偏移量：-40
 
  }BMSMonitor_Temp_T;
  extern BMSMonitor_Temp_T g_BMSMonitor_Temp;
 
  //#define    Monitor_BMS_Current   0x1812C0F4     //电流阈值及SOC阈值信息 
  typedef struct 
  { 
    //00
    uint16  Current_DisCharge_High1;  //放电过流阈值1级，0.1A/位     偏移量:-750
    uint16  Current_DisCharge_High2;  //放电过流阈值2级，0.1A/位     偏移量:-750

    //01
    uint16  Current_Charge_High1;     //充电过流阈值1级，0.1A/位      偏移量:-750
    uint16  Current_Charge_High2;     //充电过流阈值2级，0.1A/位      偏移量:-750
    
  }BMSMonitor_Curr_T;
  extern BMSMonitor_Curr_T g_BMSMonitor_Curr;
  
  //#define    Monitor_BMS_Insulation   0x1813C0F4     //绝缘故障阈值信息 
  typedef struct 
  { //00
    uint16 Insulation_Resis1;     // 绝缘电阻一级阀值， 单位 0.1KΩ/Bit;
    uint16 Insulation_Resis2;     // 绝缘电阻二级阀值， 单位 0.1KΩ/Bit;
    uint16 Insulation_Resis3;     // 绝缘电阻三级阀值， 单位 0.1KΩ/Bit;  
  
  }BMSMonitor_Insu_T;
  extern BMSMonitor_Insu_T g_BMSMonitor_Insul; 
  
  //发送至上位机的SOC初始化/查表标记
  /*typedef struct 
  { 
    uint8   SOC_t; //放在发送至上位机的头文件中  
  
  }BMSMonitor_SOC_T;
  extern BMSMonitor_SOC_T g_BMSMonitor_SOC;*/ 
  
  //发送至上位机的SOH值
  typedef struct 
  { 
    float  SOH; //放在发送至上位机的头文件中  
  
  }BMSMonitor_SOH_T;
  extern BMSMonitor_SOH_T g_BMSMonitor_SOH; 
  
  //0x1814C0F4
  typedef struct
  {
     uint16 Voll_Sys_Low1_LT;
     uint16 Voll_Sys_Low2_LT;
     uint16 Volt_Cell_Low1_LT; //单体电压欠压一级阀值， 单位 0.0001V/Bit
     uint16 Volt_Cell_Low2_LT; //单体电压欠压二级阀值， 单位 0.0001V/Bit
  
  }BMSMonitor_New_LT_T;
  extern BMSMonitor_New_LT_T g_BMSMonitor_New_LT;

//---------------------------------------------------------------------------------------------------- 
  
  //#define    Monitor_BMS_SOC   0x1915F4C0         //特殊类信息标定说明
  typedef struct 
  { //00
    uint16  SOC_Init;             //SOC初始值， 单位 1%/Bit；
    union
    {
     int  Relay1_Station;       //继电器开关变量
      struct 
      {
        int Relay_Positive:2;    //主正继电器
        int Relay_Negtive:2;     //主负继电器
        int Relay_Precharge:2;   //预充继电器
        int Relay_Charge:2;      //充电继电器
        
      }Monitor_BMS_DelayBit;
    }Monitor_BMS_Delay_T;
    
    uint16 Ref_Volt;               //参考电压标定(0~65536)
    uint16 Ref_Current;            //电流传感器的斜率
    //01
    uint16 Volt_Resis;             //电压检测中分压电阻的标定
    uint32 Pack_Total_Capacity;    //标定SOH中的总容量(2000*额定容量)
    //02
    uint16 BMS_Running_Time;       //BMS运行时长
      
  }MonitorBMS_SOC_T;
  extern MonitorBMS_SOC_T g_MonitorBMS_SOC;
   
  //#define    Monitor_to_BMSFF   0x19FFF4C0
  typedef struct 
  {
    uint8 Msg_StarUpload;              //BMS与上位机通开始信信号，0xAA： 通信开始； 其他： 不进行通信；
    uint8 Threshold_StarUpload;        //数据更新 1~5； （1 代表 500ms）  
  }MonitorBMS_Start_T;
  extern MonitorBMS_Start_T g_MonitorBMS_Start; 

  void Init_UpMonitor(void); 

  uint8 CAN_ToUpMonitor(pCANFRAME);
  uint8 CAN_UpMonitorInit(void);
  void Task_BMSToUpMonitor(void);
  void Task_BMUToUpMonitor(void);

  void UpMonitor_to_Bms(pCANFRAME);

#endif