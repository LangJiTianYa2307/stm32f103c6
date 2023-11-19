#include "delay.h"

static uint32_t fac_us=0;							//us延时倍乘数

static uint16_t fac_ms=0;							//ms延时倍乘数,在os下,代表每个节拍的ms数
	   
/*函数功能：初始化延迟函数，即初始化systick寄存器*/
void delay_init(uint8_t SYSCLK)
{
	uint32_t reload;
  	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);  //SysTick频率为HCLK
	fac_us = SYSCLK;						                          //不论是否使用OS,fac_us都需要使用
	reload = SYSCLK;					                            //每秒钟的计数次数 单位为K	   
	reload *= 1000000 / 1000;		            //根据delay_ostickspersec设定溢出时间
                                                        //reload为24位寄存器,最大值:16777216,在216M下,约合77.7ms左右	   
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;            //开启SYSTICK中断
	SysTick->LOAD = reload; 					                    //每1/OS_TICKS_PER_SEC秒中断一次	
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;             //开启SYSTICK
}								    


void delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload = SysTick->LOAD;				//LOAD的值	    	 
	ticks = nus * fac_us; 						//需要的节拍数 
	told = SysTick->VAL;        				//刚进入时的计数器值
	while(1)
	{
		tnow = SysTick->VAL;	
		if(tnow != told)
		{	    
			if(tnow<told) tcnt += told - tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt += reload - tnow + told;	    
			told = tnow;
			if(tcnt >= ticks) break;			//时间超过/等于要延迟的时间,则退出.
		}  
	};									    
}  

void delay_ms(uint32_t nms)
{
	uint32_t i;
	for(i=0;i<nms;i++) delay_us(1000);
}

			 



































