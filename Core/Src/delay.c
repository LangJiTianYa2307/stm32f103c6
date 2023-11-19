#include "delay.h"

static uint32_t fac_us=0;							//us��ʱ������

static uint16_t fac_ms=0;							//ms��ʱ������,��os��,����ÿ�����ĵ�ms��
	   
/*�������ܣ���ʼ���ӳٺ���������ʼ��systick�Ĵ���*/
void delay_init(uint8_t SYSCLK)
{
	uint32_t reload;
  	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);  //SysTickƵ��ΪHCLK
	fac_us = SYSCLK;						                          //�����Ƿ�ʹ��OS,fac_us����Ҫʹ��
	reload = SYSCLK;					                            //ÿ���ӵļ������� ��λΪK	   
	reload *= 1000000 / 1000;		            //����delay_ostickspersec�趨���ʱ��
                                                        //reloadΪ24λ�Ĵ���,���ֵ:16777216,��216M��,Լ��77.7ms����	   
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;            //����SYSTICK�ж�
	SysTick->LOAD = reload; 					                    //ÿ1/OS_TICKS_PER_SEC���ж�һ��	
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;             //����SYSTICK
}								    


void delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload = SysTick->LOAD;				//LOAD��ֵ	    	 
	ticks = nus * fac_us; 						//��Ҫ�Ľ����� 
	told = SysTick->VAL;        				//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow = SysTick->VAL;	
		if(tnow != told)
		{	    
			if(tnow<told) tcnt += told - tnow;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt += reload - tnow + told;	    
			told = tnow;
			if(tcnt >= ticks) break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	};									    
}  

void delay_ms(uint32_t nms)
{
	uint32_t i;
	for(i=0;i<nms;i++) delay_us(1000);
}

			 



































