/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_Create.H
 *Author:      ZWB
 *Description: 通信：
               接口：
               波特率：
 ========================================================================*/
#ifndef TASK_CREATE_
#define  TASK_CREATE_

#define ARRAY_SIZE   11                // 任务总数； 

typedef struct Task_TypeDef            // 任务结构体；
{
    uint8 flags;                       // 任务标识；  
    void (*handle)();                  // 任务处理函数的函数指针； 
}TASK;


extern TASK tasks[ARRAY_SIZE];   

TASK CreateTask(uint8 FLAGS, void (*HANDLE)());
void Create_task(void);
void Task_Handle(void);

#endif