
void ser_int()                              //Initialization of the serial port
{
    //TXSTA=0x20; //BRGH=0, TXEN = 1, Asynchronous Mode, 8-bit mode
    TXSTA.BRGH=0;    //low speed
    TXSTA.TXEN=1;    //enable transmitter
    TXSTA.SYNC = 0;  //Asynchronous Mode
    TXSTA.TX9 =0;    //use 8-bit
    
    RCSTA.SPEN=1; //Serial Port enabled,8-bit reception
    SPBRG=5;           //10417 baudrate for 4Mhz =n
    PIR1.TXIF=0;
    PIR1.RCIF=0;
}

void tx_char(unsigned char a)               //sends only 1 char and print it on the terminal
{
    TXREG=a;                                //Enable the transmission by setting the TXEN control bit. This will cause the TXIF interrupt bit to be set.
    while(!PIR1.TXIF);                      //TXIF is set whenever TXREG is empty
    PIR1.TXIF = 0;
}

void Tx_string( char *str_ptr )             //send a string and print it on the terminal
{
        while (*str_ptr) {
                tx_char(*str_ptr);
                str_ptr++;
        }
}