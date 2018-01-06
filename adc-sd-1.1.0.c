#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 		 	  
#include "key.h"     
#include "usmart.h" 
#include "malloc.h"
#include "sdio_sdcard.h"  
#include "w25qxx.h"    
#include "ff.h"  
#include "exfuns.h"    
#include "adc.h"
#include "lcd.h"
#include "dma.h"

FIL fil;
FRESULT res;
UINT bww;
char buf[100];
 int main(void)
 {	 
 	//u32 total,free;
	u8 res=0;	
	u8 times=0;
	u16 ADCconvertvalue;//自定义的变量
  float temp;
	u16 adcx;
  Adc_Init ();
	MYDMA_Config(DMA1_Channel1,(u32)&ADC1->DR,(u32)&ADCconvertvalue,16);//DMA初始化
	delay_init();	    	 //延时函数初始化	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	usmart_dev.init(72);		//初始化USMART		
 	LED_Init();		  			//初始化与LED连接的硬件接口
	//KEY_Init();					//初始化按键 
	W25QXX_Init();				//初始化W25Q128
	 
 	my_mem_init(SRAMIN);		//初始化内部内存池
 	exfuns_init();							//为fatfs相关变量申请内存			

	LCD_Init();
  f_mount(fs[0],"0:",1); 					//注册挂载SD卡 
	 
	POINT_COLOR=RED;//设置字体为红色 
	LCD_ShowString(60,70,200,16,16,"ADC TEST");	
	LCD_ShowString(60,110,200,16,16,"2017/12/12");	
	//显示提示信息
	POINT_COLOR=BLUE;//设置字体为蓝色
	LCD_ShowString(60,130,200,16,16,"ADC_CH0_VAL:");	      
	LCD_ShowString(60,150,200,16,16,"ADC_CH0_VOL:0.000V");	
	res=f_open (&fil,"0:/test.txt", FA_CREATE_ALWAYS|FA_WRITE);//开文件，res是返回值OK即为0
	f_write (&fil, "test", 4, &bww);//写抬头
	f_close(&fil);//关文件
	 
//	 char *sample_struct_time;
//	 char *sample_struct_data;
	 //sprintf (textbuffer,"\r\n%13sV%12s\r\n",sample_struct_time,sample_struct_data);
   
	 
	while(1)
	{
		//adcx=Get_Adc(ADC_Channel_1);
		LCD_ShowxNum(156,130,ADCconvertvalue,4,16,0);//显示ADC的值
		temp=(float)(ADCconvertvalue)*(3.3/4096);
		adcx=temp;
		LCD_ShowxNum(156,150,adcx,1,16,0);//显示电压值
		sprintf (buf,"\r\n%5f\r\n",temp);
		temp-=adcx;
		temp*=1000;
		LCD_ShowxNum(172,150,temp,3,16,0X80);
		
    //写文件
		res = f_open(&fil, "0:/test.txt", FA_OPEN_ALWAYS |FA_WRITE);
      if(res!=FR_OK)
          while(1);
      else
      {
        res = f_lseek(&fil, f_size(&fil));
        if(res!=FR_OK)
           while(1);
        else
        {
         res = f_write(&fil,buf,sizeof(buf), &bw);
            if(res!=FR_OK)
               while(1);
        }
      }
			times++;
			LCD_ShowxNum(156,170,times,4,16,0);//显示写入次数
     f_close(&fil);		
    /*******************************************/
		delay_ms(200);
	} 
}
















