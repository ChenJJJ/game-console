/*************************************************************************
Title:    Game Console Template Header File
Inital Author:   David Jahshan
Extended by : (PUT YOUR NAME HERE) (PUT YOUR STUDENT NUMBER HERE) 
Software: AVR-GCC 
Hardware: ATMEGA16 @ 8Mhz 

DESCRIPTION:
	Macros for Game Console

*************************************************************************/

#include <avr/io.h> 
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#define byte unsigned char 

/*ON OFF*/
#define ON 0xFF
#define OFF 0x00
#define HIGH 0xFF
#define LOW 0x00
#define IN 0x00
#define OUT 0xFF
#define ALL 0xFF
#define TRUE 1
#define FALSE 0
#define FORWARD 0x00
#define BACK 0xFF

/*SET and GET MACRO*/
#define SET(PORT,MASK,VALUE) PORT = ((MASK & VALUE) | (PORT & ~MASK))
#define GET(PORT,MASK) PORT & MASK

//Devices Direction MACROs
//#define MULTI_LINES_DIR(DIR) SET(DDR?,(_BV(P?#)|_BV(P?#)|_BV(P?#)),DIR)
//LOW Battery LED
#define BAT_LOW_LED_DIR(DIR) SET(DDRB,_BV(PB0),DIR)
//Push Buttons
#define UP_BUTTON_DIR(DIR) SET(DDRA,_BV(PA1),DIR)
#define DOWN_BUTTON_DIR(DIR) SET(DDRA,_BV(PA2),DIR)
#define LEFT_BUTTON_DIR(DIR) SET(DDRA,_BV(PA3),DIR)
#define RIGHT_BUTTON_DIR(DIR) SET(DDRA,_BV(PA4),DIR)
#define ACTION1_DIR(DIR) SET(DDRA,_BV(PA5),DIR)
#define ACTION2_DIR(DIR) SET(DDRA,_BV(PA6),DIR)
#define LIGHT_DIR(DIR) SET(DDRA,_BV(PA7),DIR)
//LCD_PWM
#define LCDPWM_DIR(DIR) SET(DDRB,_BV(PB3),DIR)
//LCD
#define CD_LCD_DIR(DIR)  SET(DDRD,_BV(PD7),DIR)
#define LCD_RST_DIR(DIR) SET(DDRD,_BV(PD6),DIR)
#define CSO_LCD_DIR(DIR)  SET(DDRD,_BV(PD5),DIR)
//SPI
#define SCK_DIR(DIR) SET(DDRB,_BV(PB7),DIR)
#define MISO_DIR(DIR) SET(DDRB,_BV(PB6),DIR)
#define MOSI_DIR(DIR) SET(DDRB,_BV(PB5),DIR)
#define SS_DIR(DIR) SET(DDRB,_BV(PB4),DIR)
//Fram
#define CS_FRAM_DIR(DIR) SET(DDRB,_BV(PD2),DIR)
#define HOLD_FRAM_DIR(DIR) SET(DDRB,_BV(PD4),DIR)
#define WP_FRAM_DIR(DIR) SET(DDRB,_BV(PD3),DIR)
//BATlow Measure
#define BATLOW_DIR(DIR) SET(DDRA,_BV(PA0),DIR)



//Devices Outputs
//#define MULTI_LINES(STATE) SET(DDR?,(_BV(P?#)|_BV(P?#)|_BV(P?#)),DIR)
//LOW Battery LED
#define BAT_LOW_LED(STATE) SET(PORTB,_BV(PB0),~STATE)
//Push Buttons
#define UP_BUTTON_STATE(STATE) SET(PORTA,_BV(PA1),STATE)
#define DOWN_BUTTON_STATE(STATE) SET(PORTA,_BV(PA2),STATE)
#define LEFT_BUTTON_STATE(STATE) SET(PORTA,_BV(PA3),STATE)
#define RIGHT_BUTTON_STATE(STATE) SET(PORTA,_BV(PA4),STATE)
#define LIGHT_STATE(STATE) SET(PORTA,_BV(PA7),STATE)
//LCD
#define LCDPWM(STATE) SET(PORTB,_BV(PB3),STATE)
#define CD_LCD(STATE) SET(PORTD,_BV(PD7),STATE)
#define LCD_RST(STATE) SET(PORTD,_BV(PD6),~STATE)
#define CSO_LCD(STATE) SET(PORTD,_BV(PD5),~STATE)
//SPI
#define SCK(STATE) SET(PORTB,_BV(PB7),STATE)
#define MISO(STATE) SET(PORTB,_BV(PB6),STATE)
#define MOSI(STATE) SET(PORTB,_BV(PB5),STATE)
#define SS_STATE(STATE) SET(PORTB,_BV(PB4),STATE)
//Fram
#define CS_FRAM(STATE) SET(PORTB,_BV(PD2),STATE)
#define HOLD_FRAM(STATE) SET(PORTB,_BV(PD4),STATE)
#define WP_FRAM(STATE) SET(PORTB,_BV(PD3),STATE)



//Devices Inputs
#define UP_BUTTON GET(PINA,_BV(PA1))
#define DOWN_BUTTON GET(PINA,_BV(PA2))
#define LEFT_BUTTON GET(PINA,_BV(PA3))
#define RIGHT_BUTTON GET(PINA,_BV(PA4))
#define ACTION1 GET(PINA,_BV(PA5))
#define ACTION2 GET(PINA,_BV(PA6))
#define LIGHT GET(PINA,_BV(PA7))

#define MAX_PAGE 7    //pages 0 to 7
#define MAX_COLUMN 132
#define init_bright 30
#define LowBound_battery 110
