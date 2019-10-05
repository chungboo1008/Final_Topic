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
#define Key_modify 10      //修改
#define Key_input_check 11 //答案check

#define Key_Port P3 // Port 3 接到 4x4 鍵盤
#define Display_Port P2 // Port 2 接到 7-Segment Display

sbit P2_6 = P2^6; //七解碼LED的port
sbit P2_7 = P2^7; //七解碼LED的port
sbit K4 = P3^7;   //reset鍵的port

//LCD宣告
void  delay_time(int);
void  LCD_COMMAND(char);
void  LCD_Clear(void); 
void  LCD_Home(void); 
void  LCD_INIT(void);        
void  LCD_Display_Word(char); 
void  LCD_Display_String(char  *s1);
void  LCD_ADDRESS(char,char);
//------------------------------------------------------
// LCD 模組I/O的定義
sbit     RW  = P1^1;  // 直接接地
sbit     RS  = P1^0; 
sbit     EN  = P2^5;  
#define  LCD_DATA   P0     // DB0-DB7  
//-------------------------------------------------------  
//   LCD 設定延遲時間 會隨著不同的CPU工作頻率(速度)而改變
#define  d_time     3000   

int i,LED_count=0,input_count=1; //變數i,LED計次數,判斷輸入第幾次的變數
int buffer_key_value[5]={0xff,0xff,0xff,0xff,0xff}; //儲存按了什麼按鍵的array
int LED[5]={~0x00,~0x40,~0x10,~0x04,~0x01}; //LED要顯示什麼內容的arrary
int answer[3]={0,0,0}; //儲存目標答案的arrary
//LCD顯示string1&string2
unsigned char  	code  String1[]=" Welcome to     ";
unsigned char  	code  String2[]=" Da Vinci Code! ";
unsigned char code select_array[5]={~0x01,~0x02,~0x04,~0x010,~0x20};  //七解碼選格的內容arrary
unsigned char code display_array[11]=  //七解碼LED選要顯示什麼內容的arrary
{0x3F,0x06,0x5B,0x4F,0x66, // 0, 1, 2, 3,4,
 0x6D,0x7D,0x07,0x7F,0x6F, // 5, 6, 7, 8, 9,
 0xff};  //全暗


void  main(void){
	
	 LCD_INIT();                   //初始設定LCD  Module   
     LCD_Clear();                  //清除LCD顯示RAM
     LCD_Home();                   //LCD游標歸位 
 	 LCD_ADDRESS(1,1);             //設定LCD游標位址再第一列第一行
     LCD_Display_String(String1);  //顯示String1[]字串
	 LCD_ADDRESS(2,1);             //設定LCD游標位址再第二列第一行
     LCD_Display_String(String2);  //顯示String2[]字串
	
	 K4=1; //初始值得設定 0表示被按 1則相反

	 P2_6 = 0; //七解碼LED的初始設定
	 P2_7 = 0; //七解碼LED的初始設定

	 TMOD = 0x01; // Timer 0 Mode 1 設定
	 //Timer0設定
	 TH0 = (65536-10000)/256; // 10ms 設定
	 TL0 = (65536-10000)%256; // 10ms 設定
	 ET0 = 1; // 致能 Timer0 中斷
	 TR0 = 1; // 啟動 Timer 0 計時
	 //Timer1設定
	 TH1 = (65536-10000)/256; // 10ms 設定
	 TL1 = (65536-10000)%256; // 10ms 設定
	 ET1 = 1; // 致能 Timer1 中斷
	 TR1 = 1; // 啟動 Timer 1 計時 

	 EA = 1; // 致能所有中斷

	 P1 = 0xFF; //LED的初始設定
	 
     
	while(1){
		if(K4==0) //若K4鍵被按下，做reset
		{
			for(i=0;i<5;i++)
				buffer_key_value[i]=0xff;
			LED_count=0;
			K4=1;
			input_count=1;
		}
	}

	

}


