

unsigned char LCDpins;

#define RS  0x04    // RS pin
#define E   0x08    // E pin



#define        Scroll_right                0x1e        /* Scroll display one character right (all lines)                 */
#define Scroll_left                        0x18        /* Scroll display one character left (all lines)                        */

#define        LCD_HOME                        0x02        /* Home (move cursor to top/left character position)        */
#define        Cursor_left                        0x10        /* Move cursor one character left                                                                                        */
#define        Cursor_right                0x14        /* Move cursor one character right                                                                                */
#define Cursor_uline                0x0e        /* Turn on visible underline cursor                                                                         */
#define Cursor_block                0x0f        /* Turn on visible blinking-block cursor                                                        */
#define Cursor_invis                0x0c        /* Make cursor invisible                                                                                                                        */
#define        Display_blank                0x08        /* Blank the display (without clearing)                                                                */
#define Display_restore                0x0c        /* Restore the display (with cursor hidden)                                                */
#define        LCD_CLRSCR                        0x01        /* Clear Screen                                                                                                                                                                */

#define        SET_CURSOR                        0x80        /* Set cursor position (Set_cursor+DDRAM address)                        */
#define        Set_CGRAM                                0x40        /* Set pointer in character-generator RAM Set_CGRAM+(CGRAM address)        */


 //////////////SPI FUNCTIONS


void SPI_init()
{

  //master mode, clk fosc/4
  SSPCON.B3=  0;
  SSPCON.B2=  0;
  SSPCON.B1=   0;
  SSPCON.B0=  0;
  SSPCON.SSPEN=1;              // enable synchronous serial port
  // clk idle state low
  SSPCON.CKP=  0;
  // data read on low to high  MODE 0,0
  SSPSTAT.CKE=1; //if ckp=0
  //input data sampled at the middle of interval
   SSPSTAT.SMP=0;
   TRISC.B3 = 0;                 // define clock pin as output
   TRISC.B5=0;                  // define SDO as output (master ) to lcd
   // Define clock pin as an output for clk

}
unsigned char wr_SPI ( unsigned char dat )
{
 SSPBUF = dat;             // write byte to SSPBUF register
 while( !SSPSTAT.BF );  // wait until bus cycle complete
 return ( SSPBUF );      
}

/* copies LCDpins variable to parallel output of the shift register */
void SPI_to_74HC595(  )
{
    wr_SPI ( LCDpins );     // send LCDpins out the SPI
    PORTE.B0 = 1;                //move data to parallel pins
    PORTE.B0 = 0;
}

void LCD_sendbyte( unsigned char tosend )
{
    LCDpins &= 0x0f;                //prepare place for the upper nibble
    LCDpins |= ( tosend & 0xf0 );   //copy upper nibble to LCD variable
    LCDpins |= E        ;                  //send
    SPI_to_74HC595();
    LCDpins &= ~E        ;
    SPI_to_74HC595();
    LCDpins &= 0x0f;                    //prepare place for the lower nibble
    LCDpins |= ( tosend << 4 ) & 0xf0;    //copy lower nibble to LCD variable
    LCDpins |= E          ;                   //send
    SPI_to_74HC595();
    LCDpins &= ~E          ;
    SPI_to_74HC595();
}
void LCD_sendcmd(unsigned char a)
 {   LCDpins &= ~RS         ;
      LCD_sendbyte(a);
  }

void LCD_sendchar(unsigned char a)
{   LCDpins |= RS            ;
    LCD_sendbyte(a);
}
/* LCD initialization by instruction                */
/* 4-bit 2 line                                     */
/* wait times are set for 8MHz clock (TCY 500ns)    */
void LCD_init ( void )
{
  LCDpins &= ~RS              ;
  PORTE.B0 = 0;
  /* wait 100msec */
  Delay_ms(100);
  /* send 0x03 */
  LCDpins =  0x30;  // send 0x3
  LCDpins |= E                 ;
  SPI_to_74HC595 ();
  LCDpins &= ~E                 ;
  SPI_to_74HC595 ();
  /* wait 10ms */
  Delay_ms(10);
  LCDpins |= E  ;        // send 0x3
  SPI_to_74HC595 ();
  LCDpins &= ~E                  ;
  SPI_to_74HC595 ();
  /* wait 10ms */
  Delay_ms(10);
  LCDpins |= E         ;// send 0x3
  SPI_to_74HC595 ();
  LCDpins &= ~E         ;
  SPI_to_74HC595 ();
  /* wait 1ms */
  Delay_ms(10);
  LCDpins =  0x20;        // send 0x2 - switch to 4-bit
  LCDpins |= E           ;
  SPI_to_74HC595();
  LCDpins &= ~E           ;
  SPI_to_74HC595();
  /* regular transfers start here */
 Delay_ms(10);
  LCD_sendcmd ( 0x28 );   //4-bit 2-line 5x7-font
  Delay_ms(10);
  LCD_sendcmd ( 0x01 );   //clear display
  Delay_ms(10);
  LCD_sendcmd ( 0x0c );   //turn off cursor, turn on display
  Delay_ms(10);
  LCD_sendcmd ( 0x06 );   //Increment cursor automatically
}
void LCD_string( char *str_ptr )
{
        while (*str_ptr) {
                LCD_sendchar(*str_ptr);
                str_ptr++;
        }
}
void LCD_second_row(  )
{
        LCD_sendcmd( 0xc0 );
}


void LCD_Clear(  )
{
 LCD_sendcmd( LCD_CLRSCR );
  Delay_ms(10);
  LCD_sendcmd( LCD_HOME );
}
