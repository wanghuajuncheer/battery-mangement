/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_Roll.C
 *Author:      ZWB
 *Description: 通信：
               接口：
               波特率：
 ========================================================================
 * History:    修改历史记录列表
 * 1. Date:    2017 - 11 - 9
      Author:  ZWB
      Modification: 
========================================================================*/
#include  "Includes.h"                

Task_Time Task_Flag_Counter;                             
                   
   
          
/*=======================================================================
 *函数名:      Task_Roll(void) 
 *功能:        任务时间函数
 *参数:        无       
 *返回：       无
 *说明：       任务时间到标志位给1
========================================================================*/
void Task_Roll(void)                                
{    
     static uint8 cnt;
     
     
     switch(Task_Flag_Counter.Time_Flag600ms)
     {
         
            
         case 1:
               
               tasks[6].flags=1;   // 关闭均衡 
               
             break;   
         
         
         case 3:
      
               tasks[0].flags=1;     // 电压收集 
             
             break;
             
         case 5:

               tasks[1].flags=1;   // 电压处理 
             
             break;
             
             case 6:
               
               tasks[9].flags=1;   // 打开均衡 
             
             break;
             
         case 7:
               
               
               tasks[2].flags=1;   // Pack温度收集 
             
             break;  
             
         case 9:
               
               tasks[3].flags=1;   // Pack温度 
              
             break; 
         
         case 11:
               
               tasks[4].flags=1;   // 芯片温度收集
             
             break;
         case 13:
               
               tasks[9].flags=1;   // 打开均衡 
             
             break;
         case 15:
               
               tasks[5].flags=1;   // 芯片温度处理 
             
             break;    
             
         case 17:
              if(cnt%2 == 0)
              {
               cnt = 0;
               tasks[7].flags=1;   // CAN 
              }
              cnt++;
              
             break; 
         case 19:
               
               tasks[8].flags=1;   // 绝缘检测 
             
             break;
             
         case 23:
               
               tasks[10].flags=1;   // bool 
             
             break;
         
                     
         case 50:
               
               Task_Flag_Counter.Time_Flag600ms = 0; 
             
             break;        
           
         default:
              break; 
      
     }

                           
     Task_Flag_Counter.Time_Roll++; 
}

/*========================================================================*/