
#include<lpc21xx.h>
#include"header.h"
void lcd_print(u8 x){
lcd_data((x/0x10)+48);
lcd_data((x%0x10)+48);
}
struct time{
u8 h,m;	
}ti;
struct date{
u8 d,m;	
}d;

main(){
//char l[]="TEMP:00C POT:0.0V";
unsigned int light,adc_out,xpot,temp,s,i,j,sl,sp,k;
float xlight,pot,vout;
lcd_init();
adc_init();
spi0_init();
i2c_init();
uart0_init(9600);

//writing data to rtc
i2c_byte_write(0xD0,0x1,0x50);//sec
i2c_byte_write(0xD0,0x1,0x50);//min
i2c_byte_write(0xD0,0x2,0x10);//hrs
i2c_byte_write(0xD0,0x4,0x05);//date
i2c_byte_write(0xD0,0x5,0x09);//month
i2c_byte_write(0xD0,0x6,0x24);//year
lcd_cmd(0x01);
lcd_string("h");
while(1){
//task1:for 20 readings=temp
//temp sensor reading
lcd_cmd(0x80);
for(s=0,i=0;i<30;i++){
adc_out=adc_read(1);
s=s+adc_out;
}
adc_out=s/30;
vout=(adc_out*3.3)/1023;
temp=(vout-0.5)/0.01;


//temp sensor writing
//uart0_tx_integer(temp);
lcd_string("T:");
lcd_itoa(temp);
lcd_cgram();
lcd_cmd(0x84);//for degree symbol
lcd_data(0);//page 0 
lcd_string("C Pot:");

//pot voltage reading

for(sp=0,k=0;k<30;k++){
xpot=adc_read(2);
sp=sp+xpot;
}
xpot=sp/30;
pot=(xpot*3.3)/1023;
//uart0_tx_string("pot reading\r\n");
//uart0_tx_float(pot);
lcd_ftoa(pot);
lcd_data('V');

//LCD 2nd row
lcd_cmd(0xC0);
//Light reading
//task2:convert to percentage
//task:20 readings at once

lcd_string("L:");
for(sl=0,j=0;j<20;j++){
xlight=mcp3204_read(2);
sl=sl+xlight;
}
xlight=sl/200;
light=((4095-xlight)/4095)*100;
lcd_itoa(light);
//uart0_tx_string("\r\nmcp3204 adc\n");
//uart0_tx_integer(light);
lcd_string("% ");

//reading rtc
//check rtc seperately
ti.m=i2c_byte_read_frame(0xD0,0x1);
ti.h=i2c_byte_read_frame(0xD0,0x2);
 lcd_print(ti.h);
 lcd_data(':');
 lcd_print(ti.m);
 lcd_data(' ');
d.d=i2c_byte_read_frame(0xD0,0x4);
d.m=i2c_byte_read_frame(0xD0,0x5);
lcd_print(d.d);
lcd_data('/');
lcd_data((d.m%0x10)+48);
//lcd_print(d.m);
}
}


