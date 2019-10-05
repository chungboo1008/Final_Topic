#include <reg51.h>

#define Key_0 0
#define Key_1 1
#define Key_2 2
#define Key_3 3
#define Key_4 4
#define Key_5 5
#define Key_6 6
#define Key_7 7
#define Key_8 8
#define Key_9 9
#define Key_modify 10      //�ק�
#define Key_input_check 11 //����check

#define Key_Port P3 // Port 3 ���� 4x4 ��L
#define Display_Port P2 // Port 2 ���� 7-Segment Display

sbit P2_6 = P2^6; //�C�ѽXLED��port
sbit P2_7 = P2^7; //�C�ѽXLED��port
sbit K4 = P3^7;   //reset�䪺port

//LCD�ŧi
void  delay_time(int);
void  LCD_COMMAND(char);
void  LCD_Clear(void); 
void  LCD_Home(void); 
void  LCD_INIT(void);        
void  LCD_Display_Word(char); 
void  LCD_Display_String(char  *s1);
void  LCD_ADDRESS(char,char);
//------------------------------------------------------
// LCD �Ҳ�I/O���w�q
sbit     RW  = P1^1;  // �������a
sbit     RS  = P1^0; 
sbit     EN  = P2^5;  
#define  LCD_DATA   P0     // DB0-DB7  
//-------------------------------------------------------  
//   LCD �]�w����ɶ� �|�H�ۤ��P��CPU�u�@�W�v(�t��)�ӧ���
#define  d_time     3000   

int i,LED_count=0,input_count=1; //�ܼ�i,LED�p����,�P�_��J�ĴX�����ܼ�
int buffer_key_value[5]={0xff,0xff,0xff,0xff,0xff}; //�x�s���F������䪺array
int LED[5]={~0x00,~0x40,~0x10,~0x04,~0x01}; //LED�n��ܤ��򤺮e��arrary
int answer[3]={0,0,0}; //�x�s�ؼе��ת�arrary
//LCD���string1&string2
unsigned char  	code  String1[]=" Welcome to     ";
unsigned char  	code  String2[]=" Da Vinci Code! ";
unsigned char code select_array[5]={~0x01,~0x02,~0x04,~0x010,~0x20};  //�C�ѽX��檺���earrary
unsigned char code display_array[11]=  //�C�ѽXLED��n��ܤ��򤺮e��arrary
{0x3F,0x06,0x5B,0x4F,0x66, // 0, 1, 2, 3,4,
 0x6D,0x7D,0x07,0x7F,0x6F, // 5, 6, 7, 8, 9,
 0xff};  //���t


void  main(void){
	
	 LCD_INIT();                   //��l�]�wLCD  Module   
     LCD_Clear();                  //�M��LCD���RAM
     LCD_Home();                   //LCD����k�� 
 	 LCD_ADDRESS(1,1);             //�]�wLCD��Ц�}�A�Ĥ@�C�Ĥ@��
     LCD_Display_String(String1);  //���String1[]�r��
	 LCD_ADDRESS(2,1);             //�]�wLCD��Ц�}�A�ĤG�C�Ĥ@��
     LCD_Display_String(String2);  //���String2[]�r��
	
	 K4=1; //��l�ȱo�]�w 0��ܳQ�� 1�h�ۤ�

	 P2_6 = 0; //�C�ѽXLED����l�]�w
	 P2_7 = 0; //�C�ѽXLED����l�]�w

	 TMOD = 0x01; // Timer 0 Mode 1 �]�w
	 //Timer0�]�w
	 TH0 = (65536-10000)/256; // 10ms �]�w
	 TL0 = (65536-10000)%256; // 10ms �]�w
	 ET0 = 1; // �P�� Timer0 ���_
	 TR0 = 1; // �Ұ� Timer 0 �p��
	 //Timer1�]�w
	 TH1 = (65536-10000)/256; // 10ms �]�w
	 TL1 = (65536-10000)%256; // 10ms �]�w
	 ET1 = 1; // �P�� Timer1 ���_
	 TR1 = 1; // �Ұ� Timer 1 �p�� 

	 EA = 1; // �P��Ҧ����_

	 P1 = 0xFF; //LED����l�]�w
	 
     
	while(1){
		if(K4==0) //�YK4��Q���U�A��reset
		{
			for(i=0;i<5;i++)
				buffer_key_value[i]=0xff;
			LED_count=0;
			K4=1;
			input_count=1;
		}
	}

	

}


