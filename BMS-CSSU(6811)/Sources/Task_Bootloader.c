/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_BootLoader.C
 *Author:      Wenming
 *Description: 通信：CAN2
               接口：
               波特率:500kbps
               调试：
 ========================================================================
 * History:    修改历史记录列表
 * 1. Date:
      Author:
      Modification:
========================================================================*/
#include  "Includes.h"                           // 包含所有h文件，可以调用所有函数与变量;   


Boot_Data_T Boot_Data;

/*=======================================================================
 *函数名:      Boot_DelayTime(void)
 *功能:        Bootloader中的延时函数
 *参数:        us:延时时间,单位:us       
 *返回：       无
 *说明：       Bootloader中的延时函数
========================================================================*/
void Boot_DelayTime(uint16 us) 
{
	  uint16 delayval;
	  delayval = us * 9;
	  while(delayval--);
}
/*=======================================================================
 *函数名:      Structure_Init(void)
 *功能:        初始化结构体,结构体数据清零
 *参数:        无       
 *返回：       无
 
 *说明：       
========================================================================*/  
void Task_BootLoader() 
{       
   DisableInterrupts;
   if(Boot_Data.OnlineUpdateCheck == 1)
   {
     Boot_Data.OnlineUpdateCheck = 0;
     Boot_Data.Boot=(uint16 *)0x0C00;
     *(Boot_Data.Boot) = 0x66;
     
     Boot_DelayTime(100);
     
     COPCTL=0x01;                              // enable watchdog     
     ARMCOP=0x00; 
   }
   EnableInterrupts; 
   Task_Flag_Counter.Counter_Roll_Boot++;     
}