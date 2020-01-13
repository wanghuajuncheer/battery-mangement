/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_SysTimeGet.c
 *Author:      Wenming
 *Description: 
 ========================================================================
 * History:    修改历史记录列表
 * 1. Date:
      Author:
      Modification:
========================================================================*/
#include  "includes.h"

SysTime_T g_SysTime;
/*=======================================================================
 *函数名:      Time_Init(void)
 *功能:        时钟初始化
 *参数:        无       
 *返回：       无
 
 *说明：       
========================================================================*/
void Time_Init(void)
{
  uint32 timestart;
  DS3231_Read_Time();
  timestart = ((uint32)Read_IIC_Time.IIC_Read_Year<<24) + ((uint32)Read_IIC_Time.IIC_Read_Month<<16) + ((uint32)Read_IIC_Time.IIC_Read_Day<<8) + ((uint32)(Read_IIC_Time.IIC_Read_Hour));
  g_SysTime.SOC_Static_Time = (timestart - g_SysTime.BMS_PowerOff_Time);//SOC计算用的静置时间(总断电时长)
  
  g_SysTime.BMS_StartRun_Time = Read_IIC_Time.IIC_Read_Minute;
  
  g_SysTime.BMS_SleepStatic_Time = ((uint16)Read_IIC_Time.IIC_Read_Hour<<8) | Read_IIC_Time.IIC_Read_Minute ;//避免上电后电流为0时,此初始值为0,静态的电流很快满足静态时间
}
/*=======================================================================
 *函数名:      Sleep_StaticTime
 *功能:        计算当系统为常电时，系统SOC查表的时间
 *参数:        readhour、readminite:实时读取的时间(时/分)
               current：实时测量电流的大小 
               currentset：静态电流大小设置
               starttime：满足条件时存储的初始时间
               hourset:用户设定的多长时间满足条件
               
 *返回：       uint8：返回是否需要查表,1:进行查表;0:不查表 
 *说明：       当系统为常电时，判断电流为0且SOC在[0~20]或[90~100]范围内时间超过
               3小时时进行查表初始初始化
========================================================================*/
uint8 Sleep_StaticTime(uint8 readhour, uint8 readminite, float current, float currentset, uint16 hourset)
{
    static uint8 Time_firstflag;
    static uint16 cnt[2];
    if(abs(current) < currentset)
    { 
      
      if((++cnt[0])*PEWERONOFF_PERIO/1000 >= 10) //当10S内都是电流小于currentsetA开始计时
      {  
        cnt[1] = 0;  
        cnt[0] = (uint8)(10000/PEWERONOFF_PERIO); 
        if(Time_firstflag == 0)
        { 
           Time_firstflag = 1;    
           g_SysTime.BMS_SleepStatic_Time = ((uint16)readhour<<8) | readminite ;           
        }
        else
        {   
          //此处若初始值为0,那么在电流小于5A的情况下可能会直接导致此处判断条件的通过
          if((((uint16)readhour<<8) | readminite)-g_SysTime.BMS_SleepStatic_Time >= 0)
          {
            if((((uint16)readhour<<8) | readminite)-g_SysTime.BMS_SleepStatic_Time >= (hourset<<8))//大于12小时查表
            {
               return TRUE;
            }
          }
          else
          {
            if(((uint16)24<<8)-g_SysTime.BMS_SleepStatic_Time+(((uint16)readhour<<8) | readminite) >= (hourset<<8))
            {   
               return TRUE;
            }
          }
        }
      }       
    }
    else if((++cnt[1])*PEWERONOFF_PERIO/1000 >= 2)//电流跳变持续超过2s才认为它是处于非工作状态
    {
       cnt[0] = 0; 
       cnt[1] = (uint8)(2000/PEWERONOFF_PERIO);
       Time_firstflag = 0;
    }
    return FALSE;
}


//BMS上电初始时间
/*=======================================================================
 *函数名:      Task_SysTimeGet(void)
 *功能:        BMS的上电时间的获取
 *参数:        starttime:读取起始时间的变量       
 *返回：       无
 *说明：       
========================================================================*/
void Task_SysTimeGet(void)
{
   uint8 timestart;//最新一次的时间记录
   uint8 timediff; //BMS运行的时间周期
   
   DS3231_Read_Time();
   
   timestart = Read_IIC_Time.IIC_Read_Minute;
   
   if(timestart - g_SysTime.BMS_StartRun_Time >=0) 
   {
      timediff = timestart - g_SysTime.BMS_StartRun_Time;//本次的记录时间-上次的记录时间
   }
   else
   {
      timediff = 60 - g_SysTime.BMS_StartRun_Time + timestart;
   }
   g_SysTime.BMS_StartRun_Time = Read_IIC_Time.IIC_Read_Minute;
   g_SysTime.BMS_TotalRun_MiniteTime = g_SysTime.BMS_TotalRun_MiniteTime + timediff; //系统的总运行时间
   
   g_SysTime.BMS_PowerOff_Time = ((uint32)Read_IIC_Time.IIC_Read_Year<<24) + ((uint32)Read_IIC_Time.IIC_Read_Month<<16)\
                                  + ((uint32)Read_IIC_Time.IIC_Read_Day<<8) + ((uint32)(Read_IIC_Time.IIC_Read_Hour));
   
   g_Roll_Tick.Roll_SysTime++;

}