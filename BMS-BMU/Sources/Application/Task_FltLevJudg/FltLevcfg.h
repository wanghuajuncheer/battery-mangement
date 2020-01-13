/*=======================================================================
 *Subsystem:   裸机
 *File:        FltLevcfg.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _FIL_LEV_CFG_H_
#define _FIL_LEV_CFG_H_
  
  #include  "BattInfoConfig.h"

 /*======================================================================
              故障报警及释放阈值宏定义(2级故障需重新上电后消除)
  ======================================================================*/
  //故障报警阈值等级命名规则
  #define           FAULT_MODE                      1        //1:2级故障为最高等级;0:1级故障为最高等级
  
  //环境温度分界线
  #define           NTEMP_BOUNDARY                  (0+40)        //1:2级故障为最高等级;0:1级故障为最高等级
  
  //-------------------放电故障及恢复阈值(Level_DisCharge)----------------
 
  //放电总压低(分辨率:0.0001V)(偏移量:0)
    //常温状态
  #define         F2_DISCHG_VOLTSL_NT               27000.0*(SYS_SERIES_YiDongLi)

  #define         F1_DISCHG_VOLTSL_NT               28500.0*(SYS_SERIES_YiDongLi)
  #define         R1_DISCHG_VOLTSL_NT               30500.0*(SYS_SERIES_YiDongLi)
    //低温状态
  #define         F2_DISCHG_VOLTSL_LT               25000.0*(SYS_SERIES_YiDongLi)

  #define         F1_DISCHG_VOLTSL_LT               27000.0*(SYS_SERIES_YiDongLi)
  #define         R1_DISCHG_VOLTSL_LT               28000.0*(SYS_SERIES_YiDongLi)
  
  #if (CELL_TYPE==0x03)//磷酸铁锂电池
    //放电单体低压(分辨率:0.0001V)(偏移量:0)
      //常温状态
    #define         F2_DISCHG_VOLTCL_NT             28000

    #define         F1_DISCHG_VOLTCL_NT             29000
    #define         R1_DISCHG_VOLTCL_NT             31000
      //低温状态
    #define         F2_DISCHG_VOLTCL_LT             25000

    #define         F1_DISCHG_VOLTCL_LT             27000
    #define         R1_DISCHG_VOLTCL_LT             28000
    
    //放电单体压差(分辨率:0.0001V)(偏移量:0)
    #define         F1_DISCHG_VOLTCD                3000  
    #define         R1_DISCHG_VOLTCD                2500  

  #endif    
  
  //放电高温(分辨率:1℃)(偏移量:-40)
  #define           F2_DISCHG_TEMPH                 (60 +40)

  #define           F1_DISCHG_TEMPH                 (55 +40)
  #define           R1_DISCHG_TEMPH                 (53 +40)
  
  //放电低温(分辨率:1℃)(偏移量:-40)
  #define           F2_DISCHG_TEMPL                 ((-30) +40) 

  #define           F1_DISCHG_TEMPL                 ((-15) +40)  
  #define           R1_DISCHG_TEMPL                 ((-12) +40)
  
  //放电温差
  #define           F1_DISCHG_TEMPD                 15
  #define           R1_DISCHG_TEMPD                 13
  
  //放电过流    
  #define           F2_DISCHG_CURRH                 650                   

  #define           F1_DISCHG_CURRH                 620                
  #define           R1_DISCHG_CURRH                 600 
  
  //绝缘故障 
  #define           F2_DISCHG_INSUL                 (100 * CELL_VOLT_MAX * SYS_SERIES_YiDongLi * 0.001)    //单位:kΩ
  
  //------------------充电故障阈值(FaultLevel_Charge)---------------------------
  //充电总压高(分辨率:0.0001V)(偏移量:0)
  #define         F2_CHARGE_VOLTSH                  37000.0*(SYS_SERIES_YiDongLi)

  #define         F1_CHARGE_VOLTSH                  36500.0*(SYS_SERIES_YiDongLi)
  #define         R1_CHARGE_VOLTSH                  36000.0*(SYS_SERIES_YiDongLi)
  
  #if (CELL_TYPE==0x03)//磷酸铁锂电池
    //充电单体高压(分辨率:0.0001V)(偏移量:0)
    #define         F2_CHARGE_VOLTCH                37000

    #define         F1_CHARGE_VOLTCH                36500
    #define         R1_CHARGE_VOLTCH                36000
   
    //充电电单体压差(分辨率:0.0001V)(偏移量:0)
    #define         F1_CHARGE_VOLTCD                3000  
    #define         R1_CHARGE_VOLTCD                2500  
  #endif    
  
  //充电高温(分辨率:1℃)(偏移量:-40)
  #define           F2_CHARGE_TEMPH                 (55 +40)

  #define           F1_CHARGE_TEMPH                 (50 +40)
  #define           R1_CHARGE_TEMPH                 (48 +40)
  //充电低温(分辨率:1℃)(偏移量:-40)
  #define           F2_CHARGE_TEMPL                 ((-5) +40) 

  #define           F1_CHARGE_TEMPL                 ((0) +40)  
  #define           R1_CHARGE_TEMPL                 ((5) +40)
  //充电温差
  #define           F1_CHARGE_TEMPD                 15
  #define           R1_CHARGE_TEMPD                 13
  //充电过流    
  #define           F2_CHARGE_CURRH                 275                   

  #define           F1_CHARGE_CURRH                 180                
  #define           R1_CHARGE_CURRH                 175 
  //绝缘故障
  #define           F2_CHARGE_INSUL                 100                   
                   
  
  //-------------------其他故障阈值(FaultLevel_Other)---------------------
  #define           OPENWIRE                        1
  
  
/*======================================================================
                      充放电周期时间及延迟时间宏定义
========================================================================*/
  //周期时间
  #define PERIOD_DISCHARGE              500                 // 放电任务周期是100ms
  #define PERIOD_CHARGE                 500                 // 充电任务周期是100ms
  //延迟时间
  #define DELAYTIME_UNLOCK              15                   //解锁延迟
  #define DELAYTIME_DANGERLEVEL1        10      
  #define DELAYTIME_DANGERLEVEL2        5
  #define DELAYTIME_DANGERLEVEL3        3
  #define DELAYTIME_GETRIGHT            1
  #define DELAYTIME_OFFLINE             5         //掉线延迟
  #define DELAYTIME_HVIL                5          //高压互锁延迟
  
  #define Addr_VoltLock                  0x0C02    //电压锁定标记地址



#endif