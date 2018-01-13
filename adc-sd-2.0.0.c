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
char buf[100];//字符串数组，用于写文件
float prebuf[10];//暂存10个adc数据
 int main(void)
 {	 
 	//u32 total,free;
	u8 res=0;	
	u16 times=0;//记录循环次数的变量
	u8 flag=1;//记录文件是否已经关闭的标志，1为已关闭，0为已开启。
	u16 ADCconvertvalue;//自定义的变量
  float temp;
	//u16 adcx;
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
	 


	while(1)
	{
		times++;
		temp=(float)(ADCconvertvalue)*(3.3/4096);
		prebuf[times%10]=temp;//存10个float值
    //写文件
		if(times%10==0)//每十个循环进行一次：给buf字符串赋值，判断是否能存数据
		{
		 sprintf (buf,"\r\n%5f\r\n%5f\r\n%5f\r\n%5f\r\n%5f\r\n%5f\r\n%5f\r\n%5f\r\n%5f\r\n%5f\r",prebuf[1],prebuf[2],prebuf[3],prebuf[4],prebuf[5],prebuf[6],prebuf[7],prebuf[8],prebuf[9],prebuf[0]);
		 if(flag==1)//判断文件是否开着，是否能存数据
		 {
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
					flag=0;
            if(res!=FR_OK)
               while(1);
        }
      }
		 }
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
	 }
		
		if(times==1000)//1000次写数据后关闭一次文件以防止意外丢失文件
    { 
		f_close(&fil);
    times=0;
		flag=1;
    }			
    /*******************************************/
//		delay_ms(200);
	} 
}
















