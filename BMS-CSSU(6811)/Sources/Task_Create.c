/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_Create.C
 *Author:      ZWB
 *Description: 通信：
               接口：
               波特率：
 ========================================================================
 * History:    修改历史记录列表
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#include  "Includes.h"                 /* 包含所有h文件，可以调用所有函数与变量 */ 

TASK tasks[ARRAY_SIZE];                /* 任务总数 */

/*=======================================================================
 *函数名:      CreateTask(uint8 FLAGS, void (*HANDLE)())  
 *功能:        任务创建函数
 *参数:        无       
 *返回：       无
 *说明：       每个任务创建需要一个时间计时的标志位与函数指针；
========================================================================*/
TASK CreateTask(uint8 FLAGS, void (*HANDLE)())            
{
  	TASK task;
  	
  	task.flags = FLAGS;                  /* 任务标志位 */            
  	task.handle = HANDLE;                /* 函数指针调用任务函数 */
  	return (task);
}   
/*=======================================================================
 *函数名:      Create_task(void)  
 *功能:        多任务创建函数
 *参数:        无       
 *返回：       无
 *说明：       创建任务数组函数，便于轮询执行任务类，A类是600ms执行一次；
========================================================================*/ 
void Create_task(void)                                          
{  
   
   tasks[0] = CreateTask( 0 ,Task_Volt_Collect);       /* A01 电压收集 */
   
   tasks[1] = CreateTask( 0 ,Task_Volt_Process);       /* A02 电压处理 */
     
   tasks[2] = CreateTask( 0 ,Task_Pack_Temp_Collect);  /* A03 Pack温度收集 */
   
   tasks[3] = CreateTask( 0 ,Task_Pack_Temp_Process);  /* A04 Pack温度处理 */
   
   tasks[4] = CreateTask( 0 ,Task_Chip_Temp_Collect);  /* A05 芯片温度收集 */ 
        
   tasks[5] = CreateTask( 0 ,Task_Chip_Temp_Process);  /* A06 芯片温度处理 */ 
   
   tasks[6] = CreateTask( 0 ,Task_BalanceControl_OFF);       /*  关闭均衡 */
   
   tasks[7] = CreateTask( 0 ,Bms_to_Up_Monitor);       /* A07 导线开路检测 */
   
   tasks[8] = CreateTask( 0 ,InsulationDetect);       /* A07 导线开路检测 */
   
   tasks[9] = CreateTask( 0 ,Task_BalanceControl_ON);       /*  打开均衡 */
   
   tasks[10] = CreateTask( 0 ,Task_BootLoader);       /*  打开均衡 */
}

/*=======================================================================
 *函数名:      Task_Handle(void)
 *功能:        任务轮询函数
 *参数:        无       
 *返回：       无
 *说明：       在主函数中不断的查找任务时间是否到了，如果到了就执行。
========================================================================*/
void Task_Handle(void)                                                 
{
    uint8 i;
    
    for (i = 0; i < ARRAY_SIZE; ++i)   // 轮询所有任务;
    {
       
       if (tasks[i].flags==1)          // 查询任务标志位是否置1，置1就执行;
       {
            (*tasks[i].handle)();         // 执行任务; 使用函数指针;
            tasks[i].flags = 0;        // 对任务标志位清零;
       }   
    }
    Task_Flag_Counter.Time_Handle++;
}