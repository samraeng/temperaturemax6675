#include <16LF1618.h>
#device ADC=10
#use delay(internal=32000000)



#define MAX_CS PIN_B6 
#define MAX_DO PIN_B7 
#define MAX_CLK PIN_B5 

#define LCD_ENABLE_PIN PIN_A2
#define LCD_RS_PIN PIN_A0
#define LCD_RW_PIN PIN_A1
#define LCD_DATA4 PIN_C0
#define LCD_DATA5 PIN_C1
#define LCD_DATA6 PIN_C2
#define LCD_DATA7 PIN_B4

#include <lcd.c>
byte const num[12] = {'0','1','2','3','4','5','6','7','8','9'};
void num_digit(unsigned int16 value_x);
void init_TC(); 
int16 read_TC(void)  ;
int16 sortout(int16 raw) ;
int16 toCalc_TC(int16 tmp); 
int16 do_everything(void); 

INT8 num0,num10,num100,num1000;
INT16 TEMP;
int1 thermocouple_error;
void main()
{

   lcd_init();
   lcd_gotoxy(1,1);// lcd_gotoxy(x,y)  y=row x=colum
   lcd_putc("temperature");
   init_tC();
   
   while(TRUE)
   { //init_TC(); 
     TEMP= do_everything();

        if (!(thermocouple_error))
        {
         num_digit(TEMP);
         lcd_gotoxy(4,0);
         lcd_putc(num[NUM1000]);lcd_putc(num[NUM100]);lcd_putc(num[NUM10]);lcd_putc(num[NUM0]);      
         DELAY_MS(500);
         }
         else
         {
          lcd_gotoxy(1,0);
          lcd_putc("read temp error");
         }
      
      
   }

}




void init_TC() 
{ 

output_low(MAX_CLK); 
output_low(MAX_DO); 
output_low(MAX_CS); 
setup_spi(SPI_MASTER | SPI_L_TO_H | SPI_CLK_DIV_16); 
output_high(MAX_CS); 

} 

int16 read_TC(void)    
{ 
    BYTE datah, datal=0; 
    int16 data=0; 
    int i; 

    output_low(MAX_CS); 
   for (i = 0; i<16; i++) { 
      output_high(MAX_CLK); 
      delay_cycles(1); 
      output_low(MAX_CLK); 
      shift_left(&data,2,input(MAX_DO)); 
   } 
  
  thermocouple_error = bit_test(data,2); 
  output_high(MAX_CS); 
    return(data); 
} 


void num_digit(unsigned int16 value_x)
{ 
           num0  =    value_x % 10 ;
           num10 =   (value_x/10)%10;
           num100 =  (value_x/100)%10 ;
           num1000 = (value_x/1000);  
         
           
           
}

int16 sortout(int16 raw) 
{ 
    return(0x0FFF & (raw>>3)); 
} 

int16 toCalc_TC(int16 tmp) 
{ 
   return((int16)tmp * 1); 
}

int16 do_everything(void) 
{ 
   init_TC();  
   return(toCalc_TC(sortout(read_TC()))); 
} 
