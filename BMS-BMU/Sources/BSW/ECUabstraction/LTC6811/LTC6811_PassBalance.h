#ifndef LTC6811_PASSBALANCE_H_
#define LTC6811_PASSBALANCE_H_
   
   
   #include  "TypeDefinition.h"
   #include  "LTC6811_CMDConfig.h"
   #include  "LTC6811_ConnectType.h"
   
   
   
   void  LTC6811_BalanceCMDSend(uint8 *CFG, uint8 gpio, uint8 refon, uint8 swtrd, uint8 adcopt,
                                float  vuv, float vov,  uint16 ddc,  uint8 dcto);
   uint8 LTC6811_BalanceControl(uint8 state1, uint8 state2, uint8 state3, uint8 time);
extern uint8 Balance_CFGR[NUM_IC][6];
#endif