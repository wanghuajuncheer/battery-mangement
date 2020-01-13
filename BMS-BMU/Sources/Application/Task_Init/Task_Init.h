#ifndef  TASK_CREATE_
#define  TASK_CREATE_

  #include  "TypeDefinition.h"
  
  #define ARRAY_SIZE   21                // 任务堆栈大小； 

  typedef struct                         // 任务结构体；
  {
    uint8 flags;                       // 任务标识；  
    void (*handle)();                  // 任务处理函数的函数指针； 
  }TASK;   

  typedef struct
  {
    uint8 Roll_Power;
    uint8 Roll_SOCSOH;
    uint8 Roll_BalanOff;
    uint8 Roll_VoltCMD;
    uint8 Roll_VoltCol;
    uint8 Roll_TempCMD;
    uint8 Roll_TempCol;
    uint8 Roll_Insul;
    uint8 Roll_DataPro;
    uint8 Roll_BalanOn;
    uint8 Roll_Currlimit;
    uint8 Roll_SysTime;
    uint8 Roll_FltJudg;
    uint8 Roll_Charge;
    uint8 Roll_FltCodeS;
    uint8 Roll_EEEWrite;
    uint8 Roll_FltCodeP;
    uint8 Roll_BMUUp;
    uint8 Roll_BMSUp;
    uint8 Roll_Screen;
    uint8 Roll_Boot;
    uint8 Roll_OpWire;
    
  }Roll_Tick_T;
  extern Roll_Tick_T g_Roll_Tick;


  void Task_Handle(void);                //任务轮询
  void Task_Init(void);                  //函数表的创建

#endif