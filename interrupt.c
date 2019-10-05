#include <reg51.h>

//按鍵定義 直接對定數字
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

void Slect_Seg(unsigned char number); //七解碼LED選格
void Display_Seg(unsigned char display); //七解碼LED選要顯示什麼
sbit P2_6 = P2^6; //七解碼LED的port
sbit P2_7 = P2^7; //七解碼LED的port

extern unsigned char key_current=0x0F; // 按鍵現在的狀態, 1: 正常 0:被按
extern unsigned char key_previous=0x0F; // 按鍵前 10ms 的狀態, 1: 正常 0:被按

//變數i,中斷的時間變數,中斷的時間變數,視覺暫留使用到的變數,輸入為第幾次,LED計次數
extern int i,timer1_counter=3,timer0_counter=20,count=0,input_count,LED_count;
extern int buffer_key_value[5]; //儲存按了什麼按鍵的array

extern int LED[5];  //LED要顯示什麼內容的arrary
extern int answer[3];  //儲存目標答案的arrary
extern unsigned char code select_array[5];  //七解碼選格的內容arrary
extern unsigned char code display_array[11];  //七解碼LED選要顯示什麼內容的arrary

//七解碼LED選格的函式
void Slect_Seg(unsigned char number){

   P2_6 = 0;
   P2_7 = 0;
   P0 = number;
   P2_6 = 0;
   P2_7 = 1;

   P2_6 = 0;
   P2_7 = 0;

}

//七解碼LED選要顯示什麼的函示
void Display_Seg(unsigned char display){

   P2_6 = 0;
   P2_7 = 0;
   P0 = display;
   P2_6 = 1;
   P2_7 = 0;

   P2_6 = 0;
   P2_7 = 0;

}

//中斷
void my_timer1(void) interrupt 3 {
	
	TL1 = (65536-9216)%256;
	TH1 = (65536-9216)/256;
    TF1 = 0;

    timer1_counter--;
    if (timer1_counter==0){
       if(count==0)
	   {
		Slect_Seg(~0x01);
		Display_Seg(display_array[buffer_key_value[0]]);
		count=1;
	   }
	   else if(count==1)
	   {
		Slect_Seg(~0x02);
		Display_Seg(display_array[buffer_key_value[1]]);
		count=2;
	   }	   
	   else if(count==2)
	   {
		Slect_Seg(~0x04);
		Display_Seg(display_array[buffer_key_value[2]]);
		count=3;
	   }
	   else if(count==3)
	   {
		Slect_Seg(~0x10);
		Display_Seg(display_array[buffer_key_value[3]]);
		count=4;
	   }
	   else if(count==4)
	   {
		Slect_Seg(~0x20);
		Display_Seg(display_array[buffer_key_value[4]]);
		count=0;
	   }
		
	   	P1=LED[LED_count];

		timer1_counter=3;
    }
	
}

/**********************************************************
中斷副程式: Timer 0 中斷服務程式
目的 : 定義 Timer0 計時中斷服務程式，名稱為 T0_int(void)
***********************************************************/
void T0_int(void) interrupt 1
{
 unsigned char i=0;

 //目標答案設定
 answer[0]=5;
 answer[1]=2;
 answer[2]=0;

 TH0 = (65536-9216)/256; // 10ms 再設定 
 TL0 = (65536-9216)%256; // 10ms 再設定
 timer0_counter--;
 if(timer0_counter==0)
 {
 	timer0_counter=20;

	//答案check的判斷
	if(input_count==4)
	{
		Key_Port = 0xbf; // P2.6 = 0, 掃描第二列
	 	key_current = Key_Port & 0x0f;
		if (key_current != key_previous) { // 有按鍵被按 
		 if (key_current == 0x07){
		 	 //修改鍵被按
			 buffer_key_value[2] = 10;
			 input_count--;
			 }
		 }

		Key_Port = 0xef; // P2.4 = 0, 掃描第四列
		key_current = Key_Port & 0x0f; 
		if (key_current != key_previous) 
		{ // 有按鍵被按
		  if (key_current == 0x07)
		  {			 
			 input_count++;	 
			 if(buffer_key_value[0]==answer[0] && buffer_key_value[1]==answer[1] && buffer_key_value[2]==answer[2])
			 {
				buffer_key_value[3]=1;
				buffer_key_value[4]=10;
				LED_count=0;
			 }
			 else
			 {			 	
			 	if(((buffer_key_value[0]*100+buffer_key_value[1]*10+buffer_key_value[2])-(answer[0]*100+answer[1]*10+answer[2]))>0)
					buffer_key_value[4]=5;  //小
				else
					buffer_key_value[4]=6; //大
					
 			    LED_count++;

				if(LED_count==5)
				{
					buffer_key_value[0]=answer[0];
					buffer_key_value[1]=answer[1];
					buffer_key_value[2]=answer[2];
					buffer_key_value[3]=10;
					buffer_key_value[4]=10;
					LED_count=0;
				}
				else
				{
					buffer_key_value[3]=0;
					buffer_key_value[2]=10;
					buffer_key_value[1]=10;
					buffer_key_value[0]=10;
					input_count=1;
				}
			 }
		  }
	   }
	}

    //輸入個位數的按鍵判斷
	if(input_count==3)
 	{	
	 Key_Port = 0xbf; // P2.6 = 0, 掃描第二列
	 key_current = Key_Port & 0x0f;
	 if (key_current != key_previous) { // 有按鍵被按 
		 if (key_current == 0x0E){
			 buffer_key_value[2] = Key_9;
			 i++;
			 input_count++;
		 }
		 if (key_current == 0x0D){
			 buffer_key_value[2] = Key_6;
			 i++;
			 input_count++;
		 }
		 if (key_current == 0x0B){
			 buffer_key_value[2] = Key_3;
			 i++;
			 input_count++;
		 }
		 if (key_current == 0x07){
		 	 //修改鍵被按
			 i++;
			 input_count--;
			 buffer_key_value[1] = 10;
		 }
	 }
	
	 Key_Port = 0xdf; // P2.5 = 0, 掃描第三列
	 key_current = Key_Port & 0x0f;
	 if (key_current != key_previous) { // 有按鍵被按
		 if (key_current == 0x0E){
			 buffer_key_value[2] = Key_8;
			 i++;
			 input_count++;
		 }
		 if (key_current == 0x0D){
			 buffer_key_value[2] = Key_5;
			 i++;
			 input_count++;
		 }
		 if (key_current == 0x0B){
			 buffer_key_value[2] = Key_2;
			 i++;
			 input_count++;
		 }
		 if (key_current == 0x07){
			 buffer_key_value[2] = Key_0;
			 i++;
			 input_count++;
		 }
	 }
	
	 Key_Port = 0xef; // P2.4 = 0, 掃描第四列
	 key_current = Key_Port & 0x0f; 
	 if (key_current != key_previous) { // 有按鍵被按
		 if (key_current == 0x0E){
			 buffer_key_value[2] = Key_7;
			 i++;
			 input_count++;
		 }
		 if (key_current == 0x0D){
			 buffer_key_value[2] = Key_4;
			 i++;
			 input_count++;
		 }
		 if (key_current == 0x0B){
			 buffer_key_value[2] = Key_1;
			 i++;
			 input_count++;
		 }
	 }
	 if (i >= 2) //若是有兩個以上按鍵被按
	 {
		buffer_key_value[2] = 10; // 七段顯示器全亮
		input_count=3; //重新再輸入個位數
	 }
	
	 key_previous = key_current; // 更新上ㄧ次 10ms 的值
 }

 //輸入十位數的按鍵判斷	
 else if(input_count==2)
 {
	 Key_Port = 0xbf; // P2.6 = 0, 掃描第二列
	 key_current = Key_Port & 0x0f;
		 if (key_current != key_previous) { // 有按鍵被按 
		 if (key_current == 0x0E){
			 buffer_key_value[1] = Key_9;
			 i++;
			 input_count++;
		 }
		 if (key_current == 0x0D){
			 buffer_key_value[1] = Key_6;
			 i++;
			 input_count++;
		 }
		 if (key_current == 0x0B){
			 buffer_key_value[1] = Key_3;
			 i++;
			 input_count++;
		 }
		 if (key_current == 0x07){
		 	 //修改鍵被按
			 i++;
			 input_count--;
			 buffer_key_value[0] = 10;
		 }
	 }
	
	 Key_Port = 0xdf; // P2.5 = 0, 掃描第三列
	 key_current = Key_Port & 0x0f;
	 if (key_current != key_previous) { // 有按鍵被按
		 if (key_current == 0x0E){
			 buffer_key_value[1] = Key_8;
			 i++;
			 input_count++;
		 }
		 if (key_current == 0x0D){
			 buffer_key_value[1] = Key_5;
			 i++;
			 input_count++;
		 }
		 if (key_current == 0x0B){
			 buffer_key_value[1] = Key_2;
			 i++;
			 input_count++;
		 }
		 if (key_current == 0x07){
			 buffer_key_value[1] = Key_0;
			 i++;
			 input_count++;
		 }
	 }
	
	 Key_Port = 0xef; // P2.4 = 0, 掃描第四列
	 key_current = Key_Port & 0x0f; 
	 if (key_current != key_previous) { // 有按鍵被按
		 if (key_current == 0x0E){
			 buffer_key_value[1] = Key_7;
			 i++;
			 input_count++;
		 }
		 if (key_current == 0x0D){
			 buffer_key_value[1] = Key_4;
			 i++;
			 input_count++;
		 }
		 if (key_current == 0x0B){
			 buffer_key_value[1] = Key_1;
			 i++;
			 input_count++;
		 }
	 }
	 if (i >= 2) //若是有兩個以上按鍵被按
	 {
		buffer_key_value[1] = 10; // 七段顯示器全亮
		input_count=2; //重新再輸入十位數
	 }
	
	 key_previous = key_current; // 更新上ㄧ次 10ms 的值
 }

  //輸入百位數的按鍵判斷
  if(input_count==1)
  { 	
	 Key_Port = 0xbf; // P2.6 = 0, 掃描第二列
	 key_current = Key_Port & 0x0f;
		 if (key_current != key_previous) { // 有按鍵被按 
		 if (key_current == 0x0E){
			 buffer_key_value[0] = Key_9;
			 i++;
			 input_count++;
		 }
		 if (key_current == 0x0D){
			 buffer_key_value[0] = Key_6;
			 i++;
			 input_count++;
		 }
		 if (key_current == 0x0B){
			 buffer_key_value[0] = Key_3;
			 i++;
			 input_count++;
		 }
	 }
	
	 Key_Port = 0xdf; // P2.5 = 0, 掃描第三列
	 key_current = Key_Port & 0x0f;
	 if (key_current != key_previous) { // 有按鍵被按
		 if (key_current == 0x0E){
			 buffer_key_value[0] = Key_8;
			 i++;
			 input_count++;
		 }
		 if (key_current == 0x0D){
			 buffer_key_value[0] = Key_5;
			 i++;
			 input_count++;
		 }
		 if (key_current == 0x0B){
			 buffer_key_value[0] = Key_2;
			 i++;
			 input_count++;
		 }
		 if (key_current == 0x07){
			 buffer_key_value[0] = Key_0;
			 i++;
			 input_count++;
		 }
	 }
	
	 Key_Port = 0xef; // P2.4 = 0, 掃描第四列
	 key_current = Key_Port & 0x0f; 
	 if (key_current != key_previous) { // 有按鍵被按
		 if (key_current == 0x0E){
			 buffer_key_value[0] = Key_7;
			 i++;
			 input_count++;
		 }
		 if (key_current == 0x0D){
			 buffer_key_value[0] = Key_4;
			 i++;
			 input_count++;
		 }
		 if (key_current == 0x0B){
			 buffer_key_value[0] = Key_1;
			 i++;
			 input_count++;
		 }
	 }
	 if (i >= 2) //若是有兩個以上按鍵被按
	 {
		buffer_key_value[0] = 10; // 七段顯示器全亮
		input_count=1; //重新再輸入百位數
	 }
	 
	 key_previous = key_current; // 更新上ㄧ次 10ms 的值
	 
  }
 

 }
 
 
} 
