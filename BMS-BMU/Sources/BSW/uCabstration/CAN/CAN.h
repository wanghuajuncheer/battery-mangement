/*=======================================================================
 *Subsystem:   裸机
 *File:        CAN.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _CAN_H_
#define _CAN_H_
  
  #include  "TypeDefinition.h"
  
  //CAN初始化
  enum Init_CAN
  {
    //初始化位置正常为0
    Init_Normal_CAN = 0,
    //初始化位置失败的对应
    Init_Fault_CAN_BaudRate,
    Init_Fault_CAN_Unready1,
    Init_Fault_CAN_Unready2,
    Init_Fault_CAN_Synchr
  };
  
  //CAN发送数据
  enum SendMsg_CAN
  {
    SendMsg_Normal = 0,
    SendMsg_Fault_Lenth,
    SendMsg_Fault_Synch,
    SendMsg_Fault_NoEmptyNode
  };
  
  //CAN接收数据
  enum GetMsg_CAN
  {
    GetMsg_Normal = 0,
    GetMsg_Fault_RFLG_RXF
  };
  
  //CAN报文结构体
  typedef struct 
  {
    uint32 m_ID;      // msg发送方ID
    uint8 m_IDE;      // 扩展帧为1，标准帧为0
    uint8 m_RTR;      // 远程帧为1，数据帧为0
    uint8 m_data[8];  // 帧数据
    uint8 m_dataLen;  // 帧数据长度
    uint8 m_priority; // 发送优先级 
  }CANFRAME,*pCANFRAME; 


  //CAN0
  uint8 CAN0_Init(uint16 Baud_Rate);
  uint8 CAN0_SendMsg(pCANFRAME sendFrame);
  uint8 CAN0_GetMsg(pCANFRAME receiveFrame);

  //CAN1  充电
  uint8 CAN1_Init(uint16 Baud_Rate);
  uint8 CAN1_SendMsg(pCANFRAME sendFrame);
  uint8 CAN1_GetMsg(pCANFRAME receiveFrame);
  
  //CAN2  内网 
  uint8 CAN2_Init(uint16 Baud_Rate);
  uint8 CAN2_SendMsg(pCANFRAME sendFrame);
  uint8 CAN2_GetMsg(pCANFRAME receiveFrame);
  
  void  CAN2_GetMsg_Process(pCANFRAME receiveFrame);
   
  //CAN3
  uint8 CAN3_Init(uint16 Baud_Rate);
  uint8 CAN3_SendMsg(pCANFRAME sendFrame);
  uint8 CAN3_GetMsg(pCANFRAME receiveFrame);

#endif  




