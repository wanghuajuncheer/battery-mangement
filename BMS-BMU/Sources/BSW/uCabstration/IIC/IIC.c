/*=======================================================================
 *Subsystem:   裸机
 *File:        IIC.C
 *Author:      Wenming
 *Description: 通信：
               接口：
               波特率：
 ========================================================================
 * History:    修改历史记录列表
 * 1. Date:
      Author:
      Modification:
========================================================================*/
#include  "IIC.h"


/*=======================================================================
 *函数名:      uint8 IIC_Init(void) 
 *功能:        IIC初始化函数
 *参数:        无       
 *返回：       无
 
 *说明：       
========================================================================*/
uint8 IIC_Init(void) 
{
  IIC0_IBFD = 0x94;   //总线时钟32MHz,设置SCL主频为100KHz
  IIC0_IBCR = 0x80;   //使能IIC模块,禁止中断
  IIC0_IBSR_IBAL = 1; //清除IBAL标志位  
  
  return(Init_Normal_IIC);
}

/* IIC器件的写函数 */
void IIC_write(uint8 addr,uint8 writeaddr,uint8 data) 
{   
  
  IIC0_IBCR_TXAK = 0;               // 接收到数据后有应答
  IIC0_IBCR_TX_RX = 1;              // 设置单片机为发送模式
  IIC0_IBCR_MS_SL = 1;              // 设置单片机为主机模式，产生开始信号

  IIC0_IBDR = addr;
  while(IIC0_IBSR_IBIF == 0); 
  IIC0_IBSR_IBIF = 1;
  while(IIC0_IBSR_RXAK);
  
  IIC0_IBDR = writeaddr;
  while(IIC0_IBSR_IBIF == 0); 
  IIC0_IBSR_IBIF = 1;
  while(IIC0_IBSR_RXAK);
  
  IIC0_IBDR = data;
  while(IIC0_IBSR_IBIF == 0);
  IIC0_IBSR_IBIF = 1;
  while(IIC0_IBSR_RXAK);

  IIC0_IBCR_MS_SL = 0;
}

/*IIC器件的读函数*/
uint8 IIC_read(uint8 addr,uint8 readaddr) 
{
  uint8 data;
  IIC0_IBCR_TXAK = 0;               // 接收到数据后有应答
  IIC0_IBCR_TX_RX = 1;              // 设置单片机为发送模式
  IIC0_IBCR_MS_SL = 1;              // 设置单片机为主机模式，产生开始信号

  IIC0_IBDR = addr;                 // 发送从器件的地址模式为写；
  while(IIC0_IBSR_IBIF == 0); 
  IIC0_IBSR_IBIF = 1;
  while(IIC0_IBSR_RXAK);
  
  IIC0_IBDR = readaddr;             // 发送存储器地址
  while(IIC0_IBSR_IBIF == 0); 
  IIC0_IBSR_IBIF = 1;
  while(IIC0_IBSR_RXAK);
  
  IIC0_IBCR_RSTA=1;                  
  
  IIC0_IBCR_TXAK = 0;               // 接收到数据后有应答
  IIC0_IBCR_TX_RX = 1;              // 设置单片机为发送模式
  IIC0_IBCR_MS_SL = 1;              // 设置单片机为主机模式，产生开始信号

  IIC0_IBDR = addr+1;               // 发送从器件的地址模式为读，所以加1；
  while(IIC0_IBSR_IBIF == 0); 
  IIC0_IBSR_IBIF = 1;
  while(IIC0_IBSR_RXAK);

  IIC0_IBCR_TX_RX = 0;             // 设置单片机为接收模式
  IIC0_IBCR_TXAK = 1;              // 接收到数据后无应答

  data = IIC0_IBDR;                // 清空IICD寄存器，准备接收；

  while(IIC0_IBSR_IBIF == 0); 
  IIC0_IBSR_IBIF = 1;
  IIC0_IBCR_MS_SL = 0;

  data = IIC0_IBDR;                // 读取接收到的数据
  return(data);
}

  