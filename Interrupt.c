int Timer1 = 0;                           //Counter for displaying the Values on the UART 
int Timer2 = 0;                           //Counter for Changing the value of the Heart Rate
char Temperature[] = " 00.0 C";
char HeartBeat[] = " 00 P/M";
unsigned int Temp;                        
int Pulse=0;                              //Number of pulses calculating for a minute
int ToggleButton=1;                       // Clicks on the toggle button                      
int Mode=0;                               //Mode of toggle 
int ChangeMode=0;                         //Changing the mod

void interrupt(){
     if(INTCON.RBIF == 1) // If interrupt on change flag set
     {if (PORTB.B0 == 1) // If PORTB pin is high, then it is rising edge
     {
     if(ToggleButton==0)             //Third Click will get the 1st mode   
      {
       ChangeMode=1;
       Mode=0;
       
      }
     if(ToggleButton==2)             //Second click will get the 3nd mode
      {                                                                 
       ChangeMode=1;
       Mode=2;
        }
       if(ToggleButton==1)           //First click will get the 2nd mode
      {
       ChangeMode=1;
       Mode=1;
      }
      ToggleButton++;                //If it became 3 then it resets
      if(ToggleButton==3)
      {ToggleButton=0;}
      
      }
       Temp =PORTB.B0; // Read PORTB pin 0 to end mismatch condition
       INTCON.RBIF = 0; // Clear PORTB Interrupt on change flag
     }

     if(INTCON.T0IF==1)               //Timer0 Interrupt
     {
      Timer1++;
      Timer2++;
      INTCON.T0IF=0;
     }
 
      if(Timer2 == 195)          //After 10 seconds (0.05*200)
      {
      if(PORTC.f2==0)                               //Switch button(Appears on the terminal if the switch was off) 
      {
        if(ToggleButton==1 || ToggleButton==2)      //Temperature Appears if it is in the first or second mode
        {
          Tx_string("Temperature:"); 
          Tx_string(Temperature);
          tx_char('\r\n');  
        }
        if(ToggleButton==1 || ToggleButton==0)      //Heart Rate Appears if it is in the first or thrid mode
        {
          Tx_string("Heart Rate: ");
          Tx_string(HeartBeat);
              
         
        }
          tx_char('\r\n'); 
          tx_char('\r\n');
      }
      Timer2=0;
      
      }
      if(Timer1 == 293)                            //After 15 seconds (0.05*300 )
        {
        Timer1 = 0;
        Pulse = Pulse *4;
        if (Pulse > 99){
           HeartBeat[0]  = 1 + 48; }             // Put 1 (of hundred)
  
        else{
            HeartBeat[0]  = ' '; }              //Calculating the Heart Rate every 15 second
        HeartBeat[1]  = (Pulse / 10) % 10  + 48;
        HeartBeat[2]  =  Pulse % 10  + 48;
        Pulse=0;
      }
      
      TMR0=59;
} 