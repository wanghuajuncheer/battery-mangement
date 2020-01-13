/*=======================================================================
 *Subsystem:   裸机
 *File:        CAN2.C
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
  
/*=======================================================================
 *函数名:      CAN2_Init(void)
 *功能:        初始化CAN2
 *参数:        
               sysclk：时间总线时钟:32MHz
               baud:    
 *返回：       无
 *说明：       
========================================================================*/
uint8 CAN2_Init(uint16 Baud_Rate) 
{
  uint16 CAN2cnt[3] = {0,0,0};
  
  if((Baud_Rate != 125)&&(Baud_Rate != 250)&&(Baud_Rate != 500))
  {
    return(Init_Fault_CAN_BaudRate);
  }
  
  if(CAN2CTL0_INITRQ==0)       // 查询是否进入初始化状态   
  {
    CAN2CTL0_INITRQ = 1;       // 进入初始化状态
  }
  
  do
  {
    if(++CAN2cnt[0]>3000)
    {
      return(Init_Fault_CAN_Unready1);
    }
  }
  while (CAN2CTL1_INITAK==0);  //等待进入初始化状态
  CAN2cnt[0] = 0;
  
  CAN2BTR0_SJW = 0;            //设置同步
  
  switch(Baud_Rate)
  {
    case 500:
      CAN2BTR0_BRP = 3;        //设置预分频值 
      CAN2BTR1 = 0x1c;     //设置时段1和时段2的Tq个数 
      break;
    
    case 250:
      CAN2BTR0_BRP = 7;            //设置预分频值  
      CAN2BTR1 = 0x1c;     //设置时段1和时段2的Tq个数 
      break;
      
    case 125:
      CAN2BTR0_BRP = 15;            //设置预分频值  
      CAN2BTR1 = 0x1c;     //设置时段1和时段2的Tq个数 
      break;
  } 
 
//关闭滤波器                                  
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
  
  do
  {
    if(++CAN2cnt[1]>3000)
    {
      return(Init_Fault_CAN_Unready2);
    }
  }
  while(CAN2CTL1_INITAK);      //等待回到一般运行模式
  CAN2cnt[1] = 0;
  
  do
  {
    if(++CAN2cnt[2]>3000)
    {
      return(Init_Fault_CAN_Synchr);
    }
  }
  while(CAN2CTL0_SYNCH==0);    //等待总线时钟同步
  CAN2cnt[2] = 0;
  
  CAN2RIER_RXFIE = 1;          //使能接收中断
  
  return (Init_Normal_CAN);
}

/*=======================================================================
 *函数名:      CAN2_SendMsg
 *功能:        CAN2发送数据
 *参数:        扩展帧
               sysclk：时间总线时钟:32MHz
               baud:   
 *返回：       无
 *说明：       
========================================================================*/ 
uint8 CAN2_SendMsg(pCANFRAME sendFrame)
{
  uint8  send_buf,i;
  uint16  Cnt=0;
  
  // 检查数据长度
  if(sendFrame->m_dataLen > 8)
    return (SendMsg_Fault_Lenth);

  // 检查总线时钟
  if(CAN2CTL0_SYNCH==0)
    return (SendMsg_Fault_Synch);

  send_buf = 0;
  do
  {
    // 寻找空闲的缓冲器
    Cnt++;
    CAN2TBSEL=CAN2TFLG;
    send_buf=CAN2TBSEL;
  } 
  while((!send_buf)&&(Cnt<3000)); 
  //写入标识符ID
  
  if (sendFrame->m_IDE == 0)  //按标准帧填充ID
  {
    CAN2TXIDR0 = (uint8)(sendFrame->m_ID>>3);
    CAN2TXIDR1 = (uint8)(sendFrame->m_ID<<5);
  } 
  else  //按扩展帧填充ID
  {
    CAN2TXIDR0 = (uint8)(sendFrame->m_ID>>21);
    CAN2TXIDR1 = (((uint8)(sendFrame->m_ID>>13)) & 0xe0)|0x18|(((uint8)(sendFrame->m_ID>>15))&0x07);
    CAN2TXIDR2 = (uint8)(sendFrame->m_ID>>7);
    CAN2TXIDR3 = (uint8)(sendFrame->m_ID<<1);
  }
  
  if(sendFrame->m_RTR==1)
  {     
    CAN2TXIDR1 |= 0x10;
  }
      
  for (i=0; i<sendFrame->m_dataLen; i++)  
  {
    *((&CAN2TXDSR0) + i) = sendFrame->m_data[i];
  } 
      
  // 写入数据长度
  CAN2TXDLR = sendFrame->m_dataLen;
  
  // 写入优先级
  CAN2TXTBPR = sendFrame->m_priority;
  
  // 清 TXx 标志 (缓冲器准备发送)
  CAN2TFLG = send_buf;
  
  return(SendMsg_Normal);
  
}

/*=======================================================================
 *函数名:      CAN2_GetMsg(pCANFRAME receiveFrame)
 *功能:        CAN2接收数据
 *参数:        扩展帧
               sysclk：时间总线时钟:32MHz
               baud:  
 *返回：       无
 *说明：       
========================================================================*/ 
uint8 CAN2_GetMsg(pCANFRAME receiveFrame)
{
  
  uint8 i;
	  
	if (!(CAN2RFLG_RXF))                         // 检测接收标志位；
		return (GetMsg_Fault_RFLG_RXF);
	 
	if (CAN2RXIDR1_IDE == 0)                     // 收到标准帧；
	{
		receiveFrame->m_ID = (uint32)(CAN2RXIDR0<<3) | (uint32)(CAN2RXIDR1>>5);
		receiveFrame->m_RTR = (CAN2RXIDR1>>4) & 0x01;
    receiveFrame->m_IDE = 0;
	} 
	else                                         // 收到扩展帧；
	{
	 	receiveFrame->m_ID = (((uint32)CAN2RXIDR0)<<21)|((uint32)(CAN2RXIDR1&0xe0)<<13)|((uint32)(CAN2RXIDR1&0x07)<<15)|(((uint32)CAN2RXIDR2)<<7);
    receiveFrame->m_ID = receiveFrame->m_ID | ((uint32)(CAN2RXIDR3&0xfe)>>1);
	}
	
	if(CAN3RXIDR3 & 0x01)
    receiveFrame->m_RTR = TRUE;
  else
    receiveFrame->m_RTR = FALSE;    
  
  
  receiveFrame->m_dataLen = CAN2RXDLR&0X0F;
  
  for (i=0;i<receiveFrame->m_dataLen;i++)       // 读取数据
  {
	   receiveFrame->m_data[i] = *(&(CAN2RXDSR0)+i);
  }   

  CAN2_GetMsg_Process(receiveFrame);

  CAN2RFLG_RXF = 1;

  return(GetMsg_Normal);
}

/*=======================================================================
 *函数名:      CAN2_GetMsg_Process(pCANFRAME receiveFrame)
 *功能:        CAN2接收数据时的处理方法
 *参数:        扩展帧
 *返回：       无
 
 *说明：       
========================================================================*/
void CAN2_GetMsg_Process(pCANFRAME receiveFrame)
{
  switch(receiveFrame->m_ID) 
  {
    case Boot_ID://0xF300:
      HeartBeat.HeartBeat_CSSU1 = 1;//对子板进行程序升级的时候需要屏蔽掉线的故障
      if(receiveFrame->m_data[1] == 0xAA)
      {
        if(receiveFrame->m_data[0] == BMU_IDNUM)
        {
           Boot_Data.OnlineUpdateCheck = 1;
        }           
      }
    break; 
       
    case 0x002:
    case 0x003:
    case 0x004:
    case 0x005:
    case 0x006:
    case 0x007:
    case 0x008:
    case 0x009:
       HeartBeat.HeartBeat_CSSU1 = 1;//对子板进行程序升级的时候需要屏蔽掉线的故障
    break;
        
    default:             //接收上位机和内网的报文
      if((receiveFrame->m_ID)>>24 == 0x18)//内网报文 
      {
        HeartBeat.HeartBeat_CSSU1 = 1;
        DataFromCSSU(receiveFrame);
      } 
      else if((receiveFrame->m_ID)>>24 == 0x19)//标定报文
      {            
        if((receiveFrame->m_ID)>>8 == 0x19FF99)
        {
          DataFromCSSU(receiveFrame); 
          HeartBeat.HeartBeat_CSSU1 = 1;
        }
        else 
        {
          UpMonitor_to_Bms(receiveFrame);
        }
      } 
    break;
  }
}
/*=======================================================================
 *函数名:      Interrupt_CAN2(void)
 *功能:        CAN2接收中断函数
 *参数:        扩展帧
 *返回：       无
 *说明：       CAN2的接收中断函数
========================================================================*/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
//CAN1接收内网数据 
interrupt void Interrupt_CAN2()            
{     
    CANFRAME mgGet2;
    //Task_Flag_Time.CAN1++;    
    if (CAN2_GetMsg(&mgGet2));
} 
#pragma CODE_SEG DEFAULT

















