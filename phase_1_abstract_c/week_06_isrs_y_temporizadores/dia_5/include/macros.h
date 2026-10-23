#ifndef MACROS_H
#define MACROS_H

#include <stdint.h>

/*
 * Capa 1: Mapa de memoria y hardware (punteros volatiles a direcciones fisicas)
 */
#define PORTB *(volatile uint8_t *)0x25
#define DDRB *(volatile uint8_t *)0x24
#define PIN_LED 5

#define TCCR1A *(volatile uint8_t *)0x80
#define TCCR1B *(volatile uint8_t *)0x81
#define TIMSK1 *(volatile uint8_t *)0x6F

#define TCNT1 *(volatile uint16_t *)0x84
#define OCR1A *(volatile uint16_t *)0x88

/*
 * Macros Bitwise (Capa de manipulacion)
 */
#define BIT_SET(reg, bit) ((reg) |= (1U << (bit)))
#define BIT_CLEAR(reg, bit) ((reg) &= ~(1U << (bit)))
#define BIT_TOGGLE(reg, bit) ((reg) ^= (1U << (bit)))

#endif /* MACROS_H */
