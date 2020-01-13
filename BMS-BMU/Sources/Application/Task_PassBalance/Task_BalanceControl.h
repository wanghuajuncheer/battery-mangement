#ifndef _TASK_PASSBALANCE_H_
#define _TASK_PASSBALANCE_H_
    
   
    #include   "TypeDefinition.h"
    
    #define    BALANCEPERIO  500  
    
    typedef struct
    {
      uint8 BalanceNode;
      uint8 BalanceOn;
    }PassiveBalance_T;
    extern PassiveBalance_T g_PassiveBalance; 
    
    void Task_BalanceControl_ON(void);
    void Task_BalanceControl_OFF(void);
    
#endif