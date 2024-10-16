#include<lpc21xx.h>
#include"header.h"
#define SPIF ((S0SPSR>>7)&1)

void spi0_init(void)
{
PINSEL0|=0x1500;//P0.4,5,6=0;
IODIR0=CS0;//Setting o/p dir for active low spi
IOSET0=CS0;//deselecting slave
S0SPCR=0x20;
S0SPCCR=15;//SPI freq=1Mbps
}


/*spi0_transfer function*/
u8 spi0(u8 data)
{
S0SPDR=data;
while(SPIF==0);//spif flag
return S0SPDR;
}

/*SPI0_Read function*/
u32 mcp3204_read(u8 ch_num)
{
u32 result=0;
u8 byteH=0,byteL=0;
ch_num<<=6;//Set channel number

IOCLR0=CS0;
spi0(0x06);
byteH=spi0(ch_num);
byteL=spi0(0x00);
IOSET0=CS0;//SLAVE DESELECT
byteH&=0x0F;
result=(byteH<<8)|byteL;
return result;
}

