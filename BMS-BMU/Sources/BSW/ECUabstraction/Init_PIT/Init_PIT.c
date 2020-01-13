/*=======================================================================
 *Subsystem:   裸机
 *File:        Init_PIT.C
 *Author:      Wenming
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
/*=======================================================================
 *函数名:      Init_PIT(uint8 channel, uint8 perio)
 *功能:        定时器初始化函数
 *参数:        channel:通道
               perio:周期(只能选择1,5,10)      
 *返回：       无
 
 *说明：       时
========================================================================*/
uint8 Init_PIT(void)
{
   PIT0_Init();
}




/*uint8 Init_PIT(uint8 channel, uint8 perio)      
{
  if(channel>7)
  {
    return (Init_Fault_PIT_Channel);
  }
  if((perio==1)||(perio==5)||(perio==10))
  {
    switch (perio)
    {
      case 1:
        PITInit(channel, 0, 249, 129);
      break;
      
      case 5:
        PITInit(channel, 0, 249, 639);
      break;
      
      case 10:
        PITInit(channel, 0, 249, 1279);
      break;
    }
  }
  else
  {
     return Init_Fault_PIT_Period;
  }
  return (Init_Normal_PIT);
}*/