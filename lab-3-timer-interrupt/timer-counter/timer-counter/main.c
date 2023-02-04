#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 8000000UL // 8MHz clock
#define PRESCALER (1 << CS12) // prescaler of 256 => F_CPU/256
#define SECOND 31249 // 31,249 for 1 second

// instead of PB0 it's PORTB0 as on datasheet
// Although PB0 will also have no compile issues.
#define LED1 PORTB0
#define LED2 PORTB1
#define LED3 PORTB2
#define LED4 PORTB3
#define SW1 PORTD2
#define SW2 PORTD3

#define PRESSED 0
#define SW1_STATUS (PIND & (1 << SW1))

// We should be interrupted when INT0 (SW1) make the specified event
ISR(INT0_vect) {
	// is the SW1 pressed?
	if(SW1_STATUS == PRESSED) {
		PORTB &= ~(1 << LED2); // turn ON
	} else {
		PORTB |= (1 << LED2); // turn OFF
	}
}

// We should be interrupted when INT1 (SW2) make the specified event
ISR(INT1_vect) {
	PORTB &= ~(1<< LED3);
	// turn off after one second (Overflows may happen but the time is still deterministic).
	OCR1B = TCNT1 + SECOND;
}

// We should be interrupted when Timer is equal to OCR1A then toggle LED1 and LED4
ISR(TIMER1_COMPA_vect) {
	PORTB ^= (1 << LED1) | (1 << LED4);
	OCR1A += SECOND; // we want to be interrupted one second later to execute this ISR again.
}

// We should do this when Timer reaches(matches) OCR1B
ISR(TIMER1_COMPB_vect) {
		PORTB |= (1<< LED3);
}

int main(void) {
	// Initialization //
	// setup LEDs and SWITCHES
	DDRD &= ~(1 << SW1) & ~(1 << SW2); // set switch pins as input
	DDRB |= (1 << LED1) | (1 << LED2) | (1 << LED3) | (1 << LED4); // set LED pins as output
	PORTB |= (1 << LED1) | (1 << LED2) | (1 << LED3) | (1 << LED4); // turn OFF LEDs
	
	// setup timer(s)
	TCCR1B |= PRESCALER;
	OCR1A = SECOND; // We need to know if one second has left since program started.
	
	// enable Timer's Output Compare match B and Output Compare match A interrupts
	TIMSK1 |= (1 << OCIE1B) | (1 << OCIE1A);
	
	// setup interrupt
	// EIMSK: Enable Interrupt Mask
	EIMSK |= (1 << INT0) | (1 << INT1);
	
	/**	EICRA: External Interrupt Control Register.
	 *	EICRA helps us to know if Pin is pressed or released (rising edge / falling edge or both)
	 */
	
	// for SW1 (INT1) => 1 1 => rising edge (LOW to HIGH)
	EICRA |= (1 << ISC11) | (1 << ISC10); 
	
	// for SW2 (INT0) => 0 1 => both rising and falling edges
	EICRA &= ~(1 << ISC01);
	EICRA |= (1 << ISC00);
	
	// Set Interrupt. To enable interrupts globally. (from SREG, setting D7=1 or I=1)
	sei();
	
	while(1) {
		/**
		 * Just do something else,
		 * and/or wait to be interrupted when it's time to execute Interrupt Service Routine(s).
		 */
	}
}
