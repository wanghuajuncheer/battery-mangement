/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_Volt_Process.C
 *Author:      ZWB
 *Description: 通信：
               接口：
               波特率：
               调试：观察总压与高压检测总压，最大最小单体与压差；
 ========================================================================
 * History:    修改历史记录列表
 * 1. Date:    2017 - 11 -10            
      Author:  ZWB
      Modification:在Task_Volt_Process（）函数里面增加了SS的赋值，为了唤醒SPI，
                   否则会导致读出的第一个寄存器的电压值是65535； 
 
========================================================================*/
#include  "Includes.h"                          


VoltInfo_T VoltInfo;


/*=======================================================================
 *函数名:      Task_Volt_Collect(void)
 *功能:        电压采集函数                 
 *参数:        无       
 *返回：       无
 *说明：       对LTC6804中四个电压寄存器组。
========================================================================*/
void Task_Volt_Collect(void)
{                      
    
   // memset( &Part_Check_Fault,0,sizeof(Part_Check_Fault) );
    
    Ltc6804_Clrcell();                           /* 6804电压寄存器清0 */
                                                                     
    LTC6804_adcv();                              /* 启动AD转换，等待2.4ms收集电压值 */
    
    Task_Flag_Counter.Counter_Volt_Collect++;   
}
                                  
/*=======================================================================
 *函数名:      Task_Volt_Collect(void)
 *功能:        电压处理函数
 *参数:        无       
 *返回：       无
 *说明：       对LTC6804中四个电压寄存器组。
========================================================================*/
uint16 cell_vol[NUM_IC][12]; 
void Task_Volt_Process(void) 
{
    uint8 i,j,r,n,m,k,site=0;   
  
   // uint16 cell_vol[NUM_IC][12]; 

    uint16 cell_vvol[NUM_IC][12];

    uint16 Cell_vol[NUM_IC][12];

    uint16 maxvol=0x0000,minvol=0xffff;
    uint16  pecv_flag=0;
    uint8 pecv_error_ce=0;          
    uint16 PEC_error_v[NUM_IC*4];
    
   // LTC6804_wrcfg(NUM_IC,CFG1);
    
    
    SS2=0;
    
    SS2=1;
    for(i = 0;i < NUM_IC*4;i++)
    {
       PEC_error_v[i]=0;                                     //当PEC出错可以重新回来
    }                      
                          
    
    //delay_time(1600);    
     
    LTC6804_rdcv(0, NUM_IC, cell_vol,PEC_error_v);   //读取AD电压值   cell_vol不能直接做为最终的电压
    
    
    for(k = 0;k < NUM_IC*4;k++)
    {
       pecv_flag= pecv_flag+(PEC_error_v[k]<<k);
          
    } 
    if(!pecv_flag) 
    {
       pecv_error_ce=0x00;
      for(r=0;r<NUM_IC;r++) 
      {
       for(j=0;j<12;j++)
       {  
         if(cell_vol[r][j]!=65535)
         Cell_vol[r][j]=cell_vol[r][j];
       }
      }
    }
     
    
    /**************************电压处理*******************************/
    VoltInfo.CellVolt_Total=0;
    //1ic,12BATTERY
     for(j=0;j<NUM_IC;j++)
     {
       switch(j)
       {
          case 0:
             for(i = 0; i < NUM1_Batper_front; i++)
            {
                //if(Cell_vol[0][i] > 20000 && Cell_vol[0][i] < 38000)
                cell_vvol[0][i] = Cell_vol[0][i];            
            }
            for(i = 0; i < NUM1_Batper_rear; i++)
            {
                //if(Cell_vol[0][i+NUM1_Batper_front+NUM1_Batper_empty] > 20000 && Cell_vol[0][i+NUM1_Batper_front+NUM1_Batper_empty] < 38000)
                cell_vvol[0][i+NUM1_Batper_front] = Cell_vol[0][i+NUM1_Batper_front+NUM1_Batper_empty];            
            }
            break;
            
          case 1:
             for(i = 0; i < NUM2_Batper_front; i++)
            {
               // if(Cell_vol[1][i] > 20000 && Cell_vol[1][i] < 38000)
                cell_vvol[1][i] = Cell_vol[1][i];            
            }
            for(i = 0; i < NUM2_Batper_rear; i++)
            {
               // if(Cell_vol[1][i+NUM2_Batper_front+NUM2_Batper_empty] > 20000 && Cell_vol[1][i+NUM2_Batper_front+NUM2_Batper_empty] < 38000)
                cell_vvol[1][i+NUM2_Batper_front] = Cell_vol[1][i+NUM2_Batper_front+NUM2_Batper_empty];            
            }
            break;
            
           case 2:
             for(i = 0; i < NUM3_Batper_front; i++)
            {
               // if(Cell_vol[2][i] > 20000 && Cell_vol[2][i] < 38000)
                cell_vvol[2][i] = Cell_vol[2][i];            
            }
            for(i = 0; i < NUM3_Batper_rear; i++)
            {
               // if(Cell_vol[2][i+NUM3_Batper_front+NUM3_Batper_empty] > 20000 && Cell_vol[2][i+NUM3_Batper_front+NUM3_Batper_empty] < 38000)
                cell_vvol[2][i+NUM3_Batper_front] = Cell_vol[2][i+NUM3_Batper_front+NUM3_Batper_empty];            
            }
            break;
            
           case 3:
             for(i = 0; i < NUM4_Batper_front; i++)
            {
               // if(Cell_vol[3][i] > 20000 && Cell_vol[3][i] < 38000)
                cell_vvol[3][i] = Cell_vol[3][i];            
            }
            for(i = 0; i < NUM4_Batper_rear; i++)
            {
               // if(Cell_vol[3][i+NUM4_Batper_front+NUM4_Batper_empty] > 20000 && Cell_vol[3][i+NUM4_Batper_front+NUM4_Batper_empty] < 38000)
                cell_vvol[3][i+NUM4_Batper_front] = Cell_vol[3][i+NUM4_Batper_front+NUM4_Batper_empty];            
            }
            break;
            
           case 4:
             for(i = 0; i < NUM5_Batper_front; i++)
            {
               // if(Cell_vol[4][i] > 20000 && Cell_vol[4][i] < 38000)
                cell_vvol[4][i] = Cell_vol[4][i];            
            }
            for(i = 0; i < NUM5_Batper_rear; i++)
            {
                //if(Cell_vol[4][i+NUM5_Batper_front+NUM5_Batper_empty] > 20000 && Cell_vol[4][i+NUM5_Batper_front+NUM5_Batper_empty] < 38000)
                cell_vvol[4][i+NUM5_Batper_front] = Cell_vol[4][i+NUM5_Batper_front+NUM5_Batper_empty];            
            }
            break;
            
            default:
              break;  
       }  
     }
       
       
    // 总电压
   
   
    for(j=0;j<NUM_IC;j++)
     {
       switch(j)
       {
          case 0:
            for(k =0;k<NUM1_Batper_true;k++)
           {
             VoltInfo.CellVolt[k] = cell_vvol[0][k];
             VoltInfo.CellVolt_Total= VoltInfo.CellVolt_Total+cell_vvol[0][k];                      // 总电压
           }
            break;
            
          case 1:
             for(k =0;k<NUM2_Batper_true;k++)
           {
             VoltInfo.CellVolt[k+NUM1_Batper_true] = cell_vvol[1][k];
             VoltInfo.CellVolt_Total= VoltInfo.CellVolt_Total+cell_vvol[1][k];                      // 总电压
           }
            break;
            
           case 2:
              for(k =0;k<NUM3_Batper_true;k++)
           {
             VoltInfo.CellVolt[k+NUM1_Batper_true+NUM2_Batper_true] = cell_vvol[2][k];
             VoltInfo.CellVolt_Total= VoltInfo.CellVolt_Total+cell_vvol[2][k];                      // 总电压
           }
            break;
            
           case 3:
              for(k =0;k<NUM4_Batper_true;k++)
           {
             VoltInfo.CellVolt[k+NUM1_Batper_true+NUM2_Batper_true+NUM3_Batper_true] = cell_vvol[3][k];
             VoltInfo.CellVolt_Total= VoltInfo.CellVolt_Total+cell_vvol[3][k];                      // 总电压
           }
            break;
            
           case 4:
             for(k =0;k<NUM5_Batper_true;k++)
           {
             VoltInfo.CellVolt[k+NUM1_Batper_true+NUM2_Batper_true+NUM3_Batper_true+NUM4_Batper_true] = cell_vvol[4][k];
             VoltInfo.CellVolt_Total = VoltInfo.CellVolt_Total+cell_vvol[4][k];                      // 总电压
           }
            break;
            
            default:
              break;  
       }  
     }
            
    //最大最小电压及序号 
    maxvol=VoltInfo.CellVolt[0];  
    minvol=VoltInfo.CellVolt[0];
    
    for(i=0;i<NUM_Battery;i++)    
    {
        if(maxvol<VoltInfo.CellVolt[i]) 
        {
          
              maxvol=VoltInfo.CellVolt[i];
              
              VoltInfo.CellVolt_MaxNode=i;
            
        }
            
        if (VoltInfo.CellVolt[i]<minvol)
       {
          
            minvol=VoltInfo.CellVolt[i];
            VoltInfo.CellVolt_MinNode=i;
       }
    }  
     
    VoltInfo.CellVolt_Max = maxvol;
    VoltInfo.CellVolt_Min = minvol;
    
  
    Task_Flag_Counter.Counter_Volt_Process++;      
}