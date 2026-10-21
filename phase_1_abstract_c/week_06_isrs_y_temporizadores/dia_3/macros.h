#ifndef MACROS_H
#define MACROS_H

#define BIT_TOGGLE(REG, BIT) (REG ^= (1U << (BIT)))
#define BIT_SET(REG, BIT) (REG |= (1U << (BIT)))

#define REG_DDRB *(volatile uint8_t *)0x24
#define REG_PORTB *(volatile uint8_t *)0x25

#define PIN_LED_ESTADO 5

/* ==================================================================
 * PERIFÉRICO: TIMER 1 (16 bits)
 * ================================================================== */
// Registros de control (8 bits)
#define REG_TCCR1A *(volatile uint8_t *)0x80
#define REG_TCCR1B *(volatile uint8_t *)0x81

// Registros de conteo y comparación (16 bits)
#define REG_TCNT1 *(volatile uint16_t *)0x84
#define REG_OCR1A *(volatile uint16_t *)0x88

/* Bits específicos del registro TCCR1B (Datasheet) */
#define BIT_CS10 0
#define BIT_CS11 1
#define BIT_CS12 2
#define BIT_WGM12 3 // Este lo usaremos mañana para el modo CTC

#endif