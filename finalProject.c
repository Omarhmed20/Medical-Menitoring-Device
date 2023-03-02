
// Internal oscillator used @ 8MHz

#include "SPI.c"
#include "UART.c"
#include "ADC.c"
#include "Interrupt.c"



void PORT_Init();





void main() {                   //Intialize Ports
     ser_int();                       //Serial Inializatin
     Analog_Init();
     PORT_Init();
     OSCCON =  0b01100000;                  // Set internal oscillator to 4MHz
     SPI_init();                      //SPI Inializatin
     LCD_init();                      //LCD Inializatin
     
     
     
        
     TMR0 = 59;
     IOCB = 0x01;                     // Enable interrupt on change on PORTB pin 0
     WPUB = 0x0F;                     // Enable PORTB pull-ups for pins 0-3
     INTCON= 0b10101000;              // Enable global interrupts, enable interrupt on PORTB change, Timer0 interrupt
     OPTION_REG = 0b10010111;         //enable pulls ups on PORTB    Prescalar =256 
      
      
     while(1) {
              

              Temp = Read_ADC(0) * 0.489;              // Read analog voltage and convert it to degree Celsius (0.489 = 500/1023)

              if (Temp > 99)
                 Temperature[0]  = 1 + 48;              // Put 1 (of hundred)

              else
                  Temperature[0]  = ' ';                 // Put space

          Temperature[1]  = (Temp / 10) % 10  + 48;
          Temperature[2]  =  Temp % 10  + 48;
          Temperature[5] = 223;                    // Put degree symbol ( ° )
    
     

    if(PORTC.f0==1)                                //If 1 Pulse happened
    {               
                    PORTC.F1=1;                    //Lighting the LED
                    while(PORTC.f0==1);
                    Pulse++;                       //Calculating Number of pulses
                    
                    
    }
    if(PORTC.f0==0)                                //If 1 Pulse ended
    {
                    PORTC.F1=0.;                  //Closing the LED
    }
    if(PORTC.f2==1)                               //Swith button to open,close The monitor
    {
     LCD_Clear();                            
    }
    if(PORTC.f2==0)                              //If the switch was Off
    {
    
        if(Mode==0){                               //Toggle Button Mode 1
          if(ChangeMode==1){                       //If a change of Mode happend
           LCD_Clear();                            //Clear the LCD
           ChangeMode=0;
          }                        
          LCD_string(" Temp: ");              //Showing Temperature and Heart Rate
          LCD_string(" HR: ");
          LCD_second_row(  );              
          LCD_string(Temperature);              // Display LM35 temperature result on LCD
          LCD_string(" ");
          LCD_string(HeartBeat);                // Display Heart Rate result on LCD
          }
         if(Mode==1){                                //Toggle Button Mode 2
           if(ChangeMode==1){                        //If a change of Mode happend
             LCD_Clear();                            //Clear the LCD
             
             ChangeMode=0;
            } 
            LCD_string(" Temp: ");             //Showing Temperature only
            LCD_second_row(  );              
            LCD_string(Temperature);
            LCD_string("           ");
           
           }
          if(Mode==2){                               //Toggle Button Mode 3
           if(ChangeMode==1){                        //If a change of Mode happend
             LCD_Clear();                            //Clear the LCD
             ChangeMode=0;
            }
            LCD_string(" HR: ");                //Showing Heart Rate only
            LCD_second_row(  );              
            LCD_string(HeartBeat);
            LCD_string("           "); 
           
           } 
           }                       
  }
}


void PORT_Init(){
PORTA = 0x00;
PORTB=0;
PORTC = 0x00;
TRISA = 0x01;                       //only A0 defined as analog
ANSELH=0;                          //defined as digital
TRISB = 0b11111111;                //portb defined as output
TRISC = 0b10101001 ;               //difiniton of inpur/outport pins
TRISE = 0x00;
PORTE = 0x00;
}





// End of code        