#define F_CPU 9600000UL  // ATtiny13 runs at 9.6 MHz
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// PWM duty cycles for each color channel
uint8_t red_pwm = 0;
uint8_t green_pwm = 0;
uint8_t blue_pwm = 0;

// RGB pin assignments
#define RED_PIN PB0
#define GREEN_PIN PB1
#define BLUE_PIN PB2

// PWM counter and compare values
uint8_t pwm_counter = 0;
uint8_t red_compare = 0;
uint8_t green_compare = 0;
uint8_t blue_compare = 0;

void set_rgb_pwm(uint8_t red, uint8_t green, uint8_t blue) {
	red_pwm = red;
	green_pwm = green;
	blue_pwm = blue;

	// Calculate compare values for each color
	red_compare = (red_pwm * 255) / 100;
	green_compare = (green_pwm * 255) / 100;
	blue_compare = (blue_pwm * 255) / 100;
}

ISR(TIM0_OVF_vect) {
	pwm_counter++;

	// Set RED pin
	if (pwm_counter < red_compare) {
		PORTB &= ~(1 << RED_PIN);
		} else {
		PORTB |= (1 << RED_PIN);
	}

	// Set GREEN pin
	if (pwm_counter < green_compare) {
		PORTB &= ~(1 << GREEN_PIN);
		} else {
		PORTB |= (1 << GREEN_PIN);
	}

	// Set BLUE pin
	if (pwm_counter < blue_compare) {
		PORTB &= ~(1 << BLUE_PIN);
		} else {
		PORTB |= (1 << BLUE_PIN);
	}

	// Reset counter
	if (pwm_counter == 255) {
		pwm_counter = 0;
	}
}

int main() {
	// Set RGB pins as output
	DDRB = (1 << RED_PIN) | (1 << GREEN_PIN) | (1 << BLUE_PIN);

	// Set initial color
	set_rgb_pwm(0, 0, 0);

	// Configure Timer0 for overflow interrupt
	TCCR0B |= (1 << CS00);  // No prescaler
	TIMSK0 |= (1 << TOIE0);  // Enable overflow interrupt
	sei();  // Enable global interrupts

	while (1) {
		// Gradually change to different colors
		for (uint8_t i = 0; i < 100; i++) {
			set_rgb_pwm(i, 100-i, 0);		// 100 0 0
			_delay_ms(10);    
		}
		for (uint8_t i = 0; i < 100; i++) {
			set_rgb_pwm(100-i, i, 0);		//0 100 0
			_delay_ms(10);
		}
		
		for (uint8_t i = 0; i < 100; i++) {
			set_rgb_pwm(0, 100-i, i);		// 0 0 100
			_delay_ms(10);
		}
		
		for (uint8_t i = 0; i < 100; i++) {
			set_rgb_pwm(0, i, 100-i);		//0 100 0
			_delay_ms(10);
		}
		

	}
	return 0;
}
