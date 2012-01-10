
#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/boot.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;


#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))
#define DISABLE_JTAG()  MCUCR = (1 << JTD) | (1 << IVCE) | (0 << PUD); MCUCR = (1 << JTD) | (0 << IVSEL) | (0 << IVCE) | (0 << PUD);

#define USB_PID_LEONARDO 0x0034
#define USB_PID_MICRO 0x0035
#define USB_VID 0x2341	// arduino LLC vid
#define USB_PID ARDUINO_MODEL_PID	// passed in by Makefile - 0x0034 for Leonardo, 0x0035 for MIcro

#define USB_SERIAL_STRING	'0','0','0','0','0','0','0','0','1','7','0','1'

#define OEM_NAME		'l','e','o','n','a','r','d','o'					// 8 chars
#define BOARD_INIT()	DDRC |= (1<<7); DDRB |= (1<<0); DDRD |= (1<<5); CPU_PRESCALE(0); DISABLE_JTAG();
#define L_LED_OFF()		PORTC &= ~(1<<7)
#define L_LED_ON()		PORTC |= (1<<7)
#define TX_LED_OFF()	PORTD |= (1<<5)
#define TX_LED_ON()		PORTD &= ~(1<<5)
#define RX_LED_OFF()	PORTB |= (1<<0)
#define RX_LED_ON()		PORTB &= ~(1<<0)

#define TRANSFER_PGM		0x80
#define TRANSFER_RELEASE	0x40
#define TRANSFER_ZERO		0x20

void Transfer(u8 ep, const u8* data, int len);
void Recv(u8 ep, u8* dst, u8 len);
void Program(u8 ep, u16 page, u8 count);

/* HID is not fully-supported in the bootloader - can be enabled
   for testing, but note the descriptor report and other parts are 
   not complete */
//#define HID_ENABLED		

#include "USBCore.h"
#include "USBDesc.h"

