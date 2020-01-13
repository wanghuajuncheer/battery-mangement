#ifndef  BALANCE_H_
#define  BALANCE_H_


 
extern uint8  Balance_CFGR[NUM_IC][6];          

extern uint16  Balance_Battery_Num;
extern uint8   Balance_Time;

void Balance_Config( uint8 *CFG,uint8 gpio, uint8 refon,uint8 swtrd, uint8 adcopt,
                 float  vuv,float   vov,uint16 ddc,uint8 dcto);
                 
void Passive_Balance(void);  
                 
               
                 
#endif