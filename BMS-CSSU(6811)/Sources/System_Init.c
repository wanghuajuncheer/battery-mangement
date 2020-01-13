/*=======================================================================
 *Subsystem:   裸机
 *File:        System_Init.C
 *Author:      ZWB
 *Description: 通信：
               接口：
               波特率:
 ========================================================================
 * History:    修改历史记录列表
 * 1. Date:    
      Author:  
      Modification:            
========================================================================*/
#include  "Includes.h"                 /* 包含所有h文件，可以调用所有函数与变量 */ 



/*=======================================================================
 *函数名:      System_init(void) 
 *功能:        系统初始化函数
 *参数:        无       
 *返回：       无
 *说明：       所有底层函数的初始化，以及打开中断
========================================================================*/
void System_init(void) 
{
  
    DisableInterrupts;
    
    IVBR=0x7F;                         // 修改中断地址；
    ADC_init();
    PLL_Init();
    Init_Flash();

    SPI1_Init();                       // 电压温度收集spi通讯初始化;
    
    INIT_CAN2();
    
    PITInit(0,0,249,1279);             // PIT0计时10ms中断一次; 
    EnablePIT(0);                      // 使能PIT0的中断;
   // init_PIT0(); 
    
    //VoltageDetectInit();
    LTC6804_Init();                    // LTC6804初始化;  

    Create_task();                     // 初始化创建所有任务; 
    circuit_detection();
    
    Clear_Variable();
    
    DDRT=0XFF;
    PTT_PTT0 = 0;
    memset(&balance_receive,0,sizeof(balance_receive));

    EnableInterrupts;                  
    
}

/*=======================================================================
 *函数名:      delay_time(uint16 n)
 *功能:        延迟函数
 *参数:        无       
 *返回：       无
 *说明：       任意延迟函数
========================================================================*/
/*void delay_time(uint16 n)
{
    uint16 i;
    for(i = 0; i < n; i++);
}*/

/*=======================================================================
 *函数名:      Clear_Variable(void)
 *功能:        清零函数
 *参数:        无       
 *返回：       无
 *说明：       对一些结构体和变量进行清零；
========================================================================*/
void Clear_Variable(void)
{ 
    //memset(&Volt_Data_T,0,sizeof(Volt_Data_T));   
    //memset(&Task_Flag_Counter,0,sizeof(Task_Flag_Counter));   
    //memset(&Monitor_Stand,0,sizeof(Monitor_Stand));  
    //memset(&single_voltage,0,sizeof(single_voltage));
        
}
