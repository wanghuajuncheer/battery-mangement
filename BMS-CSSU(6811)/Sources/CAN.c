#include "Includes.h"



/*************************************************************/
/*                        初始化CAN2                        */
/*************************************************************/
void INIT_CAN2(void) 
{
  
   //Task_Die |=0x00000010;
  
  if(CAN2CTL0_INITRQ==0)      // 查询是否进入初始化状态   
    CAN2CTL0_INITRQ =1;        // 进入初始化状态

  while (CAN2CTL1_INITAK==0);  //等待进入初始化状态

  CAN2BTR0_SJW = 0;            //设置同步
  CAN2BTR0_BRP = 3;            //设置波特率  

  
  CAN2BTR1 = 0x1c;     //设置时段1和时段2的Tq个数 ,总线频率为250kb/s

// 关闭滤波器                                  
  CAN2IDMR0 = 0xFF;
  CAN2IDMR1 = 0xFF;
  CAN2IDMR2 = 0xFF;
  CAN2IDMR3 = 0xFF;
  CAN2IDMR4 = 0xFF;
  CAN2IDMR5 = 0xFF;
  CAN2IDMR6 = 0xFF;
  CAN2IDMR7 = 0xFF; 

  CAN2CTL1 = 0xC0;             //使能MSCAN模块,设置为一般运行模式、使用总线时钟源 

  CAN2CTL0 = 0x00;             //返回一般模式运行

  while(CAN2CTL1_INITAK);      //等待回到一般运行模式

  while(CAN2CTL0_SYNCH==0);    //等待总线时钟同步

  CAN2RIER_RXFIE = 1;          //使能接收中断
  
  //Task_Die &=0xFFFFFFEF;
}


/*************************************************************/
/*                        CAN2发送                           */
/*************************************************************/
Bool MSCAN2SendMsg(pCANFRAME sendFrame)
{
  uint8 send_buf,i;
  uint16 Counter = 0;
  
  // 检查数据长度
  if(sendFrame->m_dataLen > 8)
    return 1;

  // 检查总线时钟
  if(CAN2CTL0_SYNCH==0)
    return 2;

  send_buf = 0;
  do
  {
    // 寻找空闲的缓冲器
    CAN2TBSEL=CAN2TFLG;
    send_buf=CAN2TBSEL;
    
    Counter++;
  }  
  while( (!send_buf)&&(Counter<2000)); 
  //写入标识符ID
  
  if (sendFrame->m_IDE == 0)  //按标准帧填充ID
  {
    CAN2TXIDR0 = (uint8)(sendFrame->m_ID>>3);
    CAN2TXIDR1 = (uint8)(sendFrame->m_ID<<5);
  } 
  else  //按扩展帧填充ID
  {
    CAN2TXIDR0 = (uint8)(sendFrame->m_ID>>21);
    CAN2TXIDR1 = (((uint8)(sendFrame->m_ID>>13)) & 0xe0)|0x18 | (((uint8)(sendFrame->m_ID>>15)) &0x07);
    CAN2TXIDR2 = (uint8)(sendFrame->m_ID>>7);
    CAN2TXIDR3 = (uint8)(sendFrame->m_ID<<1);
  }
  
  
  if(sendFrame->m_RTR==1)
  { 
      // RTR = 阴性
      CAN2TXIDR1 |= 0x10;
  }

  if (sendFrame->m_RTR == 0)  //按数据帧填充
  {
    for (i=0;i<sendFrame->m_dataLen;++i)  
    {
      *((&CAN2TXDSR0) + i) = sendFrame->m_data[i];
    }
  }
  else   //按远程帧填充
  {
    CAN2TXDLR = 0;
  }
    
  // 写入数据长度
  CAN2TXDLR = sendFrame->m_dataLen;
  
  // 写入优先级
  CAN2TXTBPR = sendFrame->m_priority;
  
  // 清 TXx 标志 (缓冲器准备发送)
  CAN2TFLG = send_buf;
  
  return 0;
  
}
/*************************************************************/
/*                        CAN2接收                           */
/*************************************************************/
Bool MSCAN2GetMsg(pCANFRAME receiveFrame)
{
  uint8 i,j;
  uint8 retu_value;
	  
	if (!(CAN2RFLG_RXF))                         // 检测接收标志位；
		return 1;
	  
	if (CAN2RXIDR1_IDE == 0)                     // 收到标准帧；
	{
		receiveFrame->m_ID = (uint32)(CAN2RXIDR0<<3) | (uint32)(CAN2RXIDR1>>5);
		receiveFrame->m_RTR = (CAN2RXIDR1>>4) & 0x01;
    receiveFrame->m_IDE = 0;
	} 
	else                                         // 收到扩展帧；
	{
		receiveFrame->m_ID = (((uint32)CAN2RXIDR0)<<21)|((uint32)(CAN2RXIDR1&0xe0)<<13)| \
		((uint32)(CAN2RXIDR1&0x07)<<15)|(((uint32)CAN2RXIDR2)<<7)| \
		((uint32)(CAN2RXIDR3&0xfe)>>1);
	}
  receiveFrame->m_dataLen = CAN2RXDLR_DLC&0X0F;

  for (i=0;i<receiveFrame->m_dataLen;i++)       // 读取数据
  {
	  receiveFrame->m_data[i]= *(&(CAN2RXDSR0)+i);
	   
  }
      
      
  switch(receiveFrame->m_ID)
  {
    case Boot_ID://0xF300
      if(receiveFrame->m_data[1] == 0xAA)
      {
          if(receiveFrame->m_data[0] == (CSSU_IDNUM&0x01))
          {
             Boot_Data.OnlineUpdateCheck = 1;
          }           
      }
    break;

    case 0x18FF9700:
      balance_receive.BalanceOn = receiveFrame->m_data[0];
      BMU_OffLineState.BMU_Life = 1;  //life信号
    break;
    
    case 0x18FF9900:
      balance_receive.total_volt = (receiveFrame->m_data[5] + ((uint16)receiveFrame->m_data[6]<<8) + ((uint32)receiveFrame->m_data[7]<<16)); 
    break;
    
    default:
    break;     
  }
  

  CAN2RFLG_RXF = 1;

  return 0;
}
