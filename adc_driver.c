/*adc_driver.c*/

#include<lpc21xx.h>
#include"header.h"
void adc_init(void){
PINSEL1|=0x15400000;//P0.27	to P0.30
ADCR=0x00200400;
}

u32 adc_read(u8 ch_num){
u32 result=0;
ADCR|=(1<<ch_num);//Step1:select channel num
ADCR|=(1<<24);//Step2:Start ADC driver
while(((ADDR>>31)&1)==0);//Step3:Monitor the DONE flag
ADCR^=(1<<24);//Step4:Stop ADC
ADCR^=(1<<ch_num);//Step5:deselect the ch_num
result=((ADDR>>6)&0x3FF);//Step6:Extract 10 bit result
return result;
}


