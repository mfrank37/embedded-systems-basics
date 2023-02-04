#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define PRESSED 0
#define RESET_SWITCH (PINA & (1<<PA0))
#define ROLL_SWITCH	 (PINA & (1<<PA1))

void turn_on_leds_corresponding_to(int n);
void turn_off_leds();

int main(void) {
	// Initialization //
	// set pins connected to switch as input
	DDRA &= ~(1<<PA0) & ~(1<<PA1);
	/*
	DDRA &= ~(1<<PA0);
	DDRA &= ~(1<<PA1);
	*/
	
	// enable internal pull up resistor
	PORTA |= (1<<PA0) | (1<<PA1);
	
	// set pins connected to LED as output
	DDRB |= (1<<PB0) | (1<<PB1) | (1<<PB2);
	
	
	turn_off_leds();
	
	int random_number;
	
	// Loop //
	while(1) {
		if(RESET_SWITCH == PRESSED) {
			turn_off_leds();
		}
		if(ROLL_SWITCH == PRESSED) {
			turn_off_leds();
			// wait 1 second
			_delay_ms(1000);
			// Generate a new random number
			random_number = rand() % 6 + 1;
			// Turn ON the LEDs corresponding to the random number
			turn_on_leds_corresponding_to(random_number);
		}
	}
}

void turn_on_leds_corresponding_to(int n) {
	switch (n) {
		case 1:
		PORTB &= ~(1<<PB0);
		break;
		case 2:
		PORTB &= ~(1<<PB1);
		break;
		case 3:
		PORTB &= ~(1<<PB0) & ~(1<<PB1);
		break;
		case 4:
		PORTB &= ~(1<<PB2);
		break;
		case 5:
		PORTB &= ~(1<<PB2) & ~(1<<PB0);
		break;
		case 6:
		PORTB &= ~(1<<PB2) & ~(1<<PB1);
		break;
	}
};

void turn_off_leds() {
	PORTB |= (1<<PB0) | (1<<PB1) | (1<<PB2);
}
