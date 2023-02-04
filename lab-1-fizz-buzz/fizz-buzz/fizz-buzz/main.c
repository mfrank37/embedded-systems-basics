#define F_CPU 1200000UL
#include <avr/io.h>
#include <util/delay.h>

#define ON_DELAY _delay_ms(950)
#define OFF_DELAY _delay_ms(50)

int main(void)
{
	// Initialization //
	// use DDRB to make four pins of port B as output
	DDRB |= 1 << PB0;
	DDRB |= 1 << PB1;
	DDRB |= 1 << PB2;
	DDRB |= 1 << PB3;
	
	int counter; // declares the counter
	
	// Loop //
	// start the counter and set the incrementation. counter should be in range 0 <= counter < 1000
	for(counter=0; counter<1000; counter++) {
		
		// check if counter is divisible by 3 and 5, to turn on LED3 at PB2 then turn off.
		if(counter%3 == 0 && counter%5 == 0) {
			PORTB = PORTB | (1<<PB2);
			ON_DELAY;
			PORTB = PORTB & ~(1<<PB2);
			OFF_DELAY;
		}
		// check if counter is divisible by 3, to turn on LED1 at PB0 then turn off.
		else if(counter%3==0) {
			PORTB = PORTB | (1<<PB0);
			ON_DELAY;
			PORTB = PORTB & ~(1<<PB0);
			OFF_DELAY;
		}
		// check if counter is divisible by 5, to turn on LED2 at PB1 then turn off.
		else if(counter%5==0) {
			PORTB = PORTB | (1<<PB1);
			ON_DELAY;
			PORTB = PORTB & ~(1<<PB1);
			OFF_DELAY;
		// we shall turn on LED4 at PB3 if counter is neither divisible by 3 nor 5, then turn off.
		} else {
			PORTB = PORTB | (1<<PB3);
			ON_DELAY;
			PORTB = PORTB & ~(1<<PB3);
			OFF_DELAY;
		}
		
		// check if counter > 100 then reset it to zero.
		if(counter > 100) {
			counter = 0;
		}
	}
}
