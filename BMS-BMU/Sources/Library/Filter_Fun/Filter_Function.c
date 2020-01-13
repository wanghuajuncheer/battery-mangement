#include  "TypeDefinition.h"
#include  "Filter_Function.h"
#include  "ADC_cfg.h"

/*=======================================================================
 *函数名:      FilterFunction_Ave(float*input, uint8 arrary)
 *功能:        电流滤波
 *参数:        float*input:指向ADC电流函数的函数指针
               arrary:获得的电流数量不超过10个       
 *返回：       滤波电流
 
 *说明：       滤波函数,对10个以内的电流值求平均值
========================================================================*/
float FilterFunction_Ave(float*input, uint8 arrary)
{
  uint8 i,j,k;
  float current[10]= {0,0,0,0,0,0,0,0,0,0}; 
  float temp=0,data=0; 
  
  if((arrary>10) || (arrary<0))
  {
    return 0xFF;
  }
  
  for(i=0; i<arrary; i++) 
  {      
    current[i] = *input++;
  }
  for(k=0; k<arrary; k++) //从小到大排序
  {
     for(j=k+1; j<arrary; j++) 
     {
        if(current[k] > current[j])
        {
           temp = current[k];
           current[k] = current[j];
           current[j] = temp;
        }
     }
  }
  for(i=1; i<arrary-1; i++) 
  {
    data += current[i]; 
  }
  
  data = data/(arrary-2); //取10组电流值去掉两端的2组值
  return data;
}

/*=======================================================================
 *函数名:      FilterFunction_Median(void) 
 *功能:        
 *参数:        无       
 *返回：       中值滤波数据
 
 *说明：       中值滤波函数
========================================================================*/
float FilterFunction_Median(float(*adc)(void), float Median) 
{
   float Cur_Filter=0,curr=0;
   
   Cur_Filter = adc();//本次检测的值
  
   if(abs(Cur_Filter - Median)>2)
   {
      curr = 0.9*Median + Cur_Filter*0.1;
   }
   else
   {
      curr = Cur_Filter;
   }
   return curr;     
}    