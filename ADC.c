
int Read_ADC (int i){               //Using ADC in the A Port
ADCON0.GO = 1;       //start conversion
while(ADCON0.GO);
Delay_us(10000);
//read lower register
PORTA = ADRESL;
//read higher register
 return(ADRESL);
}

void Analog_Init(){         //Initialization
ADCON0 = 0x80;      //ADCS1=1 Fosc/32
ADCON1 = 0x80;      //read from lsb
// Select channel 0 = AN0
ADCON0.CHS0 = 0;
ADCON0.CHS1 = 0;
ADCON0.CHS2 = 0;
ADCON0.ADON = 1;      // enable ADC module
}