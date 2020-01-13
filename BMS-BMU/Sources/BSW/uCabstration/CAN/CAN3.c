/*=======================================================================
 *Subsystem:   裸机
 *File:        CAN3.C
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
  #include  "CAN.h"
  #include  "MC9S12XEP100.h"
/*=======================================================================
 *函数名:      CAN3_Init(void)
 *功能:        初始化CAN3
 *参数:        
               sysclk：时间总线时钟:32MHz
               baud:    
 *返回：       无
 *说明：       
========================================================================*/
uint8 CAN3_Init(uint16 Baud_Rate) 
{
  if((Baud_Rate != 125)||(Baud_Rate != 250)||(Baud_Rate != 500))
  {
    return(Init_Fault_CAN_BaudRate);
  }
  
  if(CAN3CTL0_INITRQ==0)      // 查询是否进入初始化状态   
  {
    CAN3CTL0_INITRQ =1;        // 进入初始化状态
  }
 
  while (CAN3CTL1_INITAK==0);  //等待进入初始化状态

  CAN3BTR0_SJW = 0;            //设置同步
  
  switch(Baud_Rate)
  {
    case 500:
      CAN3BTR0_BRP = 3;            //设置预分频值 
      CAN3BTR1 = 0x1c;     //设置时段1和时段2的Tq个数 
      break;
    
    case 250:
      CAN3BTR0_BRP = 7;            //设置预分频值  
      CAN3BTR1 = 0x1c;     //设置时段1和时段2的Tq个数 
      break;
      
    case 125:
      CAN3BTR0_BRP = 15;            //设置预分频值  
      CAN3BTR1 = 0x1c;     //设置时段1和时段2的Tq个数 
      break;
  }
 
//关闭滤波器                                  
  CAN3IDMR0 = 0xFF;
  CAN3IDMR1 = 0xFF;
  CAN3IDMR2 = 0xFF;
  CAN3IDMR3 = 0xFF;
  CAN3IDMR4 = 0xFF;
  CAN3IDMR5 = 0xFF;
  CAN3IDMR6 = 0xFF;
  CAN3IDMR7 = 0xFF; 

  CAN3CTL1 = 0xC0;             //使能MSCAN模块,设置为一般运行模式、使用总线时钟源 

  CAN3CTL0 = 0x00;             //返回一般模式运行

  while(CAN3CTL1_INITAK);      //等待回到一般运行模式

  while(CAN3CTL0_SYNCH==0);    //等待总线时钟同步
  
  CAN3RIER_RXFIE = 1;          //使能接收中断
  
  return (Init_Normal_CAN);
}

/*=======================================================================
 *函数名:      CAN3_SendMsg
 *功能:        CAN3发送数据
 *参数:        扩展帧
               sysclk：时间总线时钟:32MHz
               baud:   
 *返回：       无
 *说明：       
========================================================================*/ 
uint8 CAN3_SendMsg(pCANFRAME sendFrame)
{
  uint8 send_buf,i;
  uint8 Cnt[1];
  
  // 检查数据长度
  if(sendFrame->m_dataLen > 8)
    return (SendMsg_Fault_Lenth);

  // 检查总线时钟
  if(CAN3CTL0_SYNCH==0)
    return (SendMsg_Fault_Synch);

  send_buf = 0;
  do
  {
    // 寻找空闲的缓冲器
    CAN3TBSEL=CAN3TFLG;
    send_buf=CAN3TBSEL;
    if(++Cnt[0]>=200)
    {
      Cnt[0] = 0;
      return(SendMsg_Fault_NoEmptyNode);
    }
  } 
  while(!send_buf); 
  //写入标识符ID
  
  if (sendFrame->m_IDE == 0)  //按标准帧填充ID
  {
    CAN3TXIDR0 = (uint8)(sendFrame->m_ID>>3);
    CAN3TXIDR1 = (uint8)(sendFrame->m_ID<<5);
  } 
  else  //按扩展帧填充ID
  {
    CAN3TXIDR0 = (uint8)(sendFrame->m_ID>>21);
    CAN3TXIDR1 = (((uint8)(sendFrame->m_ID>>13)) & 0xe0)|0x18|(((uint8)(sendFrame->m_ID>>15)) &0x07);
    CAN3TXIDR2 = (uint8)(sendFrame->m_ID>>7);
    CAN3TXIDR3 = (uint8)(sendFrame->m_ID<<1);
  }
  
  if(sendFrame->m_RTR==1)
  {     
      CAN3TXIDR1 |= 0x10;
  }
      
  for (i=0;i<sendFrame->m_dataLen;++i)  
  {
    *((&CAN3TXDSR0) + i) = sendFrame->m_data[i];
  } 
      
  // 写入数据长度
  CAN3TXDLR = sendFrame->m_dataLen;
  
  // 写入优先级
  CAN3TXTBPR = sendFrame->m_priority;
  
  // 清 TXx 标志 (缓冲器准备发送)
  CAN3TFLG = send_buf;
  
  return(SendMsg_Normal);
  
}

/*=======================================================================
 *函数名:      CAN3_GetMsg(pCANFRAME receiveFrame)
 *功能:        CAN3接收数据
 *参数:        扩展帧
               sysclk：时间总线时钟:32MHz
               baud:  
 *返回：       无
 *说明：       
========================================================================*/ 
uint8 CAN3_GetMsg(pCANFRAME receiveFrame)
{
  
  uint8 i;
	  
	if (!(CAN3RFLG_RXF))                         // 检测接收标志位；
		return (GetMsg_Fault_RFLG_RXF);
	 
	if (CAN3RXIDR1_IDE == 0)                     // 收到标准帧；
	{
		receiveFrame->m_ID = (uint32)(CAN3RXIDR0<<3) | (uint32)(CAN3RXIDR1>>5);
		receiveFrame->m_RTR = (CAN3RXIDR1>>4) & 0x01;
    receiveFrame->m_IDE = 0;
	} 
	else                                         // 收到扩展帧；
	{
	 	receiveFrame->m_ID = (((uint32)CAN3RXIDR0)<<21)|((uint32)(CAN3RXIDR1&0xe0)<<13)|((uint32)(CAN3RXIDR1&0x07)<<15)|(((uint32)CAN3RXIDR2)<<7);
    receiveFrame->m_ID = receiveFrame->m_ID | ((uint32)(CAN3RXIDR3&0xfe)>>1);
	}
	 
  receiveFrame->m_dataLen = CAN3RXDLR&0X0F;
  
  for (i=0;i<receiveFrame->m_dataLen;i++)       // 读取数据
  {
	   receiveFrame->m_data[i] = *(&(CAN3RXDSR0)+i);
  }   

  CAN3RFLG_RXF = 1;

  return(GetMsg_Normal);
}




