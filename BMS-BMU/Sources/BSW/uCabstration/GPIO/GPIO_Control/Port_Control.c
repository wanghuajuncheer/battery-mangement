/*=======================================================================
 *Subsystem:   裸机
 *File:        Port_Control.C
 *Author:      WENMING
 *Description: 通信：
               接口：
               波特率：
 ========================================================================
 * History:    修改历史记录列表
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#include  "Port_Control.h"             //指示灯驱动程序头文件
/*=======================================================================
 *函数名:       Light_Init(uint16 port,uint8 name,uint8 state)
 *功能:         初始化指示灯状态 
 *参数:         port:端口名;                                        
                name:指定端口引脚号;                                    
                state:初始状态,1=高电平,0=低电平
 *返回：        Init_state
 *说明：       
========================================================================*/
uint8 Light_Init(uint16 port,uint8 name,uint8 state)   
{
   uint8 Init_state;
   Init_state = GPIO_Init(port,name,1,state);    //初始化指示灯
   return(Init_state);
}

/*=======================================================================
 *函数名:       Light_Control(uint16 port,uint8 name,uint8 state)
 *功能:         初始化指示灯状态 
 *参数:         port:端口名;                                        
                name:指定端口引脚号;                                    
                state:初始状态,1=高电平,0=低电平
 *返回：        无
 *说明：       
========================================================================*/ 
void Light_Control(uint16 port,uint8 name,uint8 state)
{
    GPIO_Set(port,PRT,name,state);   //控制引脚状态     
} 

/*=======================================================================
 *函数名:       Light_Change(uint16 port,uint8 name)
 *功能:         状态切换，原来[暗]，则变[亮]；原来[亮]，则变[暗] 
 *参数:         port:端口名;                                        
                name:指定端口引脚号;                                    
 *返回：        无
 *说明：       
========================================================================*/ 
void Light_Change(uint16 port,uint8 name)
{
   if(GPIO_Get(port,PRT,name)==Light_ON)   //若原来为"亮",则变"暗"
     GPIO_Set(port,PRT,name,Light_OFF);
   else                                    //若原来为"暗",则变"亮"
     GPIO_Set(port,PRT,name,Light_ON);
}

/*=======================================================================
 *函数名:      Light_StateGet(uint16 port,uint8 pin)
 *功能:        light状态获取
 *参数:        port:Port通道类型
               pin:通道编号
 *返回：       state:继电器状态
 
 *说明：       
========================================================================*/  
uint8 Light_StateGet(uint16 port,uint8 pin)
{
   uint8 state;
   state = GPIO_Get(port,1,pin);
   return(state);
}



//***********************************继电器***************************************//
/*=======================================================================
 *函数名:      Port_Init(uint16 port,uint8 name,uint8 state)
 *功能:        继电器初始化函数
 *参数:        无       
 *返回：       无
 
 *说明：       
========================================================================*/ 
uint8 Port_Init(uint16 port,uint8 name,uint8 state) 
{
   uint8 Init_state;
   Init_state = GPIO_Init(port,name,1,state);    //初始化继电器 
   return(Init_state);
}

/*=======================================================================
 *函数名:      Port_Control(uint16 port,uint8 name,uint8 state)
 *功能:        继电器控制函数
 *参数:        无       
 *返回：       无
 
 *说明：       
========================================================================*/ 
void Port_Control(uint16 port,uint8 name,uint8 state)
{
    GPIO_Set(port,PRT,name,state);   //控制继电器状态     
} 

/*=======================================================================
 *函数名:      Port_StateGet(uint16 port,uint8 pin)
 *功能:        继电器状态获取
 *参数:        无       
 *返回：       无
 
 *说明：       
========================================================================*/ 
uint8 Port_StateGet(uint16 port,uint8 pin)
{
   uint8 state;
   state = GPIO_Get(port,0,pin);
   return(state);
}




