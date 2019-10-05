//-----------------------------------------------------------------
//  �r����LCD�Ҳյ{��   
// �䤤LCD_Display_String()�@���|��ܤ@���r��
// LCD_Display_Word()�@���|��ܤ@�Ӧr��
//-----------------------------------------------------------------
#include <reg51.h>
//-------------------------------------------------------
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

sbit  K1 = P3^4;
sbit  K2 = P3^5;

#define  LCD_DATA   P0     // DB0-DB7  

//-------------------------------------------------------  
//   LCD �]�w����ɶ� �|�H�ۤ��P��CPU�u�@�W�v(�t��)�ӧ���
#define  d_time     3000    

//------------------------------------------------------------------
//     �]�w����ɶ����(��r��LCD�ҲթҨϥ�)
//------------------------------------------------------------------
void  delay_time(int k)
{
    while (k>0)
      k--;
}
//-------------------------------------------------------
//   �]�w LCD Module ����R�O
//------------------------------------------------------- 
void  LCD_COMMAND(char temp)
{
      
    RW =0;
    RS=0;                   // �]�w�s���R�O�Ȧs��
    LCD_DATA=temp;          //  �e�XLCD�R�O�r�� 
      
    EN=1;
      delay_time(d_time);
    EN=0;
}
//----------------------------------------------------
//    �M��LCD���RAM
//----------------------------------------------------
void LCD_Clear(void)
{
    LCD_COMMAND(0x01);             
}
//----------------------------------------------------
//     LCD����k��
//----------------------------------------------------
void LCD_Home(void)
{
    LCD_COMMAND(0x02);        
}
//---------------------------------------------------
//     �bLCD�W��ܤ@�Ӧr��
//---------------------------------------------------
void LCD_Display_Word(char  LCD_WORD)
{
      
    RS = 1;
    RW = 0;                   //  �]�w�s����ƼȦs��
      
    LCD_DATA=LCD_WORD;      //  �e�XLCD��Ʀr�� 
      
    EN=1;
      delay_time(d_time);
    EN=0;
}
//---------------------------------------------------
//     �bLCD�W��ܤ@�Ӧr��
//     Example: s1="LCD TEST\0";  
//     �r��פ�Ÿ���'\0'
//---------------------------------------------------
void LCD_Display_String(char  *s1)
{
   unsigned char  	  i;

    for (i=0;i<50;i++){
         if (*(s1+i)=='\0') 
         	break;  //�P�_�r��O�_�w�g�ǰe����
         	
         LCD_Display_Word(*(s1+i)); 
    }
}
//---------------------------------------------------
//    ��l�]�wLCD  Module
//---------------------------------------------------
void LCD_INIT (void) 
{
    int  i;
     RW=0;             // LCD �g�J�Ҧ�
     EN=0; 

     delay_time(d_time);	//  ����
     delay_time(d_time);	//  ����

     LCD_COMMAND(0x30);     // ���m LCD
     LCD_COMMAND(0X01);     // �M��LCD���RAM

     for(i=0;i<10;i++)      //  �]�w����ɶ�
      delay_time(d_time);
         
    LCD_COMMAND(0x3f);    // �]�wLCD����C ,��Ƭ�8bits, 5*7�r�� 
     
    //  LCD_COMMAND(0x0f);        // ����� �|�{�{
    LCD_COMMAND(0x0c);            // �L��� ���|�{�{
    // LCD_COMMAND(0x0e);         // �����  ���|�{�{
     
    LCD_COMMAND(0x06);            // �]�wLCD�۰ʼW�[�Ҧ�
    delay_time(d_time);  
    
     LCD_Clear();                 //�M��LCD���RAM
     LCD_Home();                  //LCD����k��
/* 122~127 �O�d���䥦�{���ϥ� */
   // LCD_Display_String(String1); //���String1[]�r��
     
    // LCD_ADDRESS(2,1);  			//�]�wLCD��Ц�}�A�ĤG�C�Ĥ@��

    // for (i=0;i<16;i++)    
    //    LCD_Display_Word(String2[i]); //��ܤ@�Ӧr��
}

//----------------------------------------------------
//         �]�wLCD��Ц�}
//         �C(row)    : 1,2
//         ��(column) : 1,16
//----------------------------------------------------
void  LCD_ADDRESS(char row, char column)
{
     unsigned char  tmp1;
     int  i;

//       16*2 LCD Module Address
//  row_1=> 80 81 82 83 84 85 86 87 88 89 8a 8b 8c 8d 8e 8f
//  row_2=> c0 c1 c2 c3 c4 c5 c6 c7 c8 c9 ca cb cc cd ce cf

       tmp1=(row-1)*0x40+(column-1)+0x80;
        //�p��XLCD�Ҳդ���Ь۹�������}

       for (i=0;i<10;i++)       // �]�w����ɶ� 
          delay_time(d_time);
      
       LCD_COMMAND(tmp1);       // �]�w LCD ��Ц�}
       
       for (i=0;i<10;i++)       
          delay_time(d_time);   // ����ɶ�  
}
/*------------------------------------------------------------------
//    �r����LCD�Ҳմ��եD�{��
//�ӵ{���|�b�r����LCD�Ҳ���ܨⵧ�r��A�|���
//         |      ICCI      |
//         | www.icci.com.tw |
//�䤤LCD_Display_String()�@���|��ܤ@���r��
//LCD_Display_Word()�@���|��ܤ@�Ӧr��
--------------------------------------------------------------------*/





