#ifndef  _CAN_H
#define  _CAN_H



void INIT_CAN2(void);

Bool MSCAN2SendMsg(pCANFRAME sendFrame);
Bool MSCAN2GetMsg(pCANFRAME sendFrame);

#endif
