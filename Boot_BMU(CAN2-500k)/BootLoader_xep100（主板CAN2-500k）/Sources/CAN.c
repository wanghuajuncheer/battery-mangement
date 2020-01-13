#include <string.h>
#include "Types.h"
#include "MC9S12XEP100.h"
#include "CAN.h"
#include "Srec.h"
/*******************************************************************************/

#define RxBufSize 80
UINT8 RxIn;               //next available location in the Rx queue
UINT8 RxOut;              //next character to be removed from the Rx queue

UINT8 RxBuff[RxBufSize];  //receive queue
volatile UINT8 RxBAvail;  //number of bytes left in the Rx queue


CAN_MSG can_msg;
CAN_MSG can_send;

/******************************************************************************/
   //从缓存区中取字
INT8 getchar(void)
{
 UINT8 c;                   //holds the character we'll return
 
 //if there's no characters in the Rx buffer wait here
 while (RxBAvail == RxBufSize);
 
 DisableInterrupts;
 
 //get a character from the Rx buffer & advance the Rx index
 c = RxBuff[RxOut++];
 if (RxOut == RxBufSize)              //index go past the physical end of the buffer?
    RxOut = 0;                //yes wrap around to the start
 RxBAvail++;                //1 more byte available in the receive buffer

 EnableInterrupts;
 return(c);     //return the character retrieved from receive buffer
}         

/******************************************************************************/
void INIT_CAN2(void) 
{
  RxIn = RxOut= 0;  //set the Rx & Tx queue indexes to zero
  RxBAvail = RxBufSize;     //the receive buffer is empty
  can_send.id = ID_Reply; 
  can_send.len = 1;
  can_send.RTR = FALSE;
  can_send.prty = 0;
   
  if(CAN2CTL0_INITRQ==0)      // 查询是否进入初始化状态   
    CAN2CTL0_INITRQ =1;        // 进入初始化状态

  while (CAN2CTL1_INITAK==0);  //等待进入初始化状态

  CAN2BTR0_SJW = 0;            //设置同步
  CAN2BTR0_BRP = 3;            //设置波特率,设定为:500kb

  
  CAN2BTR1 = 0x5c;     //总线频率为250kb/s=400000/(8*(1+5+1+12+1))
  CAN2IDAC=0x00;    //2个32位接收滤波器，滤波器0有效
                        /*  0b00000000
                         *    ||||||||__ bit0、1、2:标识符接收有效指示，0有效   
                         *    |||||||___| 
                         *    ||||||____| 
                         *    |||||_____  
                         *    ||||______ bit4、5:IDAM 标识符接收模式，2个32位或者4个16位 
                         *    |||_______| 
                         *    ||________ bit6: 
                         *    |_________ bit7: 
                         */
  CAN2IDAR0=0x00;    //只允许ID为0x001通过
  CAN2IDAR1=0x20; 
  CAN2IDAR2=0x00; 
  CAN2IDAR3=0x00;
 
  CAN2IDAR4=0x00; 
  CAN2IDAR5=0x20; 
  CAN2IDAR6=0x00; 
  CAN2IDAR7=0x00;   
// 打开滤波器                                  
  CAN2IDMR0 = 0x00;
  CAN2IDMR1 = 0x00;
  CAN2IDMR2 = 0x00;
  CAN2IDMR3 = 0x00;
  CAN2IDMR4 = 0x00;
  CAN2IDMR5 = 0x00;
  CAN2IDMR6 = 0x00;
  CAN2IDMR7 = 0x00;  

  CAN2CTL1 = 0xC0;             //使能MSCAN模块,设置为一般运行模式、使用总线时钟源 

  CAN2CTL0 = 0x00;             //返回一般模式运行

  while(CAN2CTL1_INITAK);      //等待回到一般运行模式

  while(CAN2CTL0_SYNCH==0);    //等待总线时钟同步

  CAN2RIER_RXFIE = 1;          //使能接收中断
}

/******************************************************************************/
Bool MSCAN2GetMsg(struct can_msg *msg)
{
  
  unsigned char sp2;

  // 检测接收标志
  if(!(CAN2RFLG_RXF))
    return(FALSE);
  
  // 检测 CAN协议报文模式 （一般/扩展） 标识符
  if(CAN2RXIDR1_IDE)
    // IDE = Recessive (Extended Mode)
    return(FALSE);

  // 读标识符
  msg->id = (unsigned int)(CAN2RXIDR0<<3) | 
            (unsigned char)(CAN2RXIDR1>>5);
  
  if(CAN2RXIDR1&0x10)
    msg->RTR = TRUE;
  else
    msg->RTR = FALSE;
  
  // 读取数据长度 
  msg->len = CAN2RXDLR;
  
  // 读取数据
  for(sp2 = 0; sp2 < msg->len; sp2++)
    msg->data[sp2] = *((&CAN2RXDSR0)+sp2);

  // 清 RXF 标志位 (缓冲器准备接收)
  CAN2RFLG = 0x01;

  return(TRUE);
}


/*************************************************************/
/*                       CAN1发送                            */
/*************************************************************/
Bool MSCAN2SendMsg(struct can_send msg)
{
  unsigned char send_buf, sp;
  
  // 检查数据长度
  if(msg.len > 8)
    return(FALSE);

  // 检查总线时钟
  if(CAN2CTL0_SYNCH==0)
    return(FALSE);

  send_buf = 0;
  do
  {
    // 寻找空闲的缓冲器
    CAN2TBSEL=CAN2TFLG;      
    send_buf=CAN2TBSEL;
  }
  while(!send_buf); 
  
  // 写入标识符
  CAN2TXIDR0 = (unsigned char)(msg.id>>3);
  CAN2TXIDR1 = (unsigned char)(msg.id<<5);
  
  if(msg.RTR)
    // RTR = 阴性（数据帧or远程帧）
    CAN2TXIDR1 |= 0x10;
    
  // 写入数据
  for(sp = 0; sp < msg.len; sp++)
    *((&CAN2TXDSR0)+sp) = msg.data[sp];
    
  // 写入数据长度
  CAN2TXDLR = msg.len; 
  
  // 写入优先级
  CAN2TXTBPR = msg.prty;
  
  // 清 TXx 标志 (缓冲器准备发送)
  CAN2TFLG = send_buf;
  
  return(TRUE);  //发送数据的状态 
  
}

/*********************************************************************************/
#pragma CODE_SEG RAM_CODE
interrupt void CAN2RxISR(void) 
{  
   
   char index;
   
   if(MSCAN2GetMsg(&can_msg)) //接收数据成功
   {
     if(can_msg.id == ID_BMU) //根据ID号进行解码CAN通信数据
     {
       
       for(index = 0;index < can_msg.len;index++)                                                    
         {
            if (RxBAvail != 0)            //if there are bytes available in the Rx buffer
               {
                  RxBAvail--;
                  RxBuff[RxIn++] = can_msg.data[index];      //place the received byte in the buffer
                  can_msg.data[index]=0;
                  if (RxIn == RxBufSize)      //reached the physical end of the buffer?
                  { 
                    RxIn = 0;                  //yes. wrap around to the start
                  }
                         
               } 
         }                      
     } 
   }
}
#pragma CODE_SEG DEFAULT
/******************************************************************************/
