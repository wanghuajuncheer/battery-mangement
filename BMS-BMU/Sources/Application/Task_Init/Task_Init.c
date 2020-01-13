/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_Init.C
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

TASK tasks[ARRAY_SIZE];            //任务堆栈设计  

Roll_Tick_T g_Roll_Tick;   
/*=======================================================================
 *函数名:      Task_Init(uint8 FLAGS, void (*HANDLE)())  
 *功能:        任务创建函数
 *参数:        无       
 *返回：       无
 *说明：       每个任务创建需要一个时间计时的标志位与函数指针；
========================================================================*/
static
TASK Task_Register(uint8 FLAGS, void (*HANDLE)(void)) // 创建单任务结构体函数；
{
	TASK task;
	task.flags = FLAGS;
	task.handle = HANDLE;
	return (task);
} 

/*=======================================================================
 *函数名:      Task_Handle(void)  
 *功能:        任务轮询
 *参数:        无       
 *返回：       无
 *说明：       创建任务数组函数，便于轮询执行任务
========================================================================*/
void Task_Handle(void)// 任务执行函数，按任务的创建顺序执行任务；                     
{
  uint8 i;
  for (i = 0; i < ARRAY_SIZE; i++)
  {   
     if (tasks[i].flags==1)                    // 查询任务时间是否到；
     {             
        (*tasks[i].handle)();                  // 执行任务
        tasks[i].flags = 0;                    // 消除任务标记位              
     }        
  }
}
   
/*=======================================================================
 *函数名:      Task_Create(void)  
 *功能:        多任务创建函数
 *参数:        无       
 *返回：       无
 *说明：       创建任务数组函数，便于轮询执行任务
========================================================================*/  
void Task_Init()                                      
{                   
  //BMS数据接收及处理任务     
  tasks[0] = Task_Register(0, Task_BalanceControl_OFF);   //关闭均衡控制       
  tasks[1] = Task_Register(0, Task_PowerOnOff);           //BMS自检及上下电的控制    
  tasks[2] = Task_Register(0, Task_SOCSOH);               //SOCSOH计算函数
  tasks[3] = Task_Register(0, Task_VoltCMDSend);          //采集电压前发送的命令
  tasks[4] = Task_Register(0, Task_VoltCollect);          //电压采集
  tasks[5] = Task_Register(0, Task_TempCMDSend);          //采集温度前发送的命令
  tasks[6] = Task_Register(0, Task_TempCollect);          //温度采集 
  tasks[7] = Task_Register(0, Task_InsulationDetect);     //绝缘检测 
  tasks[8] = Task_Register(0, Task_DataProcess);          //数据处理

  tasks[9] = Task_Register(0, Task_BalanceControl_ON);    //开启均衡控制(包含控制策略)

  tasks[10] = Task_Register(0, Task_CurrLimit);           //电流限制函数
  tasks[11] = Task_Register(0, Task_SysTimeGet);          //系统时间的获取/系统运行时间的计算

  tasks[12] = Task_Register(0, Task_FltLevJudg);          //故障等级判断 

  tasks[13] = Task_Register(0, Task_Charge);              //充电任务
  tasks[14] = Task_Register(0, Task_FltCodeStore);        //故障代码存储 
  tasks[15] = Task_Register(0, Task_EEpromWrite);         //变量值的存储
  tasks[16] = Task_Register(0, Task_FltCodeProcess);      //故障代码上传

  tasks[17] = Task_Register(0, Task_BMUToUpMonitor);      //上传主板采集电压温度数据信息    
  tasks[18] = Task_Register(0, Task_BMSToUpMonitor);      //上传主板的故障信息数据 

  tasks[19] = Task_Register(0, Task_ScreenTransfer);      //显示屏的任务,3S任务
  tasks[20] = Task_Register(0, Task_BootLoader);          //Bootloader任务
}

/*=======================================================================
 *函数名:      Task_Create(void)  
 *功能:        多任务创建函数
 *参数:        无       
 *返回：       无
 *说明：       创建任务数组函数，便于轮询执行任务
========================================================================*/
void Task_Roll(void)
{
  static uint8 cnt[5]={0,0,0,0,0};
  switch(PIT_TimePeriod.T500ms)//500ms的周期
  {
    case 1:
      tasks[0].flags = 1;//Task_BalanceControl_OFF
    break;
      
    case 3:   
      tasks[1].flags = 1;//Task_PowerOnOff
    break;
    
    case 5: //SOCSOH的计算周期为100ms
    case 15:
    case 25:
    case 35:
    case 45:
      tasks[2].flags = 1;//Task_SOCSOH
    break;
    
    case 7:
      tasks[3].flags = 1;//Task_VoltCMDSend
    break;
    
    case 9:
      tasks[4].flags = 1;//Task_VoltCollect
    break;
    
    case 11:
      tasks[5].flags = 1;//Task_TempCMDSend
    break;
    
    case 13:
      tasks[6].flags = 1;//Task_TempCollect
    break;
      
    case 17:
      tasks[7].flags = 1;//Task_InsulationDetect
    break;
    
    case 21:
      
      tasks[8].flags = 1;//Task_DataProcess
    break;
    
    case 23:
      tasks[9].flags = 1;//Task_BalanceControl_ON
    break;
    
    case 27:
      tasks[10].flags = 1;//Task_CurrLimit
    break; 
    
    case 29:
      if(cnt[4]%10 == 0)  //10*0.5S任务 
      {
        cnt[4] = 0;
        tasks[11].flags = 1;//Task_SysTimeGet
      }
      cnt[4]++;
    break;
    
    case 31:
      tasks[12].flags = 1;//Task_FltLevJudg
    break;
    
    case 33:
      tasks[13].flags = 1;//Task_Charge
    break;
    
    case 37:
      tasks[14].flags = 1;//Task_FltCodeStore
    break;

    case 39:
      tasks[16].flags = 1;//Task_FltCodeProcess
    break;
      
    case 40:
      tasks[20].flags = 1;//Task_BootLoader
      
    break;

    case 41:
      if(cnt[0]%40 == 0)//40*0.5S任务 
      {
        cnt[0] = 0;
        tasks[15].flags = 1;//Task_EEpromWrite
      }
      cnt[0]++;
    break;
    
    case 43:
      if(cnt[1]%2 == 0)//2*0.5S任务 
      {
        cnt[1] = 0;
        tasks[17].flags = 1;//Task_BMUToUpMonitor
      }
      cnt[1]++;
    break;
      
    case 47:
      if(cnt[2]%2 == 0)//2*0.5S任务 
      {
        cnt[2] = 0;
        tasks[18].flags = 1;//Task_BMSToUpMonitor
      }
      cnt[2]++;
    break;
    
    case 49:
      if(cnt[3]%6 == 0)//6*0.5S任务 
      {
        cnt[3] = 0;
        tasks[19].flags = 1;//Task_ScreenTransfer
      }
      cnt[3]++;
    break; 
    
    default:
    break;
  }   
}
 
