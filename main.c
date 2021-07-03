#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define DISP_DDR	DDRA					///< DDR of the display.
#define DISP_PORT	PORTA					///< Port of the display. 

void PORT_INIT(void);
void INT_INIT(void);

volatile uint8_t direction = 0;

/*!
 *	@brief ISR for INT0 external interrupt; Switches button state at a rising edge at INT0. 
 */

ISR(INT0_vect){
	if(!(direction)){
		direction = 1;
	}
	else{
		direction = 0;
	}
}

int main(void){
	PORT_INIT();
	INT_INIT();
	
	uint8_t val = 0;					///< Variable to hold value to be displayed.
	
	sei();							///< Enable global interrupts.
	
	while(1){
		
		if(!(direction)){
			val++;
		}
		else{
			val--;
		}
		if(val > 255){
			val = 0;				///< Sets val back to minimum value if it reaches the maximum value.
		}
		else if(val < 0){
			val = 255;				///< Sets val to maximum value if it reaches the minimum value.
		}
		
		PORTA = val;
		_delay_ms(500);
	}
}

/*!
 *	@brief Initialize Ports.
 */

void PORT_INIT(void){
	DISP_DDR = 0xFF;
	DDRD &= ~(1<<PD2);					///< Set INT0 pin to input.
}

/*!
 *	@brief Initialize External Interrupt 0 at INT0 pin.
 */

void INT_INIT(void){
	MCUCR |= (1<<ISC00)|(1<<ISC01);				///< A rising edge at INT0 generates an interrupt request.
	GICR |= (1<<INT0);					///< Enable INT0 external interrupt request.
}
