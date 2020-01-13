/*=======================================================================
 *Subsystem:   裸机
 *File:        CAN1.C
 *Author:      WenYuhao
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
static void CAN1_GetMsg_Process(pCANFRAME receiveFrame);   
/*=======================================================================
 *函数名:      CAN1_Init(void)
 *功能:        初始化CAN1
 *参数:        
               sysclk：时间总线时钟:32MHz
               baud:    
 *返回：       无
 *说明：       CAN1充电
========================================================================*/
uint8 CAN1_Init(uint16 Baud_Rate) 
{
  uint16 CAN1cnt[3] = {0,0,0};
  
  if((Baud_Rate != 125)&&(Baud_Rate != 250)&&(Baud_Rate != 500))
  {
    return(Init_Fault_CAN_BaudRate);
  }
  
  if(CAN1CTL0_INITRQ == 0)      // 查询是否进入初始化状态   
  {
    CAN1CTL0_INITRQ = 1;        // 进入初始化状态
  }

  do
  {
    if(++CAN1cnt[0]>3000)
    {
      return(Init_Fault_CAN_Unready1);
    }
  }
  while (CAN1CTL1_INITAK==0);   //等待进入初始化状态
  CAN1cnt[0] = 0;
  
  CAN1BTR0_SJW = 0;             //设置同步
  
  switch(Baud_Rate)
  {
    case 500:
      CAN1BTR0_BRP = 3;            //设置预分频值 
      CAN1BTR1 = 0x1c;     //设置时段1和时段2的Tq个数 
      break;
    
    case 250:
      CAN1BTR0_BRP = 7;            //设置预分频值  
      CAN1BTR1 = 0x1c;     //设置时段1和时段2的Tq个数 
      break;
      
    case 125:
      CAN1BTR0_BRP = 15;            //设置预分频值  
      CAN1BTR1 = 0x1c;     //设置时段1和时段2的Tq个数 
      break;
  }
 
//关闭滤波器                                  
  CAN1IDMR0 = 0xFF;
  CAN1IDMR1 = 0xFF;
  CAN1IDMR2 = 0xFF;
  CAN1IDMR3 = 0xFF;
  CAN1IDMR4 = 0xFF;
  CAN1IDMR5 = 0xFF;
  CAN1IDMR6 = 0xFF;
  CAN1IDMR7 = 0xFF; 

  CAN1CTL1 = 0xC0;             //使能MSCAN模块,设置为一般运行模式、使用总线时钟源 

  CAN1CTL0 = 0x00;
               //返回一般模式运行
  do
  {
    if(++CAN1cnt[1]>3000)
    {
      return(Init_Fault_CAN_Unready2);
    }
  }
  while(CAN1CTL1_INITAK);      //等待回到一般运行模式
  CAN1cnt[1] = 0;
  
  do
  {
    if(++CAN1cnt[2]>3000)
    {
      return(Init_Fault_CAN_Synchr);
    }
  }
  while(CAN1CTL0_SYNCH==0);    //等待总线时钟同步
  CAN1cnt[2] = 0;
  
  CAN1RIER_RXFIE = 1;          //使能接收中断
  
  return (Init_Normal_CAN);
}

/*=======================================================================
 *函数名:      CAN1_SendMsg
 *功能:        CAN1发送数据
 *参数:        扩展帧
               sysclk：时间总线时钟:32MHz
               baud:   
 *返回：       无
 *说明：       
========================================================================*/ 
uint8 CAN1_SendMsg(pCANFRAME sendFrame)
{
  uint8 send_buf,i;
  uint16 Cnt=0;
  
  // 检查数据长度
  if(sendFrame->m_dataLen > 8)
    return (SendMsg_Fault_Lenth);

  // 检查总线时钟
  if(CAN1CTL0_SYNCH==0)
    return (SendMsg_Fault_Synch);

  send_buf = 0;
  do
  {
    // 寻找空闲的缓冲器
    CAN1TBSEL=CAN1TFLG;
    send_buf=CAN1TBSEL;
    Cnt++;
  } 
  while((!send_buf)&&(Cnt<3000)); 
  //写入标识符ID
  
  if (sendFrame->m_IDE == 0)  //按标准帧填充ID
  {
    CAN1TXIDR0 = (uint8)(sendFrame->m_ID>>3);
    CAN1TXIDR1 = (uint8)(sendFrame->m_ID<<5);
  } 
  else  //按扩展帧填充ID
  {
    CAN1TXIDR0 = (uint8)(sendFrame->m_ID>>21);
    CAN1TXIDR1 = (((uint8)(sendFrame->m_ID>>13)) & 0xe0)|0x18|(((uint8)(sendFrame->m_ID>>15)) &0x07);
    CAN1TXIDR2 = (uint8)(sendFrame->m_ID>>7);
    CAN1TXIDR3 = (uint8)(sendFrame->m_ID<<1);
  }
  
  if(sendFrame->m_RTR==1)
  {     
      CAN1TXIDR1 |= 0x10;
  }
      
  for (i=0;i<sendFrame->m_dataLen;++i)  
  {
    *((&CAN1TXDSR0) + i) = sendFrame->m_data[i];
  } 
      
  // 写入数据长度
  CAN1TXDLR = sendFrame->m_dataLen;
  
  // 写入优先级
  CAN1TXTBPR = sendFrame->m_priority;
  
  // 清 TXx 标志 (缓冲器准备发送)
  CAN1TFLG = send_buf;
  
  return(SendMsg_Normal);
  
}

/*=======================================================================
 *函数名:      CAN1_GetMsg(pCANFRAME receiveFrame)
 *功能:        CAN1接收数据
 *参数:        扩展帧
               sysclk：时间总线时钟:32MHz
               baud:  
 *返回：       无
 *说明：       
========================================================================*/ 
uint8 CAN1_GetMsg(pCANFRAME receiveFrame)
{
  uint8 i;
	  
	if (!(CAN1RFLG_RXF))                         // 检测接收标志位；
		return (GetMsg_Fault_RFLG_RXF);
	 
	if (CAN1RXIDR1_IDE == 0)                     // 收到标准帧；
	{
		receiveFrame->m_ID = (uint32)(CAN1RXIDR0<<3) | (uint32)(CAN1RXIDR1>>5);
		receiveFrame->m_RTR = (CAN1RXIDR1>>4) & 0x01;
    receiveFrame->m_IDE = 0;
	} 
	else                                         // 收到扩展帧；
	{
	 	receiveFrame->m_ID = (((uint32)CAN1RXIDR0)<<21)|((uint32)(CAN1RXIDR1&0xe0)<<13)|((uint32)(CAN1RXIDR1&0x07)<<15)|(((uint32)CAN1RXIDR2)<<7);
    receiveFrame->m_ID = receiveFrame->m_ID | ((uint32)(CAN1RXIDR3&0xfe)>>1);
	}
	 
  receiveFrame->m_dataLen = CAN1RXDLR&0X0F;
  
  for (i=0;i<receiveFrame->m_dataLen;i++)       // 读取数据
  {
	   receiveFrame->m_data[i] = *(&(CAN1RXDSR0)+i);
  } 
  
  CAN1_GetMsg_Process(receiveFrame);

  CAN1RFLG_RXF = 1;

  return(GetMsg_Normal);
}

/*=======================================================================
 *函数名:      CAN1_GetMsg_Process(pCANFRAME receiveFrame)
 *功能:        CAN2接收数据时的处理方法
 *参数:        扩展帧
 *返回：       无
 
 *说明：       
========================================================================*/
static
void CAN1_GetMsg_Process(pCANFRAME receiveFrame)
{
  switch(receiveFrame->m_ID) 
  {
    case 0x112:
      CAN_ChargetoBMS(receiveFrame);
      HeartBeat.HeartBeat_Charge = 1;
    break;
  }
}

/*=======================================================================
 *函数名:      Interrupt_CAN1(void)
 *功能:        CAN1接收中断函数
 *参数:        扩展帧
 *返回：       无
 *说明：       CAN1的接收中断函数
========================================================================*/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
//CAN1接收充电机数据 
interrupt void Interrupt_CAN1()            
{     
    CANFRAME mgGet1;
    //Task_Flag_Time.CAN1++;    
    if (CAN1_GetMsg(&mgGet1));
} 
#pragma CODE_SEG DEFAULT



