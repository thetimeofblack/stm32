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
	u16 ADCconvertvalue;//�Զ���ı���
  float temp;
	u16 adcx;
  Adc_Init ();
	MYDMA_Config(DMA1_Channel1,(u32)&ADC1->DR,(u32)&ADCconvertvalue,16);//DMA��ʼ��
	delay_init();	    	 //��ʱ������ʼ��	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	usmart_dev.init(72);		//��ʼ��USMART		
 	LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
	//KEY_Init();					//��ʼ������ 
	W25QXX_Init();				//��ʼ��W25Q128
	 
 	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
 	exfuns_init();							//Ϊfatfs��ر��������ڴ�			

	LCD_Init();
  f_mount(fs[0],"0:",1); 					//ע�����SD�� 
	 
	POINT_COLOR=RED;//��������Ϊ��ɫ 
	LCD_ShowString(60,70,200,16,16,"ADC TEST");	
	LCD_ShowString(60,110,200,16,16,"2017/12/12");	
	//��ʾ��ʾ��Ϣ
	POINT_COLOR=BLUE;//��������Ϊ��ɫ
	LCD_ShowString(60,130,200,16,16,"ADC_CH0_VAL:");	      
	LCD_ShowString(60,150,200,16,16,"ADC_CH0_VOL:0.000V");	
	res=f_open (&fil,"0:/test.txt", FA_CREATE_ALWAYS|FA_WRITE);//���ļ���res�Ƿ���ֵOK��Ϊ0
	f_write (&fil, "test", 4, &bww);//д̧ͷ
	f_close(&fil);//���ļ�
	 
//	 char *sample_struct_time;
//	 char *sample_struct_data;
	 //sprintf (textbuffer,"\r\n%13sV%12s\r\n",sample_struct_time,sample_struct_data);
   
	 
	while(1)
	{
		//adcx=Get_Adc(ADC_Channel_1);
		LCD_ShowxNum(156,130,ADCconvertvalue,4,16,0);//��ʾADC��ֵ
		temp=(float)(ADCconvertvalue)*(3.3/4096);
		adcx=temp;
		LCD_ShowxNum(156,150,adcx,1,16,0);//��ʾ��ѹֵ
		sprintf (buf,"\r\n%5f\r\n",temp);
		temp-=adcx;
		temp*=1000;
		LCD_ShowxNum(172,150,temp,3,16,0X80);
		
    //д�ļ�
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
			LCD_ShowxNum(156,170,times,4,16,0);//��ʾд�����
     f_close(&fil);		
    /*******************************************/
		delay_ms(200);
	} 
}
















