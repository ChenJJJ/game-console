/*************************************************************************
Title:    game_console Template
Initial Author:   David Jahshan
Extended by : (PUT YOUR NAME HERE) (PUT YOUR STUDENT NUMBER HERE)
Software: AVR-GCC 
Hardware: ATMEGA16 @ 8Mhz 

DESCRIPTION:
	Main and functions for Game Console basic sample code

*************************************************************************/


#include "game_console.h" 

byte frame_buffer[MAX_COLUMN][MAX_PAGE];  
byte page;
byte column;
byte row;
byte pixel; 
int text_column;
int text_page ;


//-----------------------------------LCD basic functions---------------------------//
void select_page(unsigned char Page) {
SPDR = Page | 0xB0;
while((SPSR & (1<<SPIF))==0); 	
}

void select_column(unsigned char column) {
SPDR = column & 0x0f;					//Load ca[3:0]
while((SPSR & (1<<SPIF))==0);		 	
column = column >> 4;
SPDR = column | 0x10;					//Load ca[7:4]
while((SPSR & (1<<SPIF))==0); 	
}

void LCD_command_tx(unsigned char tx) 
{
    SPDR = tx;
    while((SPSR & (1<<SPIF))==0); 
}

void LCD_data_tx(unsigned char tx) 
{
    CD_LCD(HIGH);		
    SPDR = tx;
    while((SPSR & (1<<SPIF))==0); 	
    CD_LCD(LOW);
}

//--------------------------------Clear LCD function-------------------------------//
void LCDclear() 
{
	int i,j;
	for (i=0;i<8;i=i+1) 			//we have maximum 8 pages
	{		
		select_page(i);
		for (j=0;j<102;j=j+1) 		//we have maximum 102 columns
		{
			select_column(j);
			LCD_data_tx( 0x00 );
		}
	}	
}

//-----------------------------------PWM initialization---------------------------//
void PWMinitial()
{
    LCDPWM_DIR(OUT);	//LCDPWM initial	
	LCDPWM(LOW);
	TCCR0 = (1<<COM01)|(1<<WGM00)|(1<<WGM01)|(1<<CS01);
		//Compare mode;  FAST PWM mode;      CLK/8     
	OCR0 = init_bright; //initial backlight brightness
}

//--------------------------------LCD_initialization-----------------------------//
byte LCD_initialise(void)
{
LCD_RST(HIGH);
_delay_ms(2);
LCD_RST(LOW);
_delay_ms(6);
LCD_command_tx(0x40);//Display start line 0
LCD_command_tx(0xA1);//SEG reverse
LCD_command_tx(0xC0);//Normal COM0~COM63
LCD_command_tx(0xA4);//Disable -> Set All Pixel to ON
LCD_command_tx(0xA6);//Display inverse off
_delay_ms(120);
LCD_command_tx(0xA2);//Set LCD Bias Ratio A2/A3
LCD_command_tx(0x2F);//Set Power Control 28...2F
LCD_command_tx(0x27);//Set VLCD Resistor Ratio 20...27
LCD_command_tx(0x81);//Set Electronic Volume
LCD_command_tx(0x10);//Set Electronic Volume 00...3F
LCD_command_tx(0xFA);//Set Adv. Program Control
LCD_command_tx(0x90);//Set Adv. Program Control x00100yz yz column wrap x Temp Comp
LCD_command_tx(0xAF);//Display on
return(TRUE);
}

//-------------------------------------ADC read function-----------------------------------//
int ADC_read(void)
{
	ADCSRA|=(1<<ADSC); 
	while((ADCSRA & (1<<ADSC))); // waiting for convsesion completed
	return (ADCH);
} 

//-----------------------------------Check Battery funtion--------------------------------//
void Battery_Check() 
{

ADMUX = (1<<REFS1)|(1<<REFS0)|(1<<ADLAR)|(0<<MUX0);	
 			//2.56V reference V; Use ) in ADCH; select PA0
 ADCSRA= (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1);//ADC enable Prescaled Freq XTAL/64 Freq
// set ADEN as 1: enable adc;   prescalar=64: (1<<ADPS2)|(1<<ADPS1)
if(ADC_read()<LowBound_battery) 
	{
		BAT_LOW_LED(ON);
	}
	else
	{BAT_LOW_LED(OFF);}
}

volatile uint8_t tot_overflow;
// global variable to count the number of overflows
void timer2_init()
{
    // set up timer with prescaler = 256
    TCCR2 |= (1 << CS22)|(1 << CS21);
    // initialize counter
    TCNT2 = 0;
    // enable overflow interrupt
    TIMSK |= (1 << TOIE2);
	 // enable global interrupts
    sei();
    // initialize overflow counter variable
    tot_overflow = 0;
}

ISR(TIMER2_OVF_vect)
{
    // keep a track of number of overflows
    tot_overflow++;
}
 

//----------------------------Ex interrupt configureation-------------------------//
void ex_INT2_init( )
{
GICR = (0 << INT2);   	//I connect push button to PB2 which is the INT2 
_delay_ms(10);
GICR = (1<<INT2);
MCUCSR = (0<<ISC2);		//falling edge activation
sei();
}


//----------------------------Fram configuration----------------------------------//
void FRAM_configuration()
{
	CS_FRAM_DIR(OUT);
	WP_FRAM_DIR(OUT);
	HOLD_FRAM_DIR(OUT);

	CS_FRAM(HIGH);
	WP_FRAM(LOW);
	HOLD_FRAM(HIGH);
}

//--------------------------Front figure shown function-------------------------------//
	void write_frontpage()     //front page shown HELLO
	{ 
	text_column=70;
	text_page = 3;
	select_page(text_page + 0); 
	select_column(text_column - 0 ); 
	LCD_data_tx( 0xFF) ; 
	select_column(text_column - 1 ); 
	LCD_data_tx( 0xFF) ; 
	select_column(text_column - 2 ); 
	LCD_data_tx( 0xC0) ; 
	select_column(text_column - 3 ); 
	LCD_data_tx( 0xC0) ; 
	select_column(text_column - 4 ); 
	LCD_data_tx( 0xC0) ; 
	select_column(text_column - 5 ); 
	LCD_data_tx( 0xFF) ; 
	select_column(text_column - 6 ); 
	LCD_data_tx( 0xFF) ; 
	select_column(text_column - 7 ); 
	LCD_data_tx( 0x0) ; 
	select_column(text_column - 8 ); 
	LCD_data_tx( 0x0) ; 
	select_column(text_column - 9 ); 
	LCD_data_tx( 0xFF) ; 
	select_column(text_column - 10 ); 
	LCD_data_tx( 0xFF) ; 
	select_column(text_column - 11 ); 
	LCD_data_tx( 0xC3) ; 
	select_column(text_column - 12 ); 
	LCD_data_tx( 0xC3) ; 
	select_column(text_column - 13 ); 
	LCD_data_tx( 0xC3) ; 
	select_column(text_column - 14 ); 
	LCD_data_tx( 0xC3) ; 
	select_column(text_column - 15 ); 
	LCD_data_tx( 0x3) ; 
	select_column(text_column - 16 ); 
	LCD_data_tx( 0x0) ; 
	select_column(text_column - 17 ); 
	LCD_data_tx( 0x0) ; 
	select_column(text_column - 18 ); 
	LCD_data_tx( 0xFF) ; 
	select_column(text_column - 19 ); 
	LCD_data_tx( 0xFF) ; 
	select_column(text_column - 20 ); 
	LCD_data_tx( 0x3) ; 
	select_column(text_column - 21 ); 
	LCD_data_tx( 0x3) ; 
	select_column(text_column - 22 ); 
	LCD_data_tx( 0x3) ; 
	select_column(text_column - 23 ); 
	LCD_data_tx( 0x3) ; 
	select_column(text_column - 24 ); 
	LCD_data_tx( 0x0) ; 
	select_column(text_column - 25 ); 
	LCD_data_tx( 0x0) ; 
	select_column(text_column - 26 ); 
	LCD_data_tx( 0xFF) ; 
	select_column(text_column - 27 ); 
	LCD_data_tx( 0xFF) ; 
	select_column(text_column - 28 ); 
	LCD_data_tx( 0x3) ; 
	select_column(text_column - 29 ); 
	LCD_data_tx( 0x3) ; 
	select_column(text_column - 30 ); 
	LCD_data_tx( 0x3) ; 
	select_column(text_column - 31 ); 
	LCD_data_tx( 0x3) ; 
	select_column(text_column - 32 ); 
	LCD_data_tx( 0x0) ; 
	select_column(text_column - 33 ); 
	LCD_data_tx( 0x0) ; 
	select_column(text_column - 34 ); 
	LCD_data_tx( 0xFC) ; 
	select_column(text_column - 35 ); 
	LCD_data_tx( 0xFE) ; 
	select_column(text_column - 36 ); 
	LCD_data_tx( 0x3) ; 
	select_column(text_column - 37 ); 
	LCD_data_tx( 0x3) ; 
	select_column(text_column - 38 ); 
	LCD_data_tx( 0xFE) ; 
	select_column(text_column - 39 ); 
	LCD_data_tx( 0xFC) ; 
	select_page(text_page + 1); 
	select_column(text_column - 0 ); 
	LCD_data_tx( 0xFF) ; 
	select_column(text_column - 1 ); 
	LCD_data_tx( 0xFF) ; 
	select_column(text_column - 2 ); 
	LCD_data_tx( 0x0) ; 
	select_column(text_column - 3 ); 
	LCD_data_tx( 0x0) ; 
	select_column(text_column - 4 ); 
	LCD_data_tx( 0x0) ; 
	select_column(text_column - 5 ); 
	LCD_data_tx( 0xFF) ; 
	select_column(text_column - 6 ); 
	LCD_data_tx( 0xFF) ; 
	select_column(text_column - 7 ); 
	LCD_data_tx( 0x0) ; 
	select_column(text_column - 8 ); 
	LCD_data_tx( 0x0) ; 
	select_column(text_column - 9 ); 
	LCD_data_tx( 0xFF) ; 
	select_column(text_column - 10 ); 
	LCD_data_tx( 0xFF) ; 
	select_column(text_column - 11 ); 
	LCD_data_tx( 0xC0) ; 
	select_column(text_column - 12 ); 
	LCD_data_tx( 0xC0) ; 
	select_column(text_column - 13 ); 
	LCD_data_tx( 0xC0) ; 
	select_column(text_column - 14 ); 
	LCD_data_tx( 0xC0) ; 
	select_column(text_column - 15 ); 
	LCD_data_tx( 0xC0) ; 
	select_column(text_column - 16 ); 
	LCD_data_tx( 0x0) ; 
	select_column(text_column - 17 ); 
	LCD_data_tx( 0x0) ; 
	select_column(text_column - 18 ); 
	LCD_data_tx( 0xFF) ; 
	select_column(text_column - 19 ); 
	LCD_data_tx( 0xFF) ; 
	select_column(text_column - 20 ); 
	LCD_data_tx( 0x0) ; 
	select_column(text_column - 21 ); 
	LCD_data_tx( 0x0) ; 
	select_column(text_column - 22 ); 
	LCD_data_tx( 0x0) ; 
	select_column(text_column - 23 ); 
	LCD_data_tx( 0x0) ; 
	select_column(text_column - 24 ); 
	LCD_data_tx( 0x0) ; 
	select_column(text_column - 25 ); 
	LCD_data_tx( 0x0) ; 
	select_column(text_column - 26 ); 
	LCD_data_tx( 0xFF) ; 
	select_column(text_column - 27 ); 
	LCD_data_tx( 0xFF) ; 
	select_column(text_column - 28 ); 
	LCD_data_tx( 0x0) ; 
	select_column(text_column - 29 ); 
	LCD_data_tx( 0x0) ; 
	select_column(text_column - 30 ); 
	LCD_data_tx( 0x0) ; 
	select_column(text_column - 31 ); 
	LCD_data_tx( 0x0) ; 
	select_column(text_column - 32 ); 
	LCD_data_tx( 0x0) ; 
	select_column(text_column - 33 ); 
	LCD_data_tx( 0x0) ; 
	select_column(text_column - 34 ); 
	LCD_data_tx( 0x3F) ; 
	select_column(text_column - 35 ); 
	LCD_data_tx( 0x7F) ; 
	select_column(text_column - 36 ); 
	LCD_data_tx( 0xC0) ; 
	select_column(text_column - 37 ); 
	LCD_data_tx( 0xC0) ; 
	select_column(text_column - 38 ); 
	LCD_data_tx( 0x7F) ; 
	select_column(text_column - 39 ); 
	LCD_data_tx( 0x3F) ; 
	}


int main(void)
{	
	BAT_LOW_LED(OFF); //Make sure it is off before changing direction
	BAT_LOW_LED_DIR(OUT); //Set BATTERY LED I/Os as outputs.
	
	BATLOW_DIR(IN);

	UP_BUTTON_DIR(IN); //Set UP_BUTTON I/Os as input.
	DOWN_BUTTON_DIR(IN); //Set DOWN_BUTTON I/Os as input.
	LEFT_BUTTON_DIR(IN); //Set LEFT_BUTTON I/Os as input.
	RIGHT_BUTTON_DIR(IN); //Set RIGHT_BUTTON I/Os as input.
	LIGHT_DIR(IN);		//Set Light change button I/Os as input.
	ACTION1_DIR(IN);	//Set action 1 button as input
	ACTION2_DIR(IN);	//Set action 2 button as input

//----------------- Set pull up resistors-----------------------//
	UP_BUTTON_STATE(HIGH);	
	DOWN_BUTTON_STATE(HIGH);
	LEFT_BUTTON_STATE(HIGH);
	RIGHT_BUTTON_STATE(HIGH);
	LIGHT_STATE(HIGH);
	ACTION1_DIR(HIGH);
	ACTION2_DIR(HIGH);
	
//-----------------	 SPI configuration-------------------------//
	SCK_DIR(OUT);    	
    MISO_DIR(IN);
	MOSI_DIR(OUT);

//------------------MCU configuration---------------------//
    SS_DIR(OUT);        	//set MCU PB4 as output
	CSO_LCD_DIR(OUT);        //Set LCD I/Os as output
    CD_LCD_DIR(OUT);
	LCD_RST_DIR(OUT);

    CSO_LCD(HIGH);
	SPCR = (1<<SPE)|(1<<MSTR); //SPE and MSTR must be high, initialize SPI, CLK/4
	
	LCD_initialise(); //initialise LCD
	LCDclear();	
	PWMinitial();  //initialise our LCD PWM
	
	timer2_init();		//initialise Timer2 interrupt
	ex_INT2_init();		//initialise External interrupt
	
	row=30;          //inti values
	column=70;
	int direction_sign =0;// moving direction 1 is up, 2 is down, 3 is left, 4 is right

	write_frontpage();	//show 'hello' at the first beginning
	 _delay_ms(1000);

	LCDclear();
	while (TRUE)//Master loop always true so always loop
	{	
	
		if (tot_overflow >= 12)  			//every overflow time is 4.096ms
        {
            // check if the timer count reaches 53
            if (TCNT2 >= 53)				//around 50ms totally
            {	
				if(direction_sign==1)
               	{row++;}
				else if(direction_sign==2)
               	{row--;}
				else if(direction_sign==3)
               	{column++;}
				else if(direction_sign==4)
               	{column--;}
                TCNT2 = 0;            // reset counter
                tot_overflow = 0;     // reset overflow counter
            }
        }
		
	 	if(~UP_BUTTON) {row++;direction_sign=1;}
        if(~DOWN_BUTTON) {row --;direction_sign=2;}
        if(~RIGHT_BUTTON){column--;direction_sign=4;}
        if(~LEFT_BUTTON){column++;direction_sign=3;}
        page = row/8;
        pixel = row%8;
        select_page(page);
        select_column(column);
        pixel = (_BV(pixel) | (frame_buffer[column][page]));
        frame_buffer[column][page] = pixel;
        LCD_data_tx(pixel);
        _delay_ms(100);
		
		Battery_Check();	//Check whether the battery power low level or not
		
	}

}


//------------------------Interrupt Service Route--------------------------------------//
ISR(INT2_vect)   //ISR for external INT2-------Backlight PWM control
{
	if(~LIGHT) 		//when external interrupt is activated, check the button, if pressed, then change the brightness of backlight
{
	if(OCR0<220)
	{OCR0=OCR0+10;}
	else
	{OCR0=1;}		//reset the backlight to low light
	
}
}
