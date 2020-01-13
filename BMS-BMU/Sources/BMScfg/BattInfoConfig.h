/*=======================================================================
 *Subsystem:   裸机
 *File:        BattInfoConfig.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _BATT_INFO_CONFIG_H_
#define _BATT_INFO_CONFIG_H_

 /*=======================================================================
                                功能设置
========================================================================*/
  //功能模块使能配置                                   
  #define           ENABLE_CAHRGE_HEAT                              0       //充电加热使能(1:使能)
  #define           ENABLE_RELAYADHESION_JUDGE                      1       //继电器粘连判断功能使能(1:使能)
  #define           ENABLE_HVIL                                     1       //高压互锁功能使能(1:使能)
  #define           ENABLE_CHARGEGUN_TEMP                           0       //充电枪温度测试使能(1:使能)
  //模式选择
  #define           MODE_STOP_OR_SLEEP                              1       //1:单片机进入伪休眠(低功耗状态),0:任务停止的模式单片机不进入模式
  #define           MODE_THERMISTOR                                 0x01    //0x00:10K的热敏电阻；0x01:100K的热敏电阻
  #define           MODE_VCU_WORK                                   1       //1:充电时VCU断电;0:充电时VCU不断电
  //初始化配置                                  
  #define           RESET_EEPROM                                    0       //EEprom内存清零(1:EEprom初始化)
  #define           RESET_SOC                                       0       //SOC初始化(1:初始化)
  #define           RESET_CLOCK                                     0       //1:时钟初始化,0:不初始化
  
  /*==============================================================
                              参数配置
  ==============================================================*/
   //电芯种类
  #define CELL_TYPE                                                 0x03    /*0x01：铅酸；  0x02：镍氢；        0x03：磷酸铁锂；0x04：锰酸锂；0x05：钴酸锂；
                                                                              0x06：三元锂；0x07：聚合物锂离子；0x08：钛酸锂；  0xFF：其他*/
  //电池单体参数配置
  #if(CELL_TYPE == 0x06) //三元锂电池
    #define         CELL_CAPACITY                                   46.0    //单体容量(Ah)
    #define         CELL_VOLT_NOMINAL                               3.7     //标称单体电压(V)
    #define         CELL_VOLT_MAX                                   4.3     //最高单体电压(V)
    #define         CELL_VOLT_MIN                                   2.8     //最低单体电压(V)
  
  #elif(CELL_TYPE == 0x03)//磷酸铁锂电池
    #define         CELL_CAPACITY                                   271.0    //单体容量(Ah)
    #define         CELL_VOLT_NOMINAL                               3.22    //标称单体电压(V)
    #define         CELL_VOLT_MAX                                   3.65    //最高单体电压(V)
    #define         CELL_VOLT_MIN                                   2.75     //最低单体电压(V)     
  #endif
  
  //电池单体使用参数  
  #define           CELL_TEMP_MAX_DISCHARGE                         60      //最大放电温度(℃)
  #define           CELL_TEMP_MIN_DISCHARGE                         (-30)   //最小放电温度(℃)
  #define           CELL_TEMP_MAX_CHARGE                            60      //最大充电温度(℃)
  #define           CELL_TEMP_MIN_CHARGE                            (-28)       //最小充电温度(℃)
  #define           CELL_LIFE_CYCLE                                 2000    //循环寿命（次），条件：25±2℃，1C/1C，100%  DOD
  #define           CELL_RESIS_INTERNAL                             16      //电池内阻(mΩ)
  
  //单体备用参数
  //#define  CellEnergyRatio                145     //单体比能量（Wh/KG）
  //#define  CellMass                       45      //单体电芯重量(g)
  //#define  CellLength                     65.2    //单体电芯长度尺寸+-1(mm)
  //#define  Cellradius                     18.3    //单体电芯宽度尺寸+-1(mm) 
  //#define  CellSelfDiscRate               0.03    //自放电率 (25℃，28天)
  //#define  CellMaxStorageTemp             35      //电池最高存储温度(℃)
  //#define  CellMinStorageTemp             15      //电池最低存储温度(℃)
  
  //电池系统参数 
  #define           SYS_PARALLEL                                    2.0                 //系统总并数
  #define           SYS_SERIES                                      50                  //系统总串数       
  #define           SYS_SERIES_YiDongLi                             25                  //易动力项目系统串数
  #define           SYS_NUMBER_BOX                                  1                   //系统的箱数
  #define           SYS_NUMBER_MODULE                               (2*SYS_NUMBER_BOX)  //系统的模组数量   
  #define           SYS_NUMBER_MODULE_TEMP                          5                   //系统每个模组的温度数量

  #define           SYS_NUMBER_BOX_MODULE                           (SYS_NUMBER_MODULE/SYS_NUMBER_BOX) //每个箱多少模组
  #define           SYS_NUMBER_TEMP                                 (SYS_NUMBER_MODULE_TEMP*SYS_NUMBER_MODULE)//系统温度总个数
  
  #define           SYS_SERIES_BOX                                  (SYS_SERIES/SYS_NUMBER_BOX)    //每个箱体的串联数
  #define           SYS_NUMBER_BOX_TEMP                             (SYS_NUMBER_BOX_MODULE*SYS_NUMBER_MODULE_TEMP)//每个箱多少温度
  #define           SYS_PARALLEL_MODULE                             SYS_PARALLEL              //每个模组并联数
  #define           SYS_SERIES_MODULE                               (SYS_SERIES/SYS_NUMBER_MODULE) //每个模组串联数
  
  #define           SYS_VOLT_NOMINAL                                (CELL_VOLT_NOMINAL*SYS_SERIES_YiDongLi)//系统额定电压
  #define           SYS_VOLT_MAX                                    (CELL_VOLT_MAX*SYS_SERIES_YiDongLi)  //系统最高电压
  #define           SYS_VOLT_MIN                                    (CELL_VOLT_MIN*SYS_SERIES_YiDongLi)  //系统最低电压
  
  #define           SYS_CAPACITY                                    (CELL_CAPACITY*SYS_PARALLEL)//系统容量(Ah)
  #define           SYS_ELECTRIC_QUANTITY                           ((CELL_CAPACITY)*(SYS_VOLT_NOMINAL)/1000.0) //系统电量(KWh)
  #define           SYS_LIFE_END_CAPACITY                           ((SYS_CAPACITY)*0.8) //电池终止寿命(%80SOH)
  
 
  


#endif