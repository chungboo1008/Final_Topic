//-----------------------------------------------------------------
//  字元型LCD模組程式   
// 其中LCD_Display_String()一次會顯示一筆字串
// LCD_Display_Word()一次會顯示一個字元
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
// LCD 模組I/O的定義
sbit     RW  = P1^1;  // 直接接地
sbit     RS  = P1^0; 
sbit     EN  = P2^5;  

sbit  K1 = P3^4;
sbit  K2 = P3^5;

#define  LCD_DATA   P0     // DB0-DB7  

//-------------------------------------------------------  
//   LCD 設定延遲時間 會隨著不同的CPU工作頻率(速度)而改變
#define  d_time     3000    

//------------------------------------------------------------------
//     設定延遲時間函數(文字型LCD模組所使用)
//------------------------------------------------------------------
void  delay_time(int k)
{
    while (k>0)
      k--;
}
//-------------------------------------------------------
//   設定 LCD Module 控制命令
//------------------------------------------------------- 
void  LCD_COMMAND(char temp)
{
      
    RW =0;
    RS=0;                   // 設定存取命令暫存器
    LCD_DATA=temp;          //  送出LCD命令字元 
      
    EN=1;
      delay_time(d_time);
    EN=0;
}
//----------------------------------------------------
//    清除LCD顯示RAM
//----------------------------------------------------
void LCD_Clear(void)
{
    LCD_COMMAND(0x01);             
}
//----------------------------------------------------
//     LCD游標歸位
//----------------------------------------------------
void LCD_Home(void)
{
    LCD_COMMAND(0x02);        
}
//---------------------------------------------------
//     在LCD上顯示一個字元
//---------------------------------------------------
void LCD_Display_Word(char  LCD_WORD)
{
      
    RS = 1;
    RW = 0;                   //  設定存取資料暫存器
      
    LCD_DATA=LCD_WORD;      //  送出LCD資料字元 
      
    EN=1;
      delay_time(d_time);
    EN=0;
}
//---------------------------------------------------
//     在LCD上顯示一個字串
//     Example: s1="LCD TEST\0";  
//     字串終止符號為'\0'
//---------------------------------------------------
void LCD_Display_String(char  *s1)
{
   unsigned char  	  i;

    for (i=0;i<50;i++){
         if (*(s1+i)=='\0') 
         	break;  //判斷字串是否已經傳送完畢
         	
         LCD_Display_Word(*(s1+i)); 
    }
}
//---------------------------------------------------
//    初始設定LCD  Module
//---------------------------------------------------
void LCD_INIT (void) 
{
    int  i;
     RW=0;             // LCD 寫入模式
     EN=0; 

     delay_time(d_time);	//  延遲
     delay_time(d_time);	//  延遲

     LCD_COMMAND(0x30);     // 重置 LCD
     LCD_COMMAND(0X01);     // 清除LCD顯示RAM

     for(i=0;i<10;i++)      //  設定延遲時間
      delay_time(d_time);
         
    LCD_COMMAND(0x3f);    // 設定LCD為兩列 ,資料為8bits, 5*7字型 
     
    //  LCD_COMMAND(0x0f);        // 有游標 會閃爍
    LCD_COMMAND(0x0c);            // 無游標 不會閃爍
    // LCD_COMMAND(0x0e);         // 有游標  不會閃爍
     
    LCD_COMMAND(0x06);            // 設定LCD自動增加模式
    delay_time(d_time);  
    
     LCD_Clear();                 //清除LCD顯示RAM
     LCD_Home();                  //LCD游標歸位
/* 122~127 保留給其它程式使用 */
   // LCD_Display_String(String1); //顯示String1[]字串
     
    // LCD_ADDRESS(2,1);  			//設定LCD游標位址再第二列第一行

    // for (i=0;i<16;i++)    
    //    LCD_Display_Word(String2[i]); //顯示一個字元
}

//----------------------------------------------------
//         設定LCD游標位址
//         列(row)    : 1,2
//         行(column) : 1,16
//----------------------------------------------------
void  LCD_ADDRESS(char row, char column)
{
     unsigned char  tmp1;
     int  i;

//       16*2 LCD Module Address
//  row_1=> 80 81 82 83 84 85 86 87 88 89 8a 8b 8c 8d 8e 8f
//  row_2=> c0 c1 c2 c3 c4 c5 c6 c7 c8 c9 ca cb cc cd ce cf

       tmp1=(row-1)*0x40+(column-1)+0x80;
        //計算出LCD模組之游標相對應的位址

       for (i=0;i<10;i++)       // 設定延遲時間 
          delay_time(d_time);
      
       LCD_COMMAND(tmp1);       // 設定 LCD 游標位址
       
       for (i=0;i<10;i++)       
          delay_time(d_time);   // 延遲時間  
}
/*------------------------------------------------------------------
//    字元型LCD模組測試主程式
//該程式會在字元型LCD模組顯示兩筆字串，會顯示
//         |      ICCI      |
//         | www.icci.com.tw |
//其中LCD_Display_String()一次會顯示一筆字串
//LCD_Display_Word()一次會顯示一個字元
--------------------------------------------------------------------*/





