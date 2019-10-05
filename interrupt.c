#include <reg51.h>

//����w�q ������w�Ʀr
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

void Slect_Seg(unsigned char number); //�C�ѽXLED���
void Display_Seg(unsigned char display); //�C�ѽXLED��n��ܤ���
sbit P2_6 = P2^6; //�C�ѽXLED��port
sbit P2_7 = P2^7; //�C�ѽXLED��port

extern unsigned char key_current=0x0F; // ����{�b�����A, 1: ���` 0:�Q��
extern unsigned char key_previous=0x0F; // ����e 10ms �����A, 1: ���` 0:�Q��

//�ܼ�i,���_���ɶ��ܼ�,���_���ɶ��ܼ�,��ı�ȯd�ϥΨ쪺�ܼ�,��J���ĴX��,LED�p����
extern int i,timer1_counter=3,timer0_counter=20,count=0,input_count,LED_count;
extern int buffer_key_value[5]; //�x�s���F������䪺array

extern int LED[5];  //LED�n��ܤ��򤺮e��arrary
extern int answer[3];  //�x�s�ؼе��ת�arrary
extern unsigned char code select_array[5];  //�C�ѽX��檺���earrary
extern unsigned char code display_array[11];  //�C�ѽXLED��n��ܤ��򤺮e��arrary

//�C�ѽXLED��檺�禡
void Slect_Seg(unsigned char number){

   P2_6 = 0;
   P2_7 = 0;
   P0 = number;
   P2_6 = 0;
   P2_7 = 1;

   P2_6 = 0;
   P2_7 = 0;

}

//�C�ѽXLED��n��ܤ��򪺨��
void Display_Seg(unsigned char display){

   P2_6 = 0;
   P2_7 = 0;
   P0 = display;
   P2_6 = 1;
   P2_7 = 0;

   P2_6 = 0;
   P2_7 = 0;

}

//���_
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
���_�Ƶ{��: Timer 0 ���_�A�ȵ{��
�ت� : �w�q Timer0 �p�ɤ��_�A�ȵ{���A�W�٬� T0_int(void)
***********************************************************/
void T0_int(void) interrupt 1
{
 unsigned char i=0;

 //�ؼе��׳]�w
 answer[0]=5;
 answer[1]=2;
 answer[2]=0;

 TH0 = (65536-9216)/256; // 10ms �A�]�w 
 TL0 = (65536-9216)%256; // 10ms �A�]�w
 timer0_counter--;
 if(timer0_counter==0)
 {
 	timer0_counter=20;

	//����check���P�_
	if(input_count==4)
	{
		Key_Port = 0xbf; // P2.6 = 0, ���y�ĤG�C
	 	key_current = Key_Port & 0x0f;
		if (key_current != key_previous) { // ������Q�� 
		 if (key_current == 0x07){
		 	 //�ק���Q��
			 buffer_key_value[2] = 10;
			 input_count--;
			 }
		 }

		Key_Port = 0xef; // P2.4 = 0, ���y�ĥ|�C
		key_current = Key_Port & 0x0f; 
		if (key_current != key_previous) 
		{ // ������Q��
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
					buffer_key_value[4]=5;  //�p
				else
					buffer_key_value[4]=6; //�j
					
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

    //��J�Ӧ�ƪ�����P�_
	if(input_count==3)
 	{	
	 Key_Port = 0xbf; // P2.6 = 0, ���y�ĤG�C
	 key_current = Key_Port & 0x0f;
	 if (key_current != key_previous) { // ������Q�� 
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
		 	 //�ק���Q��
			 i++;
			 input_count--;
			 buffer_key_value[1] = 10;
		 }
	 }
	
	 Key_Port = 0xdf; // P2.5 = 0, ���y�ĤT�C
	 key_current = Key_Port & 0x0f;
	 if (key_current != key_previous) { // ������Q��
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
	
	 Key_Port = 0xef; // P2.4 = 0, ���y�ĥ|�C
	 key_current = Key_Port & 0x0f; 
	 if (key_current != key_previous) { // ������Q��
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
	 if (i >= 2) //�Y�O����ӥH�W����Q��
	 {
		buffer_key_value[2] = 10; // �C�q��ܾ����G
		input_count=3; //���s�A��J�Ӧ��
	 }
	
	 key_previous = key_current; // ��s�W���� 10ms ����
 }

 //��J�Q��ƪ�����P�_	
 else if(input_count==2)
 {
	 Key_Port = 0xbf; // P2.6 = 0, ���y�ĤG�C
	 key_current = Key_Port & 0x0f;
		 if (key_current != key_previous) { // ������Q�� 
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
		 	 //�ק���Q��
			 i++;
			 input_count--;
			 buffer_key_value[0] = 10;
		 }
	 }
	
	 Key_Port = 0xdf; // P2.5 = 0, ���y�ĤT�C
	 key_current = Key_Port & 0x0f;
	 if (key_current != key_previous) { // ������Q��
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
	
	 Key_Port = 0xef; // P2.4 = 0, ���y�ĥ|�C
	 key_current = Key_Port & 0x0f; 
	 if (key_current != key_previous) { // ������Q��
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
	 if (i >= 2) //�Y�O����ӥH�W����Q��
	 {
		buffer_key_value[1] = 10; // �C�q��ܾ����G
		input_count=2; //���s�A��J�Q���
	 }
	
	 key_previous = key_current; // ��s�W���� 10ms ����
 }

  //��J�ʦ�ƪ�����P�_
  if(input_count==1)
  { 	
	 Key_Port = 0xbf; // P2.6 = 0, ���y�ĤG�C
	 key_current = Key_Port & 0x0f;
		 if (key_current != key_previous) { // ������Q�� 
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
	
	 Key_Port = 0xdf; // P2.5 = 0, ���y�ĤT�C
	 key_current = Key_Port & 0x0f;
	 if (key_current != key_previous) { // ������Q��
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
	
	 Key_Port = 0xef; // P2.4 = 0, ���y�ĥ|�C
	 key_current = Key_Port & 0x0f; 
	 if (key_current != key_previous) { // ������Q��
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
	 if (i >= 2) //�Y�O����ӥH�W����Q��
	 {
		buffer_key_value[0] = 10; // �C�q��ܾ����G
		input_count=1; //���s�A��J�ʦ��
	 }
	 
	 key_previous = key_current; // ��s�W���� 10ms ����
	 
  }
 

 }
 
 
} 
