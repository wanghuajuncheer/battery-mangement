#ifndef _TASK_PASSBALANCE_H_
#define _TASK_PASSBALANCE_H_
    
   
    
    
    #define    BALANCEPERIO  500        
    
    

typedef struct
{
   uint8 BalanceOn;
   uint32 total_volt;
   
}BalanceInfo_T;
extern BalanceInfo_T balance_receive;

typedef struct
{
   uint8 CSSUBalanceOn;
   uint8 CSSUBalanceNode;
}ToBMU_BalanceState_T;
extern ToBMU_BalanceState_T ToBMU_BalanceState;

typedef struct
{
   uint8 BMU_Life;
   uint8 BMU_OffLine;
}BMU_OffLineState_T;
extern BMU_OffLineState_T BMU_OffLineState;


void Task_BalanceControl_ON(void);
void Task_BalanceControl_OFF(void);
    
#endif